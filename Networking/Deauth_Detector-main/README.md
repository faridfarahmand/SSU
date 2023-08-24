# Deauth_Detector


This is a script for a simple Deauthentication (Deauth) Attack Detector that uses Scapy to sniff network traffic for Deauth packets, an indicator of a Deauth attack. The script provides a graphical user interface (GUI) using Tkinter, where the state of the network is displayed. The status changes from 'Secure' to 'Deauth attack in progress' based on the detection of Deauth packets.

## Requirements
- Python 3.0+
- Scapy: A powerful Python-based network packet manipulation program.
- Tkinter: Python's standard GUI package.
- Threading: To handle multiple tasks concurrently.

## Installation
Install the necessary libraries using pip. It's recommended to use a virtual environment:

pip install scapy tkinter


## Usage

1. Run the script using python command in your terminal:

   sudo python deauth_detector.py

2. The script will open a Tkinter GUI window with a status button. 
3. If the network is secure, the status button is green and shows "Secure".
4. When a Deauth attack is detected, the status button turns red and displays "Deauth attack in progress".
5. If a Deauth attack ceases for more than 5 seconds, the button reverts to green and displays "Secure".

_Note: This script assumes that your wireless interface is 'wlan0mon'. Please replace 'wlan0mon' with the name of your wireless interface if it is different._

## Disclaimer
This script is for educational purposes only. Misuse of this information can be illegal and unethical. Always obtain proper authorization before probing or interfering with network traffic.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
MIT
