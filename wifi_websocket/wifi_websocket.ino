#include <WiFi.h>
#include <WebSocketsClient.h>
#include "DHT.h"

// WiFi and WebSocket Server Configuration
const char* ssid = "LUCKDOWN S03";
const char* password = "Warmbodies@2022";
const char* webSocketServer = "192.168.1.66";  // Replace with your server's IP
const uint16_t port = 3000;

// DHT Sensor Configuration
#define DHTPIN 4  // Pin connected to the DHT sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

WebSocketsClient webSocket;
unsigned long lastSendTime = 0;  // Timestamp to control sending intervals
const unsigned long sendInterval = 5000;  // Send data every 5 seconds

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
      break;

    default:
      break;
  }
}
