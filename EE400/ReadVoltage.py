# read_voltage_every_minute.py
# Reads voltage from Analog Discovery Scope Channel 1 every 60 seconds
# C:\Users\Farid\Documents\Farid\Tidal\Tidal\MyPython\'Analog Discovery 3'

from ctypes import *
import sys
import time
from datetime import datetime

# Load WaveForms library
if sys.platform.startswith("win"):
    dwf = cdll.dwf
elif sys.platform.startswith("darwin"):
    dwf = cdll.LoadLibrary("/Library/Frameworks/dwf.framework/dwf")
else:
    dwf = cdll.LoadLibrary("libdwf.so")

# Variables
hdwf = c_int()
voltage = c_double()
status = c_byte()

# Open first connected Analog Discovery
print("Opening Analog Discovery...")
dwf.FDwfDeviceOpen(c_int(-1), byref(hdwf))

if hdwf.value == 0:
    error_msg = create_string_buffer(512)
    dwf.FDwfGetLastErrorMsg(error_msg)
    print("Failed to open device:")
    print(error_msg.value.decode())
    quit()

print("Device opened successfully.")

# Configure Scope Channel 1
channel = 0   # 0 = Channel 1, 1 = Channel 2

dwf.FDwfAnalogInChannelEnableSet(hdwf, c_int(channel), c_bool(True))
dwf.FDwfAnalogInChannelRangeSet(hdwf, c_int(channel), c_double(5.0))  # +/- 5 V range
# dwf.FDwfAnalogInChannelRangeSet(hdwf, c_int(channel), c_double(25.0)) # change this if voltage is larger than 5 V
dwf.FDwfAnalogInFrequencySet(hdwf, c_double(1000.0))                 # 1 kHz sample rate

# Start analog input
dwf.FDwfAnalogInConfigure(hdwf, c_bool(False), c_bool(True))
time.sleep(1)  

print("Reading voltage every 1 minute. Press Ctrl+C to stop.\n")

try:
    while True:
        # Update measurement
        dwf.FDwfAnalogInStatus(hdwf, c_bool(False), byref(status))

        # Read one voltage sample from Channel 1
        dwf.FDwfAnalogInStatusSample(hdwf, c_int(channel), byref(voltage))

        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        print(f"{now}   Voltage = {voltage.value:.4f} V")

        time.sleep(60) # wait for 60 seconds

except KeyboardInterrupt:
    print("\nStopped by user.")

finally:
    dwf.FDwfDeviceClose(hdwf)
    print("Device closed.")
