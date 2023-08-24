# Importing necessary libraries
import threading
from scapy.all import *
from tkinter import *
from tkinter.scrolledtext import ScrolledText

# A set to store unique MAC addresses
mac_addresses = set()

# Creating the root UI object
root = Tk()
# Setting the title of the UI window
root.title("New Device Detection")
# Creating a ScrolledText widget (a text display box with a scrollbar)
text_area = ScrolledText(root, width = 50, height = 50)

# Function to process each packet that is sniffed
def detect_new_device(packet):
    # Check if the packet is a Wi-Fi packet and is a probe request
    if packet.haslayer(Dot11) and packet.type == 0 and packet.subtype == 4:
        # If yes, pass the packet to the new_device function
        new_device(packet)

# Function to process new devices
def new_device(packet):
    # Extracting the MAC address from the packet
    mac = packet.addr2
    # If the MAC address is not already in the set of known MAC addresses
    if mac not in mac_addresses:
        # Add it to the set
        mac_addresses.add(mac)
        # Extracting the SSID from the packet
        ssid = next((elt.info for elt in packet[Dot11Elt] if isinstance(elt, Dot11Elt) and elt.ID == 0), None)
        # If there is an SSID, try to decode it
        if ssid is not None:
            try:
                ssid = ssid.decode()
            except UnicodeDecodeError:
                ssid = "Cannot decode SSID"
        # Add a line in the ScrolledText widget with the MAC address and SSID
        text_area.insert(INSERT, f"MAC: {mac}, SSID: {ssid}\n")

# Function to start sniffing the network
def sniff_network():
    # Start sniffing the network interface with Scapy's sniff function
    sniff(iface="wlan0mon", prn=detect_new_device, store=False)

# Adding the ScrolledText widget to the UI
text_area.pack()
# Starting a new thread to run the network sniffer
sniffer_thread = threading.Thread(target=sniff_network)
# Starting the sniffer thread
sniffer_thread.start()

# Start the Tkinter UI main loop
root.mainloop()
