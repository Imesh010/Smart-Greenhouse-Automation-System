#include <Arduino.h>
#include <WiFi.h>

void setup()
{
    Serial.begin(115200);
    delay(2000);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    Serial.println("========== ESP32-C3 ==========");

    Serial.print("MAC Address : ");
    Serial.println(WiFi.macAddress());

    Serial.print("WiFi Channel: ");
    Serial.println(WiFi.channel());

    Serial.println("==============================");
}

void loop()
{
    delay(5000);

    Serial.print("Channel: ");
    Serial.println(WiFi.channel());
}