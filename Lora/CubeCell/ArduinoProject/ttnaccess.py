# --------------------------------------------------
# Title: Accessing The Things Network
# --------------------------------------------------
# Program Details:
#     The purpose of this program is to access the TTN network
#     and retrieve the data from a device.
# Inputs:
#     - TTN server information
#     - Data format
# Outputs:
#     - Data received by TTN
# Date: October 11, 2024
# File Dependencies / Libraries: None
# Compiler: Python 3
# Author: Farid Farahmand
# Versions:
#     - V1.0: Original
# Useful Links:
#     - https://nam1.cloud.thethings.network/console/applications/cubecell-bc1/api-keys
# --------------------------------------------------

import paho.mqtt.client as mqtt
import json

# TTN parameters
TTN_APP_ID = 'cubecell-bc1'  # Replace with your TTN application ID
TTN_DEVICE_ID = 'faridcubecell'  # Replace with your TTN device ID
# Must create an API - Click on API Key on the menu.
TTN_ACCESS_KEY = 'NNSXS.2TGGXSF6KRJTIGBTF2TRNWLXPR3XYV2Q5XLV3TA.OYGSBIFOTSP3O4NOSI3LZSAP4ME5XYQVVMSHCQC7M2YZITI3CN5A'  # Replace with your TTN access key
TTN_SERVER = 'nam1.cloud.thethings.network'  # TTN server (adjust for your region if necessary)
TTN_PORT = 1883  # Default MQTT port (use 8883 for TLS)

# Examples of BASE64 data transmitted: 1234567803EA1722091C
# Use this here to check:
# in https://nam1.cloud.thethings.network/console/applications/cubecell-bc1/devices/faridcubecell/payload-formatters/uplink

# MQTT topic format for TTN uplinks
TTN_TOPIC = f'v3/{TTN_APP_ID}@ttn/devices/{TTN_DEVICE_ID}/up'
global counter
counter = 0

# Callback when message is received
def on_message(client, userdata, message):
    print("Received message from TTN")
    global counter  # Declare that we are using the global variable
    try:
        # Decode the payload
        payload = json.loads(message.payload.decode('utf-8'))
        node_id = payload['uplink_message']['decoded_payload'].get('NodeId')
        temp45 = payload['uplink_message']['decoded_payload'].get('Temp45')
        temp6 = payload['uplink_message']['decoded_payload'].get('Temp6')
        temp7 = payload['uplink_message']['decoded_payload'].get('Temp7')
        temp89 = payload['uplink_message']['decoded_payload'].get('Temp89')
        print(f"Data Count: {counter}")
        print(f"NodeId: {node_id}")
        print(f"Temp45: {temp45}")
        print(f"Temp6: {temp6}")
        print(f"Temp7: {temp7}")
        print(f"Temp89: {temp89}")
        print(f"-----------")
        counter += 1
    except Exception as e:
        print(f"Error processing message: {e}")

# Callback when connected to TTN
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to TTN")
        client.subscribe(TTN_TOPIC)
    else:
        print(f"Failed to connect, return code {rc}")

# Setup MQTT client
client = mqtt.Client()

# Use correct authentication format for TTN
client.username_pw_set(f'{TTN_APP_ID}@ttn', password=TTN_ACCESS_KEY)

# Attach callback functions
client.on_connect = on_connect
client.on_message = on_message

# Optional: Enable logging for debugging
client.enable_logger()

# Connect to TTN (Consider using TLS if needed)
client.connect(TTN_SERVER, TTN_PORT, 60)

# Blocking loop to process messages
client.loop_forever()
