#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "LUCKDOWN S03";
const char* password = "Warmbodies@2022";

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password); // Start connecting to Wi-Fi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
}

void loop() {
  // Optional: Use the loop for other tasks
  Serial.println("hello world");
  delay(1000);
}
