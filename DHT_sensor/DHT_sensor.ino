#include <WiFi.h>
#include <WebSocketsClient.h>

// Replace with your WiFi credentials
const char* ssid = "LUCKDOWN S03";
const char* password = "Warmbodies@2022";

WebSocketsClient webSocket;

// Function to handle WebSocket events
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  Serial.println("websocker vitra");
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket disconnected");
      break;

    case WStype_CONNECTED:
      Serial.println("WebSocket connected");
      break;

    case WStype_TEXT:
      Serial.printf("Received message: %s\n", payload);
      break;

    case WStype_ERROR:
      Serial.println("WebSocket error");
      break;

    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Connect to WebSocket server
  // webSocket.begin("192.168.1.66", 3000);  // Update with your server IP and port
  // webSocket.onEvent(webSocketEvent);
}

void loop() {
  // Handle WebSocket connection
  // webSocket.loop();
}
