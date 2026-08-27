#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Must match the struct on the node side exactly
typedef struct SensorPacket {
  uint8_t nodeId;
  float temperature;
  float humidity;
} SensorPacket;

SensorPacket incomingPacket;

// Store latest reading per zone (index 0 = node 1, etc.)
float zoneTemp[3]  = {0, 0, 0};
float zoneHum[3]   = {0, 0, 0};
unsigned long lastSeen[3] = {0, 0, 0};

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  (void)mac;
  memcpy(&incomingPacket, incomingData, sizeof(incomingPacket));

  int idx = incomingPacket.nodeId - 1;
  if (idx < 0 || idx > 2) return; // ignore invalid node IDs

  zoneTemp[idx] = incomingPacket.temperature;
  zoneHum[idx]  = incomingPacket.humidity;
  lastSeen[idx] = millis();

  Serial.printf("Received <- Node %d | Temp: %.1f C | Hum: %.1f%%\n",
                incomingPacket.nodeId, incomingPacket.temperature, incomingPacket.humidity);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);

  Serial.print("Master MAC (paste this into each node's masterAddress[]): ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onDataRecv);
  Serial.println("Master ready, listening for nodes...");
}

void loop() {
  // Basic "node offline" check every 15s (useful once all 3 nodes are running)
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 15000) {
    lastCheck = millis();
    for (int i = 0; i < 3; i++) {
      if (lastSeen[i] == 0) continue;
      if (millis() - lastSeen[i] > 20000) {
        Serial.printf("WARNING: Node %d hasn't reported in >20s\n", i + 1);
      }
    }
  }
}
