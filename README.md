# 🌱 Smart Greenhouse Automation System

An ESP32-based smart greenhouse monitoring and control system. Multiple wireless sensor nodes measure temperature and humidity across different zones of a greenhouse and transmit readings to a central master unit using the **ESP-NOW** protocol. The master aggregates this data and (in progress) will automatically control a water pump, exhaust fans, and a humidity system to keep growing conditions within target ranges — without needing a router, internet connection, or manual monitoring.

---

## 📖 Overview

Maintaining consistent temperature and humidity across a greenhouse is difficult when conditions vary by zone — areas near vents, doors, or direct sunlight often behave very differently from the center. This project solves that by placing independent sensor nodes in each zone and feeding all readings back to a single decision-making unit, which will trigger irrigation, ventilation, and humidity control automatically based on real-time, zone-specific data.

**Core goals:**
- Monitor temperature & humidity independently across 3 greenhouse zones
- Communicate wirelessly between nodes and master without WiFi/router dependency
- Automatically control watering, exhaust fans, and humidity based on sensor thresholds
- Keep the system low-cost, low-power, and easy to expand to more zones/actuators

---

##  System Architecture

```
 ┌─────────────┐     ┌─────────────┐     ┌─────────────┐
 │   Node 1     │     │   Node 2     │     │   Node 3     │
 │ ESP32 Super  │     │ ESP32 Super  │     │ ESP32 Super  │
 │ Mini + DHT22 │     │ Mini + DHT22 │     │ Mini + DHT22 │
 │  (Zone 1)    │     │  (Zone 2)    │     │  (Zone 3)    │
 └──────┬───────┘     └──────┬───────┘     └──────┬───────┘
        │                    │                     │
        └──────────────  ESP-NOW  ──────────────────┘
                             │
                             ▼
                   ┌───────────────────┐
                   │   Master ESP32     │
                   │  Aggregates data,   │
                   │  runs control logic │
                   └─────────┬──────────┘
                             │
              ┌──────────────┼──────────────┐
              ▼              ▼              ▼
        ┌───────────┐ ┌────────────┐ ┌─────────────┐
        │Water Pump │ │Exhaust Fans│ │  Humidity    │
        │  Relay    │ │   Relay    │ │  Control     │
        └───────────┘ └────────────┘ └─────────────┘
```

Each sensor node reads its local temperature/humidity and sends a small data packet directly to the master over ESP-NOW (peer-to-peer WiFi, no access point required). The master listens continuously, keeps the latest reading per zone, and will use threshold-based logic to decide when to switch on the pump, fans, or humidity control.

---

## 🔧 Hardware Used

| Component | Role | Quantity |
|---|---|---|
| ESP32 Super Mini | Sensor node (zones 1–3) | 3 |
| ESP32 Dev Board | Central master / controller | 1 |
| DHT22 Sensor | Temperature & humidity sensing | 3 |
| Relay Module | Pump / fan / humidity actuator control | 3 (planned) |
| Water Pump | Irrigation | 1 (planned) |
| Exhaust Fan(s) | Ventilation | 1–2 (planned) |
| Humidifier/Mister | Humidity control | 1 (planned) |

---

> Note: `master/` and `node/` are separate PlatformIO projects — open each individually in VS Code / PlatformIO.

---

## 🗺️ Roadmap

- [x] ESP-NOW communication between sensor nodes and master
- [x] Multi-zone temperature & humidity data collection
- [ ] Relay-based control for water pump
- [ ] Relay-based control for exhaust fans
- [ ] Humidity control (mister/humidifier)
- [ ] Threshold + hysteresis logic to prevent relay flapping
- [ ] Web dashboard / OLED display for live status
- [ ] Optional MQTT / Home Assistant integration for remote monitoring

---

