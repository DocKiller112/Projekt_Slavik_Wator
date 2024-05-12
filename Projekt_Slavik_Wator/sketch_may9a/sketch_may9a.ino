// Definice identifikátorů pro šablonu Blynk a autentizační token
#define BLYNK_TEMPLATE_ID "TMPL4TZ6sezBn"
#define BLYNK_TEMPLATE_NAME "Zavlažovací systém"
#define BLYNK_AUTH_TOKEN "2HpAJtAUnXTcqTl82Dmh3YdIG0Mm5P8O"

// Knihovny potřebné pro ovládání LCD a komunikaci přes WiFi
#include <LiquidCrystal_I2C.h>
[22:28] Tomáš SLAVÍK
// Definice identifikátorů pro šablonu Blynk a autentizační token

#define BLYNK_TEMPLATE_ID "TMPL4TZ6sezBn"

#define BLYNK_TEMPLATE_NAME "Zavlažovací systém"

#define BLYNK_AUTH_TOKEN "2HpAJtAUnXTcqTl82Dmh3YdIG0Mm5P8O"

// Knihovny potřebné pro ovládání LCD a komunikaci přes WiFi

#include <LiquidCrystal_I2C.h>

#include <Wire.h>

#include <WiFi.h>

#include <BlynkSimpleEsp32.h>

// Firebase ESP Client - knihovny pro Firebase

#include <FirebaseESP32.h>

// Definice pinů pro senzor vlhkosti a relé

#define sensor 33

#define relay 4

// Inicializace LCD displeje

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Timer pro pravidelné spouštění funkcí

BlynkTimer timer;

// Přihlašovací údaje pro WiFi síť

char ssid[] = "Uc17_IoT";

char pass[] = "teleinformatika";

// Přihlašovací údaje pro Firebase

#define FIREBASE_HOST "https://projekt-esp32-3a225-default-rtdb.europe-west1.firebasedatabase.app" // Adresa vaší Firebase databáze

#define FIREBASE_AUTH "projekt-esp32-3a225" // Tajný klíč vaší Firebase databáze

// Inicializace Firebase databáze

FirebaseData firebaseData;

void setup() {

 Serial.begin(115200);

 Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);

 lcd.begin();

 lcd.backlight();

 pinMode(relay, OUTPUT);

 digitalWrite(relay, HIGH);

 // Připojení k WiFi

 WiFi.begin(ssid, pass);

 while (WiFi.status() != WL_CONNECTED) {

   delay(500);

   Serial.print(".");

 }

 Serial.println("");

 Serial.print("Připojeno k WiFi: ");

 Serial.println(WiFi.localIP());

 // Připojení k Firebase

 Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

 Firebase.reconnectWiFi(true);

}

void soilMoisture() {

 int value = analogRead(sensor);

 value = map(value, 0, 4095, 0, 100);

 value = (value - 100) * -1;

 Blynk.virtualWrite(V0, value);

 Serial.println(value);

 lcd.setCursor(0, 0);

 lcd.print("Moisture :");

 lcd.print(value);

 lcd.print(" ");

 // Odeslání hodnoty vlhkosti do Firebase

 if (Firebase.ready()) {

   Firebase.setInt(firebaseData, "/soilMoisture", value);

 }

}

BLYNK_WRITE(V1) {

 bool Relay = param.asInt();

 if (Relay == 1) {

   digitalWrite(relay, LOW);

   lcd.setCursor(0, 1);

   lcd.print("Motor is ON ");

   // Odeslání stavu motoru do Firebase

   Firebase.setBool(firebaseData, "/motorStatus", true);

 } else {

   digitalWrite(relay, HIGH);

   lcd.setCursor(0, 1);

   lcd.print("Motor is OFF");

   // Odeslání stavu motoru do Firebase

   Firebase.setBool(firebaseData, "/motorStatus", false);

 }

}

void loop() {

 soilMoisture();

 Blynk.run();

 timer.run(); // Spuštění Blynk timeru

}