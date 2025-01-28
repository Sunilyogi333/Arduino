#include <WiFi.h>
#include <WebSocketsClient.h>


const char* ssid = "LUCKDOWN S03";
const char* password = "Warmbodies@2022";

// WebSocket server address and port
const char* webSocketServer = "192.168.1.66";  // Replace with your server's IP address
const uint16_t port = 3000;  // WebSocket server's port (3000 in this case)

WebSocketsClient webSocket;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize WebSocket and connect to server
  webSocket.begin(webSocketServer, port);  // Correct usage

  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();  // Keep the WebSocket connection alive
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.println("Connected to WebSocket server");
      webSocket.sendTXT("Hello from ESP32!");  // Send initial message
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
