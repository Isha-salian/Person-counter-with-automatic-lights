#include <WiFi.h>
#include <WebServer.h>

const int pirPin1 = 14;  // PIR sensor 1 connected to digital pin 14
const int pirPin2 = 10;  // PIR sensor 2 connected to digital pin 10
const int relayPin = 5;  // Relay control pin connected to digital pin 5

const char* ssid = "iQOO Z6 5G";       // Replace with your hotspot SSID
const char* password = "ishaisha"; // Replace with your hotspot password

// Variables
int count = 0;         // Counter variable to store the number of people
int sensor1Count = 0;  // Counter variable to store the count of sensor 1

// Initialize WiFi client and web server
WiFiClient wifiClient;
WebServer server(80);

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  // Initialize the serial communication
  Serial.begin(9600);
  // Configure the PIR sensor pins as inputs
  pinMode(pirPin1, INPUT);
  pinMode(pirPin2, INPUT);
  // Configure the relay pin as an output
  pinMode(relayPin, OUTPUT);
  // Turn off the relay initially
  digitalWrite(relayPin, LOW);
  // Set up routes
  server.on("/", handleRoot);
  // Start the server
  server.begin();
  
  Serial.println("Server started");
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Read the PIR sensor values
  int sensorValue1 = digitalRead(pirPin1);
  int sensorValue2 = digitalRead(pirPin2);

  // Check if PIR sensor 1 detects motion
  if (sensorValue1 == HIGH) {
    // Increment the count when motion is detected by sensor 1
    count++;
    sensor1Count++;
    Serial.println("Motion detected by Sensor 1");
    delay(1000);  // Delay to avoid multiple counts for a single detection
  }

  // Check if PIR sensor 2 detects motion
  if (sensorValue2 == HIGH) {
    // Decrement the count when motion is detected by sensor 2
    count--;
    Serial.println("Motion detected by Sensor 2");
    delay(1000);  // Delay to avoid multiple counts for a single detection
  }

  // Print the current counts
  Serial.print("People present: ");
  Serial.println(count);
  Serial.print("Total Count: ");
  Serial.println(sensor1Count);

  // Control the relay based on the count value
  if (count > 0) {
    // Turn on the relay if count is greater than zero
    digitalWrite(relayPin, HIGH);
  } else {
    // Turn off the relay if count is zero
    digitalWrite(relayPin, LOW);
  }

  // Handle client requests
  server.handleClient();
  delay(1000);
}

void connectToWiFi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Person Counter</h1>";
  html += "<h3>Count of Sensor 1: " + String(sensor1Count) + "</h3>";
  html += "<h3>Count of Sensor 2: " + String(sensor1Count - count) + "</h3>";
  html += "<h3>Bulb Status: ";
  if (count > 0) {
    html += "On";
  } else {
    html += "Off";
  }
  html += "</h3>";
  html += "<h3>Total Count of Persons Visited: " + String(sensor1Count + count) + "</h3>";
  html += "</body></html>";

  server.send(200, "text/html", html);