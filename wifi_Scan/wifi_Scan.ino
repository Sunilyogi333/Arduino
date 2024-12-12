#include <WiFi.h>

void setup() {
  Serial.begin(115200); // Start Serial Monitor
  Serial.println("Initializing WiFi scan...");

  WiFi.mode(WIFI_STA); // Set Wi-Fi mode to station
  WiFi.disconnect();   // Disconnect from any previous connection
  delay(100);

  Serial.println("Starting WiFi scan...");
  int numNetworks = WiFi.scanNetworks(); // Perform the network scan

  if (numNetworks == 0) {
    Serial.println("No networks found.");
  } else {
    Serial.printf("Found %d networks:\n", numNetworks);
    for (int i = 0; i < numNetworks; i++) {
      Serial.printf("%d: %s, Signal: %d dBm, Encryption: %s\n", i + 1,
                    WiFi.SSID(i).c_str(),
                    WiFi.RSSI(i),
                    (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Secured");
    }
  }
}

void loop() {
  // Nothing to do here
}
