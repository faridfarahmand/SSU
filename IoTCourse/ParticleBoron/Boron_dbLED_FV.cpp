/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/


THIS CODE AND LIBRARY IS UNTESTED

 
 */

// Include Particle Device OS APIs
#include "Particle.h"
// Include HTTPClient library
#include <HttpClient.h>
// Include the DHT library
#include <Adafruit_DHT.h>

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// Define HTTP client and request object
HttpClient http;
http_request_t request;
http_response_t response;

// Replace with your server's IP address or domain
const char* serverAddress = "faridfarahmand.net";  // Change to your server's domain or IP

// DHT sensor setup
#define DHTPIN D4         // Pin where the DHT sensor is connected
#define DHTTYPE DHT11     // DHT sensor type: DHT11, DHT22, etc.
DHT dht(DHTPIN, DHTTYPE);

// Variables to send
String node_value = "node_2";  

// Pin definitions
int ledPin = D6;        // LED pin
int buttonPin = D3;     // Button pin

// Cloud-exposed variables for temperature and humidity
double currentTemperature = 0.0;
double currentHumidity = 0.0;

// Function prototypes
int setLED(String command); // Particle Function to control the LED
String formatTime();        // Helper function to format time

// Function to format time as "YYYY-MM-DD HH:MM:SS"
String formatTime() {
    time_t now = Time.now();
    String timeString = String(Time.year(now)) + "-" +
                        String(Time.month(now)) + "-" +
                        String(Time.day(now)) + "%20" +
                        String(Time.hour(now)) + ":" +
                        String(Time.minute(now)) + ":" +
                        String(Time.second(now));
    return timeString;
}

void printFreeMemory() {
    size_t freeMem = System.freeMemory();
    Serial.print("Free memory: ");
    Serial.println(freeMem);
}

// setup() runs once, when the device is first turned on
void setup() {
    Serial.begin(9600);

    // Initialize DHT sensor
    dht.begin();

    // Initialize LED pin as OUTPUT
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // Initialize button pin as INPUT with pull-up resistor
    pinMode(buttonPin, INPUT_PULLUP);

    // Expose the temperature and humidity as variables
    Particle.variable("temperature", currentTemperature);
    Particle.variable("humidity", currentHumidity);

    // Register the Particle Function for controlling the LED
    Particle.function("setLED", setLED);

    Log.info("Device ready and connected to Particle Cloud.");
}

// Particle Function to control the LED
int setLED(String command) {
    if (command == "on") {
        digitalWrite(ledPin, HIGH);
        Log.info("LED turned ON via Particle Function.");
        return 1; // Success
    } else if (command == "off") {
        digitalWrite(ledPin, LOW);
        Log.info("LED turned OFF via Particle Function.");
        return 1; // Success
    } else {
        Log.warn("Invalid command received for LED: %s", command.c_str());
        return -1; // Failure
    }
}

// loop() runs over and over again
void loop() {
    // Read data from the DHT sensor
    float temp = dht.getTempF();  // Get temperature in Fahrenheit
    float hum = dht.getHumidity();  // Get humidity percentage

    // Check if readings are valid
    if (isnan(temp) || isnan(hum)) {
        Log.warn("Failed to read from DHT sensor!");
        return;
    }

    // Update global variables
    currentTemperature = temp;
    currentHumidity = hum;

    // Check if the button is pressed (active low)
    if (digitalRead(buttonPin) == LOW) {
        Log.info("Button pressed. Sending HTTP request...");

        // Prepare the GET request to send sensor data
        String timeReceived = formatTime();
        String getRequestPath = "/Chartjs/iotlab.php?nodeId=" + node_value +
                                "&nodeTemp=" + String(currentTemperature) +
                                "&nodeHum=" + String(currentHumidity) +
                                "&timeReceived=" + timeReceived;

        request.hostname = serverAddress;
        request.port = 80;
        request.path = getRequestPath;

        Serial.println(request.hostname + request.path);
        Log.info("Sending GET request with data...");
        http.get(request, response);

        Log.info("GET Response Status: %d", response.status);
        if (response.status == 200) {
            Log.info("Data Submitted!");
        }

        // Debounce delay to prevent multiple triggers from a single press
        delay(300);
    }
}
