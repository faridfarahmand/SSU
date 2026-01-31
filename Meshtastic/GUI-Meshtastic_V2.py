"""
EmergiNet - Resident Response Checker (GUI) using Meshtastic USB

Tabs
    - RESPONSE: Connect/listen to Meshtastic on the SAME USB port (/dev/cu.usbserial-0001),
    extract FROM (fromId) from incoming packets, display Last FROM, and match to residents.csv.
    Shows a full-row logo at the top of the RESPONSE tab only.
- NO_RESPONSE: Shows residents.csv entries not yet matched.
- SEND: Send arbitrary text over the SAME open Meshtastic interface (multiple sends).
- ALARM: Button "Hacienda 1" broadcasts xSiren!E1 over the SAME open Meshtastic interface.

Dependencies
- tkinter must be available in your Python build
- tkcalendar optional: pip install tkcalendar
- meshtastic: pip install meshtastic
- pypubsub: pip install pypubsub
- pillow: pip install pillow

Files
- residents.csv (first,last,device_id)
- hacienda4.png (optional, shown only on RESPONSE tab as a full-row banner)

Updates 
- 1/31/26  
  Added a HELP! message to teh node. If buttons B and C are pressed (LOW) on the node the GUI will detect the message and show SOS for that individual. 
  The HELP! message can be sent several times.
"""

import csv
import os
import queue
import time
from dataclasses import dataclass
from typing import Dict, List, Optional, Set, Tuple

import tkinter as tk
from tkinter import ttk, messagebox

#TARGET_SIREN_NODE_ID = "!433e287c"  # Node with Siren
TARGET_SIREN_NODE_ID = "!43b797fc"


# Optional calendar widget
try:
    from tkcalendar import DateEntry  # type: ignore
    TKCALENDAR_AVAILABLE = True
except Exception:
    TKCALENDAR_AVAILABLE = False

# Meshtastic + pubsub
try:
    import meshtastic.serial_interface  # type: ignore
    from pubsub import pub  # type: ignore
    MESHTASTIC_AVAILABLE = True
except Exception:
    MESHTASTIC_AVAILABLE = False

# Pillow (logo scaling)
try:
    from PIL import Image, ImageTk  # type: ignore
    PIL_AVAILABLE = True
except Exception:
    PIL_AVAILABLE = False


# ---- CSV file (first,last,device_id) ----
CSV_FILE = "residents.csv"

EMOJI_OK = "✅"
EMOJI_NO = "❌"
EMOJI_HELP = "🆘"


READ_POLL_MS = 100

# ---- Same USB port 0001 ----
MESHTASTIC_PORT = "/dev/cu.usbserial-0001"
MESHTASTIC_CHANNEL_INDEX = 0

# ---- Full-row logo (RESPONSE tab only) ----
LOGO_PATH = "hacienda4.png"
LOGO_SIZE_PX = 82  # logo height (px)


def normalize_device_id(device_id: str) -> str:
    s = (device_id or "").strip()
    if s.startswith("!"):
        s = s[1:]
    return s.lower()


@dataclass(frozen=True)
class Resident:
    first: str
    last: str
    device_id_raw: str

    @property
    def full_name(self) -> str:
        name = f"{self.first} {self.last}".strip()
        return " ".join(name.split())

    @property
    def device_id_norm(self) -> str:
        return normalize_device_id(self.device_id_raw)


def load_residents(csv_path: str) -> Tuple[Dict[str, Resident], List[str]]:
    mapping: Dict[str, Resident] = {}
    errors: List[str] = []

    if not os.path.exists(csv_path):
        errors.append(f"CSV file not found: {csv_path}")
        return mapping, errors

    with open(csv_path, "r", newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        for i, row in enumerate(reader, start=1):
            if not row or all(not c.strip() for c in row):
                continue
            if len(row) < 3:
                errors.append(f"Line {i}: expected 3 columns (first,last,device_id), got {len(row)}")
                continue

            first = row[0].strip()
            last = row[1].strip()
            dev = row[2].strip()

            if not dev:
                errors.append(f"Line {i}: missing device_id")
                continue

            resident = Resident(first=first, last=last, device_id_raw=dev)
            mapping[resident.device_id_norm] = resident

    return mapping, errors


def extract_from_id_from_packet(packet) -> Optional[str]:
    """
    Robustly extract sender ID from Meshtastic receive payloads.
    Handles dict payloads and object payloads (depending on meshtastic version and packet type).
    """

    def _get_from_mapping(d: dict) -> Optional[str]:
        # Direct keys (common)
        for k in ("fromId", "from", "from_id", "FROM"):
            v = d.get(k)
            if v:
                return str(v).strip()

        # Common nested container
        inner = d.get("packet")
        if isinstance(inner, dict):
            for k in ("fromId", "from"):
                v = inner.get(k)
                if v:
                    return str(v).strip()

        # Another nesting some versions use
        decoded = d.get("decoded")
        if isinstance(decoded, dict):
            inner2 = decoded.get("packet")
            if isinstance(inner2, dict):
                for k in ("fromId", "from"):
                    v = inner2.get(k)
                    if v:
                        return str(v).strip()

        return None

    # Dict-style
    if isinstance(packet, dict):
        return _get_from_mapping(packet)

    # Object-style
    try:
        for attr in ("fromId", "from_id", "from"):
            if hasattr(packet, attr):
                v = getattr(packet, attr)
                if v:
                    return str(v).strip()

        # Try __dict__ if present
        if hasattr(packet, "__dict__"):
            d = packet.__dict__
            if isinstance(d, dict):
                got = _get_from_mapping(d)
                if got:
                    return got

        # Last resort: parse string representation
        s = str(packet)
        for token in ("fromId='", 'fromId="', "fromId:"):
            idx = s.find(token)
            if idx != -1:
                tail = s[idx + len(token):]
                for stop in ("'", '"', " ", ",", "}"):
                    j = tail.find(stop)
                    if j != -1:
                        tail = tail[:j]
                        break
                tail = tail.strip()
                if tail:
                    return tail
    except Exception:
        pass

    return None


def extract_text_from_packet(packet) -> Optional[str]:
    """
    Extract decoded text from a meshtastic receive packet.
    Used to detect HELP messages so RESPONSE tab can show EMOJI_HELP.
    """

    def _from_dict(d: dict) -> Optional[str]:
        # Most common: packet["decoded"]["text"]
        decoded = d.get("decoded")
        if isinstance(decoded, dict):
            t = decoded.get("text")
            if isinstance(t, str) and t.strip():
                return t.strip()

        # Sometimes: d["text"] exists
        t2 = d.get("text")
        if isinstance(t2, str) and t2.strip():
            return t2.strip()

        return None

    if isinstance(packet, dict):
        return _from_dict(packet)

    # Object-style: try attributes and __dict__
    try:
        for attr in ("text",):
            if hasattr(packet, attr):
                v = getattr(packet, attr)
                if isinstance(v, str) and v.strip():
                    return v.strip()

        if hasattr(packet, "__dict__"):
            d = packet.__dict__
            if isinstance(d, dict):
                got = _from_dict(d)
                if got:
                    return got
    except Exception:
        pass

    return None


def load_logo_full_row(path: str, target_height: int):
    """
    Load and scale logo to a fixed height, preserving aspect ratio.
    Intended as a full-row banner (width expands visually via layout).
    """
    if not PIL_AVAILABLE:
        return None
    if not os.path.exists(path):
        return None

    img = Image.open(path).convert("RGBA")
    w, h = img.size
    if h <= 0:
        return None

    scale = target_height / float(h)
    new_w = max(1, int(w * scale))
    img = img.resize((new_w, target_height), Image.LANCZOS)
    return ImageTk.PhotoImage(img)


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("EmergiNet - Response Checker (Meshtastic)")
        self.geometry("1020x640")

        self.residents: Dict[str, Resident] = {}
        self.matched: Set[str] = set()
        self.no_item_by_id: Dict[str, str] = {}

        # Meshtastic interface (shared for receive + send + alarm)
        self.mesh_iface = None
        self._subscribed = False

        # Queue for UI updates from meshtastic callbacks
        self.ui_queue: "queue.Queue[Tuple[str, str]]" = queue.Queue()

        # Keep references for images
        self.logo_img = None

        self._build_ui()
        self._load_csv(show_warnings=True)
        self._rebuild_no_response()

        self.after(READ_POLL_MS, self._process_ui_queue)
        self.protocol("WM_DELETE_WINDOW", self._on_close)

    # ---------------- UI ----------------

    def _build_ui(self) -> None:
        self.notebook = ttk.Notebook(self)
        self.notebook.pack(fill="both", expand=True, padx=10, pady=10)

        self.tab_response = ttk.Frame(self.notebook)
        self.tab_no_response = ttk.Frame(self.notebook)
        self.tab_send = ttk.Frame(self.notebook)
        self.tab_alarm = ttk.Frame(self.notebook)

        self.notebook.add(self.tab_response, text="RESPONSE")
        self.notebook.add(self.tab_no_response, text="NO_RESPONSE")
        self.notebook.add(self.tab_send, text="SEND")
        self.notebook.add(self.tab_alarm, text="ALARM")

        self._build_response_tab()
        self._build_no_response_tab()
        self._build_send_tab()
        self._build_alarm_tab()

    def _build_response_tab(self) -> None:
        # ---- Full-row logo (RESPONSE tab only) ----
        logo_row = ttk.Frame(self.tab_response)
        logo_row.pack(fill="x", padx=10, pady=(8, 4))
        logo_row.columnconfigure(0, weight=1)

        self.logo_img = load_logo_full_row(LOGO_PATH, LOGO_SIZE_PX)
        if self.logo_img is not None:
            logo_label = ttk.Label(logo_row, image=self.logo_img, anchor="center")
            logo_label.grid(row=0, column=0, sticky="ew")

        controls = ttk.LabelFrame(self.tab_response, text="Controls")
        controls.pack(fill="x", padx=10, pady=10)

        ttk.Label(controls, text="Select date:").grid(row=0, column=0, sticky="w", padx=6, pady=6)
        if TKCALENDAR_AVAILABLE:
            self.date_entry = DateEntry(controls, width=14)
            self.date_entry.grid(row=0, column=1, sticky="w", padx=6, pady=6)
        else:
            self.day_var = tk.StringVar(value=str(time.localtime().tm_mday))
            self.date_entry = ttk.Entry(controls, width=6, textvariable=self.day_var)
            self.date_entry.grid(row=0, column=1, sticky="w", padx=6, pady=6)
            ttk.Label(controls, text="(Day of month)").grid(row=0, column=2, sticky="w", padx=6, pady=6)

        ttk.Label(controls, text=f"Meshtastic USB port: {MESHTASTIC_PORT}").grid(
            row=1, column=0, columnspan=3, sticky="w", padx=6, pady=6
        )

        self.start_btn = ttk.Button(controls, text="CHECK (Start Listening)", command=self._start_listening)
        self.start_btn.grid(row=0, column=5, sticky="e", padx=10, pady=6)

        self.stop_btn = ttk.Button(controls, text="STOP", command=self._stop_listening, state="disabled")
        self.stop_btn.grid(row=1, column=5, sticky="e", padx=10, pady=6)

        test_frame = ttk.LabelFrame(self.tab_response, text="Manual Test (Paste FROM value)")
        test_frame.pack(fill="x", padx=10, pady=8)

        ttk.Label(test_frame, text="FROM value:").grid(row=0, column=0, padx=6, pady=6, sticky="w")
        self.manual_from_var = tk.StringVar()
        self.manual_from_entry = ttk.Entry(test_frame, textvariable=self.manual_from_var, width=28)
        self.manual_from_entry.grid(row=0, column=1, padx=6, pady=6, sticky="w")

        self.manual_btn = ttk.Button(test_frame, text="TEST", command=self._manual_test)
        self.manual_btn.grid(row=0, column=2, padx=10, pady=6, sticky="w")

        self.reload_btn = ttk.Button(test_frame, text=f"Reload {CSV_FILE}", command=self._reload_csv)
        self.reload_btn.grid(row=0, column=3, padx=10, pady=6, sticky="w")

        self.reset_btn = ttk.Button(test_frame, text="Reset Matches", command=self._reset_matches)
        self.reset_btn.grid(row=0, column=4, padx=10, pady=6, sticky="w")

        frame = ttk.LabelFrame(self.tab_response, text="RESPONSE (Matched)")
        frame.pack(fill="both", expand=True, padx=10, pady=10)

        self.status_var = tk.StringVar(value="Ready.")
        ttk.Label(frame, textvariable=self.status_var).pack(anchor="w", padx=8, pady=4)

        self.last_from_var = tk.StringVar(value="Last FROM: (none)")
        ttk.Label(frame, textvariable=self.last_from_var).pack(anchor="w", padx=8, pady=4)

        cols = ("status", "device_id", "name")
        self.resp_tree = ttk.Treeview(frame, columns=cols, show="headings", height=12)
        self.resp_tree.heading("status", text="")
        self.resp_tree.heading("device_id", text="Device ID")
        self.resp_tree.heading("name", text="Name")

        self.resp_tree.column("status", width=50, anchor="center")
        self.resp_tree.column("device_id", width=220, anchor="w")
        self.resp_tree.column("name", width=650, anchor="w")
        self.resp_tree.pack(fill="both", expand=True, padx=8, pady=8)

    def _build_no_response_tab(self) -> None:
        frame = ttk.LabelFrame(self.tab_no_response, text="NO_RESPONSE (Unmatched, Live)")
        frame.pack(fill="both", expand=True, padx=10, pady=10)

        cols = ("status", "device_id", "name")
        self.no_tree = ttk.Treeview(frame, columns=cols, show="headings", height=18)
        self.no_tree.heading("status", text="")
        self.no_tree.heading("device_id", text="Device ID")
        self.no_tree.heading("name", text="Name")

        self.no_tree.column("status", width=50, anchor="center")
        self.no_tree.column("device_id", width=220, anchor="w")
        self.no_tree.column("name", width=650, anchor="w")

        self.no_tree.pack(fill="both", expand=True, padx=8, pady=8)

        self.count_var = tk.StringVar(value="")
        ttk.Label(frame, textvariable=self.count_var).pack(anchor="w", padx=8, pady=6)

    def _build_send_tab(self) -> None:
        frame = ttk.LabelFrame(self.tab_send, text="SEND (Meshtastic RF broadcast)")
        frame.pack(fill="both", expand=True, padx=12, pady=12)

        ttk.Label(
            frame,
            text=f"Type a message and press SEND.\nMeshtastic port: {MESHTASTIC_PORT} | channelIndex: {MESHTASTIC_CHANNEL_INDEX}",
            justify="left"
        ).pack(anchor="w", padx=10, pady=8)

        self.send_text = tk.Text(frame, height=10, wrap="word")
        self.send_text.pack(fill="both", expand=True, padx=10, pady=10)

        btn_row = ttk.Frame(frame)
        btn_row.pack(fill="x", padx=10, pady=10)

        self.send_btn = ttk.Button(btn_row, text="SEND", command=self._send_message_meshtastic)
        self.send_btn.pack(side="left")

        ttk.Button(btn_row, text="Clear", command=lambda: self.send_text.delete("1.0", "end")).pack(side="left", padx=10)

        self.send_status_var = tk.StringVar(value="Meshtastic: not connected")
        ttk.Label(frame, textvariable=self.send_status_var).pack(anchor="w", padx=10, pady=6)

    def _build_alarm_tab(self) -> None:
        frame = ttk.LabelFrame(self.tab_alarm, text="ALARM")
        frame.pack(fill="both", expand=True, padx=12, pady=12)

        ttk.Label(
            frame,
            text="Press a button below to broadcast an alarm command to the mesh.",
            justify="left"
        ).pack(anchor="w", padx=10, pady=10)

        btn_row = ttk.Frame(frame)
        btn_row.pack(fill="x", padx=10, pady=10)

        self.hacienda_btn = ttk.Button(btn_row, text="Hacienda 1", command=self._alarm_hacienda_1)
        self.hacienda_btn.pack(side="left")

        self.alarm_status_var = tk.StringVar(value="")
        ttk.Label(frame, textvariable=self.alarm_status_var).pack(anchor="w", padx=10, pady=6)

    # ---------------- ALARM ----------------

    def _alarm_hacienda_1(self) -> None:
        if not MESHTASTIC_AVAILABLE:
            messagebox.showerror("Missing Dependency", "Meshtastic not installed. Run: pip install meshtastic")
            return

        try:
            self._ensure_meshtastic_connected()
        except Exception as e:
            messagebox.showerror("Meshtastic", f"Not connected: {e}")
            return

        try:
            self.mesh_iface.sendText(
                "xSiren!E1",
                channelIndex=MESHTASTIC_CHANNEL_INDEX,
                destinationId=TARGET_SIREN_NODE_ID
            )
            self.alarm_status_var.set(f"Direct sent to {TARGET_SIREN_NODE_ID}: xSiren!E1 (Hacienda 1)")

        except Exception as e:
            self.alarm_status_var.set(f"Send failed: {e}")
            try:
                if self.mesh_iface is not None:
                    self.mesh_iface.close()
            except Exception:
                pass
            self.mesh_iface = None

    # ---------------- CSV ----------------

    def _load_csv(self, show_warnings: bool) -> None:
        mapping, errors = load_residents(CSV_FILE)
        self.residents = mapping

        if show_warnings and errors:
            msg = "Issues while loading residents.csv:\n\n" + "\n".join(errors[:15])
            if len(errors) > 15:
                msg += f"\n... and {len(errors) - 15} more."
            messagebox.showwarning("CSV Load Warnings", msg)

        if not self.residents:
            self.status_var.set(f"Loaded 0 entries. Check {CSV_FILE}.")
        else:
            self.status_var.set(f"Loaded {len(self.residents)} residents from {CSV_FILE}.")

    def _reload_csv(self) -> None:
        self._load_csv(show_warnings=True)
        self._rebuild_no_response()
        self.status_var.set(f"Reloaded {CSV_FILE}.")

    # ---------------- Tables ----------------

    def _rebuild_no_response(self) -> None:
        self._clear_tree(self.no_tree)
        self.no_item_by_id.clear()

        if not self.residents:
            self.count_var.set("NO_RESPONSE count: 0 (no residents loaded)")
            return

        unmatched = sorted(set(self.residents.keys()) - self.matched)
        for dev_norm in unmatched:
            r = self.residents[dev_norm]
            item = self.no_tree.insert("", "end", values=(EMOJI_NO, r.device_id_raw, f"{EMOJI_NO} {r.full_name}"))
            self.no_item_by_id[dev_norm] = item

        self.count_var.set(f"NO_RESPONSE count: {len(unmatched)}")

    def _update_no_count(self) -> None:
        if not self.residents:
            self.count_var.set("NO_RESPONSE count: 0 (no residents loaded)")
            return
        unmatched_count = len(set(self.residents.keys()) - self.matched)
        self.count_var.set(f"NO_RESPONSE count: {unmatched_count}")

    # ---------------- Meshtastic receive ----------------

    def _start_listening(self) -> None:
        if not MESHTASTIC_AVAILABLE:
            messagebox.showerror("Missing Dependency", "Meshtastic not installed. Run: pip install meshtastic")
            return

        self._load_csv(show_warnings=True)
        self._rebuild_no_response()

        try:
            self._ensure_meshtastic_connected()
        except Exception as e:
            messagebox.showerror("Meshtastic", f"Failed to connect: {e}")
            return

        self.start_btn.config(state="disabled")
        self.stop_btn.config(state="normal")
        self.status_var.set("Listening for Meshtastic packets...")

    def _stop_listening(self) -> None:
        self.start_btn.config(state="normal")
        self.stop_btn.config(state="disabled")
        self.status_var.set("Stopped listening (connection kept for SEND/ALARM).")

    def _ensure_meshtastic_connected(self) -> None:
        if not self._subscribed:
            pub.subscribe(self._on_meshtastic_receive, "meshtastic.receive")
            self._subscribed = True

        if self.mesh_iface is None:
            self.mesh_iface = meshtastic.serial_interface.SerialInterface(devPath=MESHTASTIC_PORT)
            try:
                self.send_status_var.set(f"Meshtastic: connected on {MESHTASTIC_PORT}")
            except Exception:
                pass

    def _on_meshtastic_receive(self, packet, interface=None, topic=None):
        try:
            from_id = extract_from_id_from_packet(packet)
            if from_id:
                txt = extract_text_from_packet(packet) or ""
                is_help = (txt.strip().upper() == "HELP!")
                self.ui_queue.put(("FROM", f"{from_id}|{'HELP!' if is_help else 'OK'}"))
            else:
                self.ui_queue.put(("DEBUG", f"Received packet but no fromId found (type={type(packet)})"))
        except Exception as e:
            self.ui_queue.put(("DEBUG", f"Receive handler error: {e}"))

    def _process_ui_queue(self) -> None:
        try:
            while True:
                kind, payload = self.ui_queue.get_nowait()
                if kind == "FROM":
                    parts = payload.split("|", 1)
                    from_id = parts[0]
                    tag = parts[1] if len(parts) > 1 else "OK"
                    is_help = (tag == "HELP!")

                    self.last_from_var.set(f"Last FROM: {from_id}")
                    self._handle_from_value(from_id, source="meshtastic", is_help=is_help)

                elif kind == "DEBUG":
                    self.status_var.set(payload)
        except queue.Empty:
            pass

        self.after(READ_POLL_MS, self._process_ui_queue)

    # ---------------- Matching ----------------

    def _manual_test(self) -> None:
        from_val = self.manual_from_var.get().strip()
        if not from_val:
            messagebox.showwarning("Input Required", "Please enter a FROM value (example: !9e9fc250).")
            return
        self.last_from_var.set(f"Last FROM: {from_val}")
        self._handle_from_value(from_val, source="manual", is_help=False)

    def _handle_from_value(self, from_value: str, source: str, is_help: bool = False) -> None:
        dev_norm = normalize_device_id(from_value)
        resident = self.residents.get(dev_norm)

        if not resident:
            self.status_var.set(f"Unknown FROM ({source}): {from_value}")
            return

        # If HELP! is received:
        # - Do NOT add to matched
        # - Do NOT remove from NO_RESPONSE
        # - Always show in RESPONSE with 🆘 (even if already matched or not in NO_RESPONSE)
        if is_help:
            self.resp_tree.insert(
                "",
                "end",
                values=(EMOJI_HELP, resident.device_id_raw, f"{EMOJI_HELP} {resident.full_name}")
            )
            self.status_var.set(f"HELP! received ({source}): {resident.full_name} ({resident.device_id_raw})")
            return

        if dev_norm in self.matched:
            self.status_var.set(f"Already matched ({source}): {resident.full_name} ({resident.device_id_raw})")
            return

        self.matched.add(dev_norm)

        self.resp_tree.insert(
            "",
            "end",
            values=(EMOJI_OK, resident.device_id_raw, f"{EMOJI_OK} {resident.full_name}")
        )

        no_item = self.no_item_by_id.get(dev_norm)
        if no_item:
            try:
                self.no_tree.delete(no_item)
            except Exception:
                pass
            self.no_item_by_id.pop(dev_norm, None)

        self._update_no_count()
        self.status_var.set(f"Matched ({source}): {resident.full_name} ({resident.device_id_raw})")

    def _reset_matches(self) -> None:
        self.matched.clear()
        self._clear_tree(self.resp_tree)
        self._rebuild_no_response()
        self.status_var.set("Reset matches. NO_RESPONSE now shows everyone.")
        self.last_from_var.set("Last FROM: (none)")

    # ---------------- SEND (Meshtastic) ----------------

    def _send_message_meshtastic(self) -> None:
        if not MESHTASTIC_AVAILABLE:
            messagebox.showerror("Missing Dependency", "Meshtastic not installed. Run: pip install meshtastic")
            return

        msg = self.send_text.get("1.0", "end").strip()
        if not msg:
            messagebox.showwarning("Message Required", "Please type a message before pressing SEND.")
            return

        try:
            self._ensure_meshtastic_connected()
        except Exception as e:
            messagebox.showerror("Meshtastic", f"Not connected: {e}")
            return

        try:
            self.mesh_iface.sendText(msg, channelIndex=MESHTASTIC_CHANNEL_INDEX)
            self.send_status_var.set(f"Sent: {msg}")
        except Exception as e:
            self.send_status_var.set(f"Send failed: {e}")
            try:
                if self.mesh_iface is not None:
                    self.mesh_iface.close()
            except Exception:
                pass
            self.mesh_iface = None

    # ---------------- Utils ----------------

    @staticmethod
    def _clear_tree(tree: ttk.Treeview) -> None:
        for item in tree.get_children():
            tree.delete(item)

    def _on_close(self) -> None:
        try:
            if self.mesh_iface is not None:
                self.mesh_iface.close()
        except Exception:
            pass
        self.destroy()


if __name__ == "__main__":
    App().mainloop()
