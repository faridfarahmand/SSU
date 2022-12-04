//-----------------------------
// Title: MQTT
//-----------------------------
// Program Details:
//-----------------------------
// Purpose: Conenct to broker.mqtt-dashboard.com, Publish and subscribe
// Dependencies: Make sure you have installed PubSubClient.h
// Compiler: PIO Version 1.72.0
// Atuhor: Originally an example called ESP8266-mqtt / slightly modified and cleaned up by Farid Farahmand
// OUTPUT: publishes 1,2,3,.... on outTopic every publishTimeInterval
// INPUT: Received value from the broker on inTopic  
// SETUP: To see the published values go to http://www.hivemq.com/demos/websocket-client/ 
//        subscribe to inTopic and outTopic. You can also create an APP using MQTT Dash
// Versions: 
//  v1: Nov-24-2022 - Cleaned up version 
//-----------------------------

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include<PubSubClient.h>

// WiFi variables
const char* ssid = "xxxxxxxxxxxxxx";  // Enter your WiFi name
const char* password = "yyyyyyyyyyy";  // Enter WiFi password

// MQTT variables
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* publishTopic = "xxxxxxxxxxxx";   // outTopic where ESP publishes
const char* subscribeTopic = "aaaaaaaaaaaaaaaaaa";  // inTopic where ESP has subscribed to
#define publishTimeInterval 10000 // in seconds 

// Definitions 
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
#define BUILTIN_LED 2 // built-in LED
char msg[MSG_BUFFER_SIZE];
int value = 0;
int ledStatus = 0;

WiFiClient espClient;
PubSubClient client(espClient); // define MQTTClient 


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
  //------------------------------------------

void callback(char* topic, byte* payload, int length) {
  Serial.print("Message arrived ["); // Received the incoming message
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);  // the received value is in (char)payload[i]
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  // add your code here

}
  //------------------------------------------

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe(subscribeTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 //------------------------------------------

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
//------------------------------------------
void loop() {

  if (!client.connected()) {
    reconnect(); // check for the latest value in inTopic 
  }
  client.loop();

// Publish to outTopic 
  unsigned long now = millis(); 
  if (now - lastMsg > publishTimeInterval) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "Number # %d", value); // prints Number # 1, Number # 2, .....
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(publishTopic, msg);
  }
}

