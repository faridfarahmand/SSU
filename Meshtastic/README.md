## EmergiNet – Future Work

### Local Directories
- **GUI:**  
  `/Users/farid/PycharmProjects/PuhabiProject`
- **Wio Terminal Programming:**  
  `/Users/farid/Documents/Arduino/Wio_Meshtastic_RX_Buz_working`

---

### Known Issues (Current Bugs)
- Some units transmit a **RECEIVED** message immediately on startup.
- The **RECEIVED** message is currently **broadcast to all nodes** instead of being targeted.
- The **siren command** is broadcast to all nodes; it should be sent **only to the intended node(s)**.
- Node responses and acknowledgments should be sent **exclusively to the NetController**, not to other member nodes.

---

### Planned Enhancements (New Features)
- Add a GUI tab to display **node status and health**, including:
  - Battery level
  - Charging state
  - Firmware/software version

