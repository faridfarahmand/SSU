//-----------------------------
// Title: sendRequest
//-----------------------------
// Program Details:
//-----------------------------
// Purpose: Connect to the database site and insert data
// Inputs: URL+Parameters
// Outputs: GET response
// Dependencies: sendRequest.h 
// Compiler: PIO Version 1.72.0
// Atuhor: Farid Farahmand
// Versions: 
//  v1: Oct-24-2022 - Original Program
//-----------------------------

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <sendRequest.h> // required header file
//#include <WiFiClientSecure.h> // not needed

const char* ssid     = "Wifi"; // must be exactly as it apears - case sensitive
const char* password = "Ixdcd";

//https://faridfarahmand.net/PHPCRUD/firstphp_db_insert.php?nodeId=node_3&nodeTemp=39&timeReceived=2022-10-25T20:44:11.4055468
String url     = "https://faridfarahmand.net/PHPCRUD/firstphp_db_insert.php";
String data1   = "?nodeId=node_11&nodeTemp="; // currently empty
u_int8_t tempValue = 0;
String data2   = "&timeReceived=2022-10-25T20:44:11.4055468";

void setup () {
  Serial.begin(9600);
  delay(10); // just a short delay
  Serial.println("");
  
  // (A) Start connecting to the WiFI
  Serial.println("Connecting to WiFi"); 
  WiFi.begin(ssid, password); // Connection to WiFi Starts until status()=WL_CONNECTED
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("."); // waiting to get connected
  }
  // Details of the connection
   connectionDetails();
}
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient https;

    // For testing only
    tempValue = random(0, 100); // geenrate a random number

    String fullUrl = url + data1 + tempValue + data2; // preparing the full URL
    Serial.println("Requesting: --> " + fullUrl);
    if (https.begin(client, fullUrl)) { // start the connection 1=started / 0=failed

      int httpCode = https.GET(); // choose GET or POST method
      //int httpCode = https.POST(fullUrl); // need to include URL
      
      Serial.println("Response code <--: " + String(httpCode)); // print response code: e.g.,:200
      if (httpCode > 0) {
        //Serial.println(https.getString()); // this is the content of the get request received
      }
      https.end(); // end of request
    } else {
        Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  delay(5000); // delay between each REQUEST to the server

}

