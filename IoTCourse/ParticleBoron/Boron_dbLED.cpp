/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
// Include HTTPClient library
#include <HttpClient.h>

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);


// Define HTTP client and request object
HttpClient http;
http_request_t request;
http_response_t response;

// Replace with your server's IP address or domain
const char* serverAddress = "faridfarahmand.net";  // Change to your server's domain or IP

// Variables to send (you can collect real sensor data here)
String node_value = "node_2";  // Replace with your actual node value
int temperature = 25;  // Replace with actual temperature reading
int humidity = 60;  // Replace with actual humidity reading

// Pin definition for the LED
int ledPin = D6;  // Define the pin where your LED is connected
int buttonPin = D3;     // Button pin


// Function to format time
// Function to format time as "YYYY-MM-DD HH:MM:SS"
String formatTime() {
    // Get the current time as a time_t
    time_t now = Time.now();

    // Format the time into a string
    String timeString = String(Time.year(now)) + "-" +
                        String(Time.month(now)) + "-" +
                        String(Time.day(now)) + "%20" +
                        String(Time.hour(now)) + ":" +
                        String(Time.minute(now)) + ":" +
                        String(Time.second(now));

    return timeString;  // Return formatted time
}

void printFreeMemory() {
    size_t freeMem = System.freeMemory();
    Serial.print("Free memory: ");
    Serial.println(freeMem);
}



// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here
      // Start Serial for debugging
    Serial.begin(9600);

    // Initialize LED pin as OUTPUT
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);  // Ensure the LED is off at startup
    // Initialize button pin as INPUT with pull-up resistor
    pinMode(buttonPin, INPUT_PULLUP);

    
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

  // Example: Publish event to cloud every 10 seconds. Uncomment the next 3 lines to try it!
  // Log.info("Sending Hello World to the cloud!");
  // Particle.publish("Hello world!");
  // delay( 10 * 1000 ); // milliseconds and blocking - see docs for more info!


    // Check if the button is pressed (active low)
    if (digitalRead(buttonPin) == LOW) {
        Log.info("Button pressed. Sending HTTP request...");


    // Get the current time (you can replace this with your own time logic)
    // Get the current time in the desired format
    String timeReceived = formatTime();

    // Prepare the GET request with query parameters
    String getRequestPath = "/Chartjs/iotlab.php?nodeId=" + node_value +
                            "&nodeTemp=" + String(temperature) +
                            "&nodeHum=" + String(humidity) +
                            "&timeReceived=" + timeReceived;

    // Set the GET request details
    request.hostname = serverAddress;
    request.port = 80;  // Default HTTP port
    //request.path = getRequestPath;  // Path with query parameters for your PHP script

    Serial.println(request.hostname+request.path);

    // Send the GET request
    Serial.println("Sending GET dB request with data...");
    http.get(request, response);

    // Debugging output for the GET request
    Serial.print("GET dB Response Status: ");
    Serial.println(response.status);
    if (response.status == 200) {
        Serial.println("Data Submitted!");
    }
    
    //Serial.print("GET dB Response Body: ");
    //Serial.println(response.body);

    // Wait 5 seconds before sending another GET request to check the status file
    delay(10000);

    // Prepare the GET request to check the .txt file
    request.path = "/LED/tp.txt";  // Path to the .txt file on the server

    Serial.println(request.hostname+request.path);


    // Send the GET request to check the status of the .txt file
    Serial.println("Sending LED GET request to check status...");
    http.get(request, response);

    // Debugging output for the status check GET request
    Serial.print("GET LED Response Status: ");
    Serial.println(response.status);

    Serial.print("GET LED Response Body: ");
    Serial.println(response.body);
    String led = response.body;

   // Check the response to see if it says "on" or "off"
    if (response.body.indexOf("on") != -1) {
        // If the response contains "on", turn the LED on
        digitalWrite(ledPin, HIGH);
        Serial.println("LED turned ON.");
    } else if (response.body.indexOf("off") != -1) {
        // If the response contains "off", turn the LED off
        digitalWrite(ledPin, LOW);
        Serial.println("LED turned OFF.");
    } else {
        Serial.println("Unknown response, LED state unchanged.");
    }

    // Wait another 5 seconds before sending the next GET request with data
    //delay(10000);

}

}