
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_TEMPLATE_ID "TMPL3fPEk8irW"
#define BLYNK_DEVICE_NAME "EarthquakeAlert"

// Blynk Authentication Token
#define BLYNK_AUTH "JRPi-rOLqwMYODjZ6wWfIzEKZS3SrnnA"

// WiFi Credentials
#define WIFI_SSID "Oppo"
#define WIFI_PASS "Gayu@1006"

// Components
#define SW420_PIN D5  // Vibration Sensor
#define BUZZER_PIN D6  // Buzzer
#define LED_PIN D7  // LED
#define SOLENOID_PIN D8  // Solenoid Valve

// Initialize LCD (16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables
int vibration = 0;

void setup() {
    Serial.begin(115200);
    Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASS);

    // LCD Setup
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Earthquake Alert");

    // Pin Modes
    pinMode(SW420_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(SOLENOID_PIN, OUTPUT);

    // Default States
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(SOLENOID_PIN, LOW);
}

// Function to Handle Earthquake Alert
void earthquakeAlert() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Vibration Detected!");

    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(SOLENOID_PIN, HIGH);  // Close gas valve

    Blynk.notify("Earthquake Alert! Take safety precautions.");
    delay(5000);

    // Reset System
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(SOLENOID_PIN, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Monitoring...");
}

// Blynk Control for Solenoid Valve
BLYNK_WRITE(V1) {
    int valveState = param.asInt();
    digitalWrite(SOLENOID_PIN, valveState);
}

// Blynk Control for Buzzer
BLYNK_WRITE(V2) {
    int buzzerState = param.asInt();
    digitalWrite(BUZZER_PIN, buzzerState);
}

void loop() {
    Blynk.run();

    vibration = digitalRead(SW420_PIN);
    if (vibration == HIGH) {
        earthquakeAlert();
    }
}
