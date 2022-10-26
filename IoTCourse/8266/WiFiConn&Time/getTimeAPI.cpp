//-----------------------------
// Title: GetTimeAPI
//-----------------------------
// Program Details:
//-----------------------------
// Purpose: Connect to the timeapi.io site and GET time
// Inputs: None
// Outputs: Date and Time 
// Dependencies: No external file is required
// Compiler: PIO Version 1.72.0
// Atuhor: Farid Farahmand
// Versions: 
//  v1: Oct-24-2022 - Original Program
//-----------------------------

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>  // must be installed 

const char* ssid     = "Wifi khoneh"; // must be exactly as it apears - case sensitive
const char* password = "Ivonne!234";

// Exampe: https://timeapi.io/api/Time/current/zone?timeZone=America/Los_Angeles
String url     = "https://timeapi.io/api/Time/current/zone?timeZone=America/Los_Angeles";

DynamicJsonDocument doc(1024);

void setup () {
  Serial.begin(9600);
  delay(10); // just a short delay
  Serial.println("");
  
  // (A) Start connecting to teh WiFI
  Serial.println("Connecting to WiFi"); 
  WiFi.begin(ssid, password); // Connection to WiFi Starts until status()=WL_CONNECTED
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("."); // waiting to get connected
  }
}
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient https;

    String fullUrl = url; // preparing the full URL
    Serial.println("Requesting: --> " + fullUrl);
    if (https.begin(client, fullUrl)) { // start the connection 1=started / 0=failed

      int httpCode = https.GET(); // choose GET or POST method
      //int httpCode = https.POST(fullUrl); // need to include URL
      
      Serial.println("Response code <--: " + String(httpCode)); // print response code: e.g.,:200
      if (httpCode > 0) {
        Serial.println(https.getString()); // this is the content of the get request received
        deserializeJson(doc,https.getString()); // deserialize the JSON file
        /*--- Sample Response ----
        {"year":2022,"month":10,"day":25,"hour":20,"minute":44,"seconds":11,"milliSeconds":405,
        "dateTime":"2022-10-25T20:44:11.4055468","date":"10/25/2022","time":"20:44",
        "timeZone":"America/Los_Angeles","dayOfWeek":"Tuesday","dstActive":true}
        ------------------------ */
        deserializeJson(doc,https.getString()); // deserialize the JSON format into keys and values
        String mytime = doc["dateTime"]; // get the value associated with the dataTime key
        Serial.println(mytime); // soomething like 2022-10-25T21:03:44.1790514
      }
      https.end(); // end of request
    } else {
        Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  delay(5000); // delay between each REQUEST to the server

}