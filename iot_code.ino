#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ===== Wi-Fi Credentials =====
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_PASSWORD"

// ===== Firebase Credentials =====
#define API_KEY "YOUR_API_KEY"
#define DATABASE_URL "https://fire-alarm-f8d78-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Firebase Login
#define USER_EMAIL "YOUR_FIREBASE_NAME"
#define USER_PASSWORD "YOUR_FIREBASE_PASSWORD"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ===== Pins (UNCHANGED) =====
const int flamePin = 18;
const int ledFlame = 5;
const int ledNoFlame = 19;
const int buzzer = 23;
const int relayPin = 4;
const int mq2Pin = 22;

// ===== Temperature Sensor =====
#define ONE_WIRE_BUS 21
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ===== Stability counters =====
int flameCount = 0;
int gasCount = 0;

void setup() {
  Serial.begin(115200);

  // INPUT_PULLUP prevents floating values
  pinMode(flamePin, INPUT_PULLUP);
  pinMode(mq2Pin, INPUT_PULLUP);

  pinMode(ledFlame, OUTPUT);
  pinMode(ledNoFlame, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relayPin, OUTPUT);

  sensors.begin();

  // ---- Wi-Fi ----
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\n✅ Wi-Fi Connected!");

  // ---- Firebase ----
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("🔗 Connecting to Firebase...");
  unsigned long start = millis();

  while (!Firebase.ready() && millis() - start < 20000) {
    Serial.print(".");
    delay(500);
  }

  if (Firebase.ready()) {
    Serial.println("\n✅ Firebase Connected!");
  } else {
    Serial.println("\n❌ Firebase Failed!");
  }

  // 🔥 MQ2 warm-up time
  Serial.println("⏳ Warming up MQ2 sensor...");
  delay(20000);
}

void loop() {

  // ===== Raw Sensor Read =====
  int flameRaw = digitalRead(flamePin);
  int gasRaw = digitalRead(mq2Pin);

  // ===== Debug Print =====
  Serial.print("Flame Raw: ");
  Serial.print(flameRaw);
  Serial.print(" | Gas Raw: ");
  Serial.print(gasRaw);

  // ===== Corrected Logic =====
  bool flameDetected = (flameRaw == LOW);   // most flame sensors active LOW
  bool smokeDetected = (gasRaw == LOW);     // try LOW (stable after pullup)

  // ===== Stability Check (avoid false trigger) =====
  if (flameDetected) flameCount++;
  else flameCount = 0;

  if (smokeDetected) gasCount++;
  else gasCount = 0;

  // Trigger only if detected continuously
  bool stableFlame = (flameCount >= 3);
  bool stableGas = (gasCount >= 3);

  bool fireCondition = stableFlame || stableGas;

  // ===== Temperature =====
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  Serial.print(" | Temp: ");
  Serial.println(tempC);

  // ===== Output Control =====
  digitalWrite(ledFlame, fireCondition);
  digitalWrite(ledNoFlame, !fireCondition);
  digitalWrite(buzzer, fireCondition);
  digitalWrite(relayPin, !fireCondition);

  Serial.println(fireCondition ? "🔥 ALERT!" : "✅ SAFE");

  // ===== Firebase Upload =====
  if (Firebase.ready()) {
    Firebase.RTDB.setString(&fbdo, "/Alert", fireCondition ? "Alert" : "Safe");
    Firebase.RTDB.setInt(&fbdo, "/FireStatus", fireCondition ? 1 : 0);
    Firebase.RTDB.setInt(&fbdo, "/FlameStatus", stableFlame ? 1 : 0);
    Firebase.RTDB.setInt(&fbdo, "/GasLevel", stableGas ? 1 : 0);
    Firebase.RTDB.setInt(&fbdo, "/RelayStatus", fireCondition ? 1 : 0);
    Firebase.RTDB.setFloat(&fbdo, "/Temperature", tempC);

    Serial.println("📤 Firebase Updated");
  }

  delay(2000);
}