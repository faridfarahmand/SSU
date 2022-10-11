#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>

const char* ssid     = "xxx"; // must be exactly as it apears - case sensitive
const char* password = "xxx";
const char* remote_host = "godaddy.com";
void setup() {
  
  Serial.begin(9600);
  delay(10);
  Serial.println("");
  Serial.println("");
  Serial.println("Connecting to WiFi");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    
    delay(100);
    Serial.print(".");
    
  }
  Serial.println("");
  Serial.print("Local IP Address: ");  
  Serial.print(WiFi.localIP());
  Serial.println("");
  Serial.print("Pinging: ");
  Serial.print(remote_host);
  Serial.println("");
  Serial.println("");

// WiFi Library: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html 
// In this program we have Class called Ping.
  Serial.println(WiFi.getHostname());// ESP-2E7168
  Serial.println(WiFi.RSSI());// -59 for example
  Serial.println(WiFi.localIP()); //8266 current IP address
  Serial.println(WiFi.SSID()); // SSID available - a number

  Serial.println(Ping.averageTime());// some number 
  Serial.println(Ping.ping(remote_host));// 1=connected; 0 not connected
}
void loop() { 
  
    if(Ping.ping(remote_host)) {
    Serial.println("Online");
    delay(1000);
    
  } else {
    
    Serial.println("Offline");
    delay(1000); 
  }
}
