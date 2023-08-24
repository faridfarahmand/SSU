import subprocess
import tkinter as tk
import threading
import time

# Define the hosts you want to ping
# You can replace the keys of this dictionary to customize the labels in the UI
hosts = {
    "Arduino Camera -> 192.168.0.172": "192.168.0.172",
    "Desktop -> 192.168.0.120": "192.168.0.120",
    "Raspberry Pi -> 192.168.0.121": "192.168.0.121",
    "Kali Virtual Machine -> 192.168.0.147": "192.168.0.147",
}

# Function to perform the ping
def ping(host):
    # Execute the ping command and discard output. Return True if ping successful, else False.
    # Sends 1 ICMP packet to speed up response time
    return subprocess.run(['ping', '-c', '1', host], stdout=subprocess.DEVNULL).returncode == 0

# Function to create the GUI
def create_gui(hosts):
    # Create a tkinter window
    root = tk.Tk()
    # Set the title of the window
    root.title("LAN Monitor")

    # Create a label for each host with initial 'green' status
    host_labels = {name: tk.Label(root, text=name, bg="green", bd=2, relief="solid") for name in hosts.keys()}

    # Add each label to the window on a grid, and make it expandable
    for i, label in enumerate(host_labels.values()):
        label.grid(row=i, column=0, sticky="nsew")
        root.grid_rowconfigure(i, weight=1)

    root.grid_columnconfigure(0, weight=1)

    # Function to update the host statuses
    def update_host_statuses():
        while True:
            # Loop through the hosts and update the label color based on ping status
            for name, label in host_labels.items():
                host = hosts[name]
                # Set the label color to 'red' if the host is not reachable, else 'green'
                label.config(bg="red" if not ping(host) else "green")
            # Wait for 1 seconds before the next round of pings
            time.sleep(1)

    # Start a new thread that will update the host statuses
    threading.Thread(target=update_host_statuses, daemon=True).start()

    # Start the tkinter event loop
    root.mainloop()

def main():
    # Create the GUI
    create_gui(hosts)

if __name__ == "__main__":
    main()
