#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "html.h"

// Access Point credentials
const char* ap_ssid = "ESP32-AirQuality";
const char* ap_password = "12345678"; // Minimum 8 characters

// Pin definitions
const int mq135Pin = 34;     // Air quality sensor (analog)
const int mq7Digital = 35;   // CO sensor (digital)
const int buzzerPin = 21;    // Buzzer for alerts

// Web server on port 80
WebServer server(80);

// Sensor readings structure
struct SensorData {
  int mq135_raw;
  float mq135_ppm;
  bool co_detected;
  String air_quality_level;
  String co_status;
  float temperature;
  float pressure;
  int altitude;
  unsigned long timestamp;
};

SensorData currentData;

// Forward declarations for functions used before their definitions
void readSensors();
void checkAlerts();
void playAlert();
void playStartupMelody();
void playHappyBirthday();

// Notes for buzzer alert
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

// Alert melody
int alertMelody[] = { NOTE_C5, NOTE_A4, NOTE_C5, NOTE_A4 };
int alertDurations[] = { 8, 8, 8, 8 };

// HTML page is now included from html.h file

void setup() {
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(mq7Digital, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Initialize sensor data with default values
  currentData.mq135_raw = 0;
  currentData.mq135_ppm = 0;
  currentData.co_detected = false;
  currentData.air_quality_level = "Initializing...";
  currentData.co_status = "Initializing...";
  currentData.temperature = 25.0;
  currentData.pressure = 1013.25;
  currentData.altitude = 50;
  currentData.timestamp = 0;
  
  // Setup Access Point
  WiFi.softAP(ap_ssid, ap_password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.println("=== ESP32 Air Quality Monitor ===");
  Serial.println("Access Point Started!");
  Serial.println("SSID: " + String(ap_ssid));
  Serial.println("Password: " + String(ap_password));
  Serial.println("IP address: " + IP.toString());
  Serial.println("Connect to this network and open: http://" + IP.toString());
  Serial.println("====================================");
  
  // Setup web server routes
  server.on("/", HTTP_GET, []() {
    Serial.println("Web page requested");
    server.send(200, "text/html", htmlPage);
  });
  
  server.on("/data", HTTP_GET, []() {
    Serial.println("Data requested");
    
    // Create JSON response
    DynamicJsonDocument doc(1024);
    
    doc["mq135_raw"] = currentData.mq135_raw;
    doc["mq135_ppm"] = currentData.mq135_ppm;
    doc["co_detected"] = currentData.co_detected;
    doc["air_quality_level"] = currentData.air_quality_level;
    doc["co_status"] = currentData.co_status;
    doc["temperature"] = currentData.temperature;
    doc["pressure"] = currentData.pressure;
    doc["altitude"] = currentData.altitude;
    doc["timestamp"] = currentData.timestamp;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonString);
  });
  
  // Add a simple ping endpoint
  server.on("/ping", HTTP_GET, []() {
    server.send(200, "text/plain", "pong");
  });
  
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Ready to monitor air quality!");
  
  // Play startup melody
  playStartupMelody();
}

void loop() {
  server.handleClient();
  
  // Read sensors every 1 second
  static unsigned long lastReading = 0;
  if (millis() - lastReading >= 1000) {
    readSensors();
    lastReading = millis();
  }
  
  // Check for alerts
  checkAlerts();
  
  delay(50); // Small delay to prevent watchdog issues
}

void readSensors() {
  // Read MQ135 (Air Quality) with multiple samples for accuracy
  int totalRaw = 0;
  for (int i = 0; i < 10; i++) {
    totalRaw += analogRead(mq135Pin);
    delay(10);
  }
  currentData.mq135_raw = totalRaw / 10;
  
  // Improved PPM conversion for MQ135
  // MQ135 is sensitive to CO2, alcohol, benzene, NH4, toluene
  float voltage = (currentData.mq135_raw / 4095.0) * 3.3;
  float resistance = (3.3 - voltage) / voltage * 10.0; // 10kΩ load resistor
  currentData.mq135_ppm = pow(10, ((log10(resistance) - 0.8) / -0.4));
  
  // Clamp PPM values to reasonable range
  if (currentData.mq135_ppm < 0) currentData.mq135_ppm = 0;
  if (currentData.mq135_ppm > 1000) currentData.mq135_ppm = 1000;
  
  // Determine air quality level based on PPM reading
  if (currentData.mq135_ppm < 50) {
    currentData.air_quality_level = "Fresh Air";
  } else if (currentData.mq135_ppm < 200) {
    currentData.air_quality_level = "Normal Indoor Air";
  } else if (currentData.mq135_ppm < 400) {
    currentData.air_quality_level = "Poor Air Quality";
  } else {
    currentData.air_quality_level = "Hazardous Air";
  }
  
  // Read MQ7 (CO Detection) with debouncing
  static int coReadings = 0;
  static int coCount = 0;
  
  if (digitalRead(mq7Digital) == HIGH) {
    coCount++;
  } else {
    coCount = 0;
  }
  
  // Require 3 consecutive HIGH readings to confirm CO detection
  if (coCount >= 3) {
    currentData.co_detected = true;
    coReadings = 0;
  } else if (coCount == 0) {
    currentData.co_detected = false;
  }
  
  currentData.co_status = currentData.co_detected ? "CO Detected!" : "Clean Air";
  
  // Simulate environmental data (replace with real sensors if available)
  // Add some realistic variation based on time
  unsigned long time = millis() / 1000;
  currentData.temperature = 25.0 + sin(time * 0.001) * 2.0 + (random(-20, 20) / 10.0);
  currentData.pressure = 1013.25 + sin(time * 0.0005) * 5.0 + (random(-50, 50) / 10.0);
  currentData.altitude = 50 + sin(time * 0.0002) * 5.0 + random(-5, 5);
  
  // Update timestamp
  currentData.timestamp = millis();
  
  // Debug output
  Serial.println("=== Sensor Readings ===");
  Serial.println("MQ135 Raw: " + String(currentData.mq135_raw));
  Serial.println("MQ135 PPM: " + String(currentData.mq135_ppm, 1));
  Serial.println("Air Quality: " + currentData.air_quality_level);
  Serial.println("CO Status: " + currentData.co_status);
  Serial.println("Temperature: " + String(currentData.temperature, 1) + "°C");
  Serial.println("Pressure: " + String(currentData.pressure, 1) + " hPa");
  Serial.println("=======================");
}

void checkAlerts() {
  static bool lastCoState = false;
  static String lastAirQuality = "";
  static bool coAlertActive = false;
  static bool airQualityAlertActive = false;
  
  // CO Alert with specific thresholds
  if (currentData.co_detected && !lastCoState) {
    Serial.println("🚨 ALERT: Carbon Monoxide Detected!");
    Serial.println("⚠️ CO levels above 70 ppm: Life-threatening situation!");
    Serial.println("🔊 Sounding continuous alarm - evacuate immediately!");
    playAlert();
    coAlertActive = true;
  } else if (!currentData.co_detected && lastCoState) {
    Serial.println("✅ CO levels returned to safe levels");
    coAlertActive = false;
  }
  
  // Continuous CO alert if still detected
  if (currentData.co_detected && coAlertActive) {
    static unsigned long lastCoAlert = 0;
    if (millis() - lastCoAlert > 5000) { // Alert every 5 seconds
      Serial.println("🚨 CONTINUOUS CO ALERT: Evacuate immediately!");
      playAlert();
      lastCoAlert = millis();
    }
  }
  
  // Air Quality Alert with specific thresholds
  if ((currentData.air_quality_level == "Poor Air Quality" || 
       currentData.air_quality_level == "Hazardous Air") && 
      currentData.air_quality_level != lastAirQuality) {
    
    if (currentData.air_quality_level == "Poor Air Quality") {
      Serial.println("⚠️ ALERT: Poor Air Quality detected!");
      Serial.println("📊 Air quality above 200 ppm - requires ventilation");
    } else if (currentData.air_quality_level == "Hazardous Air") {
      Serial.println("🚨 CRITICAL ALERT: Hazardous Air Quality!");
      Serial.println("📊 Air quality above 400 ppm - immediate action required!");
    }
    
    playAlert();
    airQualityAlertActive = true;
  } else if (currentData.air_quality_level != "Poor Air Quality" && 
             currentData.air_quality_level != "Hazardous Air" && 
             airQualityAlertActive) {
    Serial.println("✅ Air quality returned to acceptable levels");
    airQualityAlertActive = false;
  }
  
  // Ammonia (NH3) simulation alert (if PPM > 25)
  static bool nh3AlertActive = false;
  if (currentData.mq135_ppm > 25 && !nh3AlertActive) {
    Serial.println("⚠️ NH3 ALERT: Ammonia levels above 25 ppm!");
    Serial.println("👃 Distinct odor detected - improve ventilation!");
    playAlert();
    nh3AlertActive = true;
  } else if (currentData.mq135_ppm <= 25 && nh3AlertActive) {
    Serial.println("✅ NH3 levels returned to safe range");
    nh3AlertActive = false;
  }
  
  // CO2 simulation alert (if PPM > 1000)
  static bool co2AlertActive = false;
  if (currentData.mq135_ppm > 1000 && !co2AlertActive) {
    Serial.println("⚠️ CO2 ALERT: Carbon Dioxide levels above 1000 ppm!");
    Serial.println("🌬️ Poor ventilation detected - increase fresh air intake!");
    playAlert();
    co2AlertActive = true;
  } else if (currentData.mq135_ppm <= 1000 && co2AlertActive) {
    Serial.println("✅ CO2 levels returned to acceptable range");
    co2AlertActive = false;
  }
  
  lastCoState = currentData.co_detected;
  lastAirQuality = currentData.air_quality_level;
}

void playAlert() {
  int melodyLength = sizeof(alertMelody) / sizeof(alertMelody[0]);
  
  for (int i = 0; i < melodyLength; i++) {
    int noteDuration = 1000 / alertDurations[i];
    tone(buzzerPin, alertMelody[i], noteDuration);
    
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}

void playStartupMelody() {
  int startupMelody[] = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5 };
  int startupDurations[] = { 8, 8, 8, 4 };
  int melodyLength = sizeof(startupMelody) / sizeof(startupMelody[0]);
  
  for (int i = 0; i < melodyLength; i++) {
    int noteDuration = 1000 / startupDurations[i];
    tone(buzzerPin, startupMelody[i], noteDuration);
    
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}

// Function to play happy birthday (for testing/celebrations)
void playHappyBirthday() {
  int melody[] = {
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
    NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
    NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
  };
  
  int noteDurations[] = {
    4,4,2,2,2,1,
    4,4,2,2,2,1,
    4,4,2,2,2,2,2,
    4,4,2,2,2,1
  };
  
  int melodyLength = sizeof(melody) / sizeof(melody[0]);
  
  for (int thisNote = 0; thisNote < melodyLength; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}