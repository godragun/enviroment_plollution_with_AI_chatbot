# Environment Pollution Monitoring with AI Chatbot

A comprehensive IoT system for **real-time air quality and gas monitoring**, equipped with an interactive AI chatbot for instant guidance and safety information. Designed for smart environments, this project combines hardware sensor integration with a modern web dashboard and an AI assistant for user support.

---

## 🚦 Hardware Pin Configuration

| Sensor/Module    | Description                    | ESP32 Pin  |
|------------------|-------------------------------|------------|
| MQ135            | Air Quality Sensor (Analog)    | 34         |
| MQ7 (Digital)    | Carbon Monoxide Sensor         | 35         |
| Buzzer           | Alerts/Notifications           | 21         |

---

## 💾 Prerequisites & Downloads

- **Arduino IDE** (or PlatformIO)
- **ESP32 Board Package**
    - Install via Arduino IDE: *Tools > Board > Boards Manager > ESP32*
- **Libraries**:
    - `WiFi.h` (built-in)
    - `PubSubClient.h` (for MQTT, if used)
    - `Adafruit_Sensor.h`, `Adafruit_GFX.h`, `Adafruit_SSD1306.h` (for display, optional)
    - Any WebSocket/MQTT/HTTP library for real-time dashboard
- **Frontend**: Modern browser for web dashboard

---

## 🧠 AI Chatbot Features

- **Comprehensive Knowledge Base**:
    - **Carbon Monoxide (CO):** Safety limits, symptoms, emergency actions
    - **Ammonia (NH₃):** Exposure limits, health effects, prevention
    - **Carbon Dioxide (CO₂):** Safe levels, environmental impact
    - **Air Quality Index (AQI):** Guidelines & health advice
    - **General Safety:** Detection, prevention, monitoring tips

- **User Experience**:
    - **Floating Chatbot Button**: Always available, interactive Q&A
    - **Dark/Light Mode**: Seamless theme switching

---

## 🚨 Alert Rules

| Pollutant      | Threshold                   | Response                                |
|----------------|-----------------------------|-----------------------------------------|
| **CO**         | >70 ppm                     | ⚠️ Continuous buzzer alarm (life-threatening) |
| **NH₃**        | >25 ppm                     | ⚠️ Immediate alert (OSHA limit)         |
| **CO₂**        | >1000 ppm                   | ⚠️ Ventilation warning                  |
| **Air Quality**| Progressive thresholds      | ⚠️ Warnings for poor/hazardous AQI      |

---

## 📊 Real-Time Dashboard Features

- **Live Sensor Data**: Real-time charts and logs
- **Progressive Alerts**: Visual and audible warnings
- **Responsive Design**: Mobile & desktop support
- **Dark/Light Mode**: Fully functional theme switching

---

## 🚀 Getting Started

1. **Setup Hardware**: Connect MQ135 (A0 to 34), MQ7 Digital (DO to 35), and Buzzer (to 21) on the ESP32.
2. **Upload Code**: Flash firmware via Arduino IDE/PlatformIO.
3. **Run Dashboard**: Open the web interface for live data and chatbot.
4. **Ask the AI**: Use the floating button to ask about pollutants, health tips, or get emergency actions.

---

## 🌱 Example Use Cases

- **Home Safety**: Warns about dangerous CO or NH₃ levels with instant chatbot help.
- **Educational**: Explains AQI and gas health impacts interactively.
- **Smart Building**: Monitors air quality and suggests ventilation actions.

---

## 🤖 AI Chatbot: Example Questions

- "What are the symptoms of CO poisoning?"
- "How can I reduce ammonia exposure?"
- "What is a safe CO₂ level indoors?"
- "What does AQI 150 mean?"

---

## 🛡️ General Safety Tips

- Regularly calibrate sensors.
- Ensure proper ventilation.
- Respond immediately to alerts.
- Consult the AI chatbot for emergency guidance.

---

## 📬 Contributions

Pull requests and feature suggestions welcome! See [issues](https://github.com/godragun/enviroment_plollution_with_AI_chatbot/issues).

---

## 📜 License

[MIT](LICENSE)

---
