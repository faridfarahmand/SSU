##############################################################################
# EmergiNet Battery Monitor  
#
# Description:
# This program requests telemetry data from selected Meshtastic nodes and
# displays battery level, uptime, and node status in a graphical interface.
#
# Features:
# - Load node information from residents.csv
# - Select individual nodes or all nodes
# - Send telemetry requests with randomized delays
# - Display battery level and uptime
# - Visual battery status indicators:
#       🟢 Battery >= 80%
#       🟡 Battery 60–79%
#       🔴 Battery < 60%
#       ❌ No response
#
# Farid Farahmand 
# EmergiNet Project
# Version 1.0
###############################################################################


import csv
import os
import random
import re
import subprocess
import sys
import threading
import queue
import time
import tkinter as tk
from tkinter import ttk, filedialog, messagebox

DEFAULT_CSV_FILE = "residents.csv"
DEFAULT_PORT = "/dev/cu.usbserial-0001"

MIN_DELAY_SEC = 60       # 1 minute
MAX_DELAY_SEC = 180      # 3 minutes
REQUEST_TIMEOUT_SEC = 90


class TelemetryGUI(tk.Tk):
    def __init__(self):
        super().__init__()

        self.title("EmergiNet Telemetry Request")
        self.geometry("950x600")

        self.nodes = []
        self.ui_queue = queue.Queue()
        self.worker_running = False

        self._build_ui()
        self.after(200, self._process_queue)

    def _build_ui(self):
        top = ttk.LabelFrame(self, text="Settings")
        top.pack(fill="x", padx=10, pady=10)

        ttk.Label(top, text="CSV File:").grid(row=0, column=0, padx=5, pady=5, sticky="w")
        self.csv_var = tk.StringVar(value=DEFAULT_CSV_FILE)
        ttk.Entry(top, textvariable=self.csv_var, width=55).grid(row=0, column=1, padx=5, pady=5)

        ttk.Button(top, text="Browse", command=self._browse_file).grid(row=0, column=2, padx=5)
        ttk.Button(top, text="LOAD/CLR", command=self._load_csv).grid(row=0, column=3, padx=5)

        ttk.Label(top, text="Port:").grid(row=1, column=0, padx=5, pady=5, sticky="w")
        self.port_var = tk.StringVar(value=DEFAULT_PORT)
        ttk.Entry(top, textvariable=self.port_var, width=30).grid(row=1, column=1, padx=5, pady=5, sticky="w")

        mid = ttk.Frame(self)
        mid.pack(fill="both", expand=True, padx=10, pady=5)

        left = ttk.LabelFrame(mid, text="Residents / Nodes")
        left.pack(side="left", fill="both", expand=True, padx=(0, 5))

        self.listbox = tk.Listbox(left, selectmode=tk.MULTIPLE, height=18)
        self.listbox.pack(fill="both", expand=True, padx=8, pady=8)

        btns = ttk.Frame(left)
        btns.pack(fill="x", padx=8, pady=8)

        ttk.Button(btns, text="SELECT ALL", command=self._select_all).pack(side="left", padx=5)
        ttk.Button(btns, text="CLEAR ALL", command=self._clear_all).pack(side="left", padx=5)
        ttk.Button(btns, text="SEND", command=self._start_requests).pack(side="right", padx=5)

        right = ttk.LabelFrame(mid, text="Telemetry Results")
        right.pack(side="right", fill="both", expand=True, padx=(5, 0))

        columns = ("name", "device_id", "battery", "uptime", "status")
        self.tree = ttk.Treeview(right, columns=columns, show="headings")

        self.tree.heading("name", text="Name")
        self.tree.heading("device_id", text="Device ID")
        self.tree.heading("battery", text="Battery")
        self.tree.heading("uptime", text="Uptime")
        self.tree.heading("status", text="Status")

        self.tree.column("name", width=160)
        self.tree.column("device_id", width=130)
        self.tree.column("battery", width=120)
        self.tree.column("uptime", width=120)
        self.tree.column("status", width=120)

        self.tree.pack(fill="both", expand=True, padx=8, pady=8)

        bottom = ttk.LabelFrame(self, text="Log")
        bottom.pack(fill="both", expand=False, padx=10, pady=10)

        self.log = tk.Text(bottom, height=7)
        self.log.pack(fill="both", expand=True, padx=8, pady=8)

    def _browse_file(self):
        filename = filedialog.askopenfilename(
            title="Select residents.csv",
            filetypes=[("CSV Files", "*.csv"), ("All Files", "*.*")]
        )
        if filename:
            self.csv_var.set(filename)

    def _load_csv(self):
        path = self.csv_var.get().strip()

        if not os.path.exists(path):
            messagebox.showerror("File Error", f"File not found:\n{path}")
            return

        self.nodes.clear()
        self.listbox.delete(0, tk.END)
        self.tree.delete(*self.tree.get_children())

        with open(path, "r", newline="", encoding="utf-8") as f:
            reader = csv.reader(f)

            for row in reader:
                if len(row) < 3:
                    continue

                first = row[0].strip()
                last = row[1].strip()
                device_id = row[2].strip()

                if first.lower() == "first" or device_id.lower() == "device_id":
                    continue

                if not device_id:
                    continue

                if not device_id.startswith("!"):
                    device_id = "!" + device_id

                name = f"{first} {last}".strip()
                self.nodes.append((name, device_id))

                self.listbox.insert(tk.END, f"{name}   ({device_id})")

                self.tree.insert(
                    "",
                    "end",
                    iid=device_id,
                    values=(name, device_id, "-", "-", "Not requested")
                )

        self._log(f"Loaded {len(self.nodes)} nodes from {path}")

    def _select_all(self):
        self.listbox.select_set(0, tk.END)

    def _clear_all(self):
        self.listbox.selection_clear(0, tk.END)

    def _start_requests(self):
        if self.worker_running:
            messagebox.showwarning("Busy", "Telemetry requests are already running.")
            return

        selected = self.listbox.curselection()
        if not selected:
            messagebox.showwarning("No Selection", "Please select at least one node.")
            return

        selected_nodes = [self.nodes[i] for i in selected]

        self.worker_running = True

        t = threading.Thread(
            target=self._worker_send_requests,
            args=(selected_nodes,),
            daemon=True
        )
        t.start()

    def _worker_send_requests(self, selected_nodes):
        port = self.port_var.get().strip()

        for name, device_id in selected_nodes:
            delay_sec = random.randint(MIN_DELAY_SEC, MAX_DELAY_SEC)

            self.ui_queue.put(("log", f"Waiting {delay_sec} seconds before requesting telemetry from {name} {device_id}"))
            self.ui_queue.put(("status", device_id, "Waiting"))

            time.sleep(delay_sec)

            self.ui_queue.put(("log", f"Requesting telemetry from {name} {device_id}"))
            self.ui_queue.put(("status", device_id, "Requesting"))

            cmd = [
                sys.executable,
                "-m",
                "meshtastic",
                "--port",
                port,
                "--request-telemetry",
                "--dest",
                device_id
            ]

            try:
                result = subprocess.run(
                    cmd,
                    capture_output=True,
                    text=True,
                    timeout=REQUEST_TIMEOUT_SEC
                )

                output = (result.stdout or "") + "\n" + (result.stderr or "")

                battery, uptime = self._parse_telemetry(output)

                if battery is None and uptime is None:
                    self.ui_queue.put(("result", device_id, "❌", "No response", "Timeout/No data"))
                    self.ui_queue.put(("log", f"No valid telemetry response from {device_id}"))
                else:
                    emoji = self._battery_emoji(battery)
                    batt_text = f"{battery:.2f}%" if battery is not None else "Unknown"
                    uptime_text = uptime if uptime is not None else "Unknown"

                    self.ui_queue.put(("result", device_id, emoji, batt_text, uptime_text))
                    self.ui_queue.put(("log", f"Received telemetry from {device_id}: Battery={batt_text}, Uptime={uptime_text}"))

            except subprocess.TimeoutExpired:
                self.ui_queue.put(("result", device_id, "❌", "No response", "Timeout"))
                self.ui_queue.put(("log", f"Timeout waiting for {device_id}"))

            except Exception as e:
                self.ui_queue.put(("result", device_id, "❌", "Error", str(e)))
                self.ui_queue.put(("log", f"Error for {device_id}: {e}"))

        self.ui_queue.put(("done",))

    def _parse_telemetry(self, text):
        battery = None
        uptime = None

        batt_patterns = [
            r"Battery level:\s*([0-9.]+)\s*%",
            r"batteryLevel['\"]?\s*[:=]\s*([0-9.]+)",
            r"Battery:\s*([0-9.]+)\s*%"
        ]

        uptime_patterns = [
            r"Uptime:\s*([0-9]+)\s*s",
            r"uptimeSeconds['\"]?\s*[:=]\s*([0-9]+)",
            r"Uptime:\s*([0-9]+)"
        ]

        for p in batt_patterns:
            m = re.search(p, text, re.IGNORECASE)
            if m:
                battery = float(m.group(1))
                break

        for p in uptime_patterns:
            m = re.search(p, text, re.IGNORECASE)
            if m:
                uptime = f"{m.group(1)} s"
                break

        return battery, uptime

    def _battery_emoji(self, battery):
        if battery is None:
            return "❌"

        if battery < 60:
            return "🔴"

        if battery < 80:
            return "🟡"

        return "🟢"

    def _process_queue(self):
        try:
            while True:
                msg = self.ui_queue.get_nowait()

                if msg[0] == "log":
                    self._log(msg[1])

                elif msg[0] == "status":
                    _, device_id, status = msg
                    self._update_status(device_id, "-", "-", status)

                elif msg[0] == "result":
                    _, device_id, emoji, battery, uptime = msg
                    self._update_status(device_id, battery, uptime, emoji)

                elif msg[0] == "done":
                    self.worker_running = False
                    self._log("All selected telemetry requests completed.")

        except queue.Empty:
            pass

        self.after(200, self._process_queue)

    def _update_status(self, device_id, battery, uptime, status):
        if device_id not in self.tree.get_children():
            return

        current = self.tree.item(device_id, "values")
        name = current[0]

        self.tree.item(
            device_id,
            values=(name, device_id, battery, uptime, status)
        )

    def _log(self, msg):
        self.log.insert(tk.END, msg + "\n")
        self.log.see(tk.END)


if __name__ == "__main__":
    app = TelemetryGUI()
    app.mainloop()
