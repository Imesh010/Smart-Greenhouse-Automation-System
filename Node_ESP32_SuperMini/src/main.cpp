#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>

// ---------- CONFIG: change this per node ----------
#define NODE_ID 1        // 1, 2, or 3 depending on which node this is
#define DHTPIN 4          // GPIO the DHT data pin is connected to
#define DHTTYPE DHT22
// ----------------------------------------------------

// Paste the MASTER's MAC address here (printed by the master firmware on boot)
uint8_t masterAddress[] = {0x30, 0x76, 0xF5, 0xE7, 0xE6, 0x24};

DHT dht(DHTPIN, DHTTYPE);

// Data structure sent to master — must match struct on master side exactly
typedef struct SensorPacket {
  uint8_t nodeId;
  float temperature;
  float humidity;
} SensorPacket;

SensorPacket packet;
esp_now_peer_info_t peerInfo;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  memcpy(peerInfo.peer_addr, masterAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer (master)");
    return;
  }

  Serial.println("Node ready. Node ID = " + String(NODE_ID));
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  packet.nodeId = NODE_ID;
  packet.temperature = t;
  packet.humidity = h;

  esp_err_t result = esp_now_send(masterAddress, (uint8_t *) &packet, sizeof(packet));

  if (result == ESP_OK) {
    Serial.printf("Sent -> Node %d | Temp: %.1f C | Hum: %.1f%%\n", NODE_ID, t, h);
  } else {
    Serial.println("Error sending data");
  }

  delay(5000); // send every 5 seconds — adjust as needed
}
