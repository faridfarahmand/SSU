# New_Device_Detector
This Python script uses Scapy to sniff for Wi-Fi probe requests on a given network interface, and displays any new devices it detects in a Tkinter window. It identifies devices by their MAC addresses, and also displays the SSID from the probe requests when available.

## Prerequisites
-Python 3 

-Scapy

-Tkinter

## Usage
To start, ensure your wireless adapter is in monitor mode.

To run the script, open a terminal window and type:

python3 new_device_detection.py

Please replace "new_device_detection.py" with the actual path to the Python script if it's not in the current directory.

When running the script, a new Tkinter window titled "New Device Detection" will appear. As the script detects new devices, it will add a line in the window with the MAC address and SSID of each device. If the SSID cannot be decoded or is not included in the probe request, it will display "Cannot decode SSID".

Please replace "wlan0mon" in the sniff_network function with the name of your network interface that's in monitor mode.

## Disclaimer
Also, ensure you have the necessary permissions to run the script, as network sniffing typically requires root privileges.

Remember that sniffing network traffic should only be done for legitimate purposes and in a manner that's consistent with all applicable laws and regulations. Unauthorized network sniffing is illegal and unethical. Use this script responsibly.

## Known Issues
Not all SSIDs are detected
