# Lan_Monitor


This Python script is a simple network monitor tool that uses the ICMP protocol to continuously ping a pre-defined set of hosts in your Local Area Network (LAN). 

The GUI is built with `Tkinter` and uses `subprocess` to send ICMP packets to the specified hosts. The status of each host is visually displayed, turning green when the host is reachable and red when the host is unreachable.

## Requirements

- Python 3.5+ 

## Usage

- Define the hosts you want to ping in the `hosts` dictionary. You can replace the keys of this dictionary to customize the labels in the UI.


hosts = {
    "Arduino Camera -> 192.168.0.172": "192.168.0.172",
    "Desktop -> 192.168.0.120": "192.168.0.120",
    "Raspberry Pi -> 192.168.0.121": "192.168.0.121",
    "Kali Virtual Machine -> 192.168.0.147": "192.168.0.147",
}


- Run the script from the command line:


python3 lan_monitor.py


- A new window will open with the current status of each host. The status is updated every second.

## Known Issues

- This script does not currently support hostnames, only IP addresses. 
- Depending on your system's firewall settings or security policies, you may need to run this script as an administrator to allow ICMP requests. 

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## Disclaimer

Only use on networks you are authorized to monitor

## License
[MIT](https://choosealicense.com/licenses/mit/)
