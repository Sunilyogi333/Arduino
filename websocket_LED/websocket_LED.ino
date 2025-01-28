#include <WiFi.h>
#include <WebSocketsClient.h>
#include "DHT.h"

// WiFi and WebSocket Server Configuration
const char* ssid = "LUCKDOWN S03";
const char* password = "Warmbodies@2022";
const char* webSocketServer = "https://backend-o7xm.onrender.com";  // Replace with your server's IP
const uint16_t port = 443;

// DHT Sensor Configuration
#define DHTPIN 4  // Pin connected to the DHT sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


WebSocketsClient webSocket;
unsigned long lastSendTime = 0;  // Timestamp to control sending intervals
const unsigned long sendInterval = 5000;  // Send data every 5 seconds

#define GREEN_BULB_PIN 22
#define GREEN_BULB2_PIN 19

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize DHT Sensor
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize WebSocket Client
  webSocket.begin(webSocketServer, port);
  webSocket.onEvent(webSocketEvent);

    // Configure the GPIO pin as output
  pinMode(GREEN_BULB_PIN, OUTPUT);
  digitalWrite(GREEN_BULB_PIN, LOW); // Start with the bulb turned off

  pinMode(GREEN_BULB2_PIN, OUTPUT);
  digitalWrite(GREEN_BULB2_PIN, LOW); // Start with the bulb turned off
}

void loop() {
  webSocket.loop();  // Keep WebSocket connection alive

  // Send data at intervals
  if (millis() - lastSendTime >= sendInterval) {
    lastSendTime = millis();  // Update last send time
    sendSensorData();
  }
}

void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Validate sensor readings
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Log data to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Create JSON payload
  String jsonPayload = String("{\"temperature\":") + temperature + ",\"humidity\":" + humidity + "}";

  // Send JSON data via WebSocket
  webSocket.sendTXT(jsonPayload);
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.println("Connected to WebSocket server");
      webSocket.sendTXT("Hello from ESP32!");
      break;

    case WStype_DISCONNECTED:
      Serial.println("Disconnected from WebSocket server");
      break;

    case WStype_TEXT:
      Serial.printf("Received message: %s\n", payload);
        // Parse the received message
      if (strcmp((char*)payload, "turn_off") == 0) {
        Serial.println("Turning off the green bulb...");
        digitalWrite(GREEN_BULB_PIN, LOW); // Turn off the bulb
        digitalWrite(GREEN_BULB2_PIN, HIGH); // Turn off the bulb
        
      } else if (strcmp((char*)payload, "turn_on") == 0) {
        Serial.println("Turning on the green bulb...");
        digitalWrite(GREEN_BULB_PIN, HIGH); // Turn on the bulb
        digitalWrite(GREEN_BULB2_PIN, LOW); // Turn off the bulb
      }
      break;

    default:
      break;
  }
}