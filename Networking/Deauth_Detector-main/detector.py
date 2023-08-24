# Import the Scapy, Tkinter, threading and time libraries
from scapy.all import sniff, Dot11, Dot11Deauth
import tkinter as tk
import threading
import time

# Create the UI
root = tk.Tk()
root.title("Deauth Attack Detector")
button = tk.Button(root, text="Secure", bg="green")
button.pack()

# Initialize a variable to store the time of the last deauth packet
last_deauth_time = None

# Variable to control sniffing thread
keep_sniffing = True

# Define a function to detect deauth attacks
def deauth_detect(packet):
    global last_deauth_time
    # If packet is 802.11 Deauthentication Packet
    if packet.haslayer(Dot11Deauth):
        last_deauth_time = time.time()
        button.config(bg="red", text="Deauth attack in progress")

# Function to stop sniffing
def stop_sniff(packet):
    return not keep_sniffing

# Function to regularly check the time of the last deauth packet
def check_deauth_time():
    global last_deauth_time
    # If more than 5 seconds have passed since the last deauth packet, switch the button back to green
    if last_deauth_time is not None and time.time() - last_deauth_time >= 5:
        button.config(bg="green", text="Secure")
        last_deauth_time = None
    # Schedule this function to run again after 1 second
    root.after(1000, check_deauth_time)

# Function to start sniffing in a separate thread
def start_sniffing():
    # Specify the interface to listen on
    interface = "wlan0mon"
    # Run the deauth_detect function
    sniff(iface=interface, prn=deauth_detect, store=0, stop_filter=stop_sniff)

# If this is the main program, run the deauth_detect function
if __name__ == "__main__":
    # Start sniffing in a separate thread
    sniff_thread = threading.Thread(target=start_sniffing)
    sniff_thread.daemon = True
    sniff_thread.start()

    # Start the regular check for the time of the last deauth packet
    root.after(1000, check_deauth_time)

    try:
        # Start the Tkinter event loop
        root.mainloop()
    finally:
        # When the GUI is closed, stop sniffing
        keep_sniffing = False
        sniff_thread.join()

