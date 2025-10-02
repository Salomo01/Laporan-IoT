#include <WiFi.h>

WiFiClient client;

// Pin Ultrasonik
const int trigPin = 13;
const int echoPin = 12;

// Pin LED
const int ledPin = 2;   // GPIO2 = LED internal ESP32

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

// ThingSpeak
const char* thingSpeakAddress = "api.thingspeak.com";
const char* ssid = "ITS-WIFI-TW2";
const char* password = "itssurabaya";
String writeAPIKey = "OOHZBWVVHY7WCDHV";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Trigger Ultrasonik
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  distanceInch = distanceCm * CM_TO_INCH;

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  // Kontrol LED
  if (distanceCm < 10.0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Kirim ke ThingSpeak
  kirim_thingspeak(distanceCm, distanceInch);

  Serial.println("--------------------");
  delay(20000);
}

void kirim_thingspeak(float discm, float disinch) {
  if (client.connect(thingSpeakAddress, 80)) {
    String request = "/update?api_key=" + writeAPIKey +
                     "&field1=" + String(discm) +
                     "&field2=" + String(disinch);

    client.print("GET " + request + " HTTP/1.1\r\n");
    client.print("Host: " + String(thingSpeakAddress) + "\r\n");
    client.print("Connection: close\r\n\r\n");

    Serial.println("Data terkirim ke ThingSpeak");
  } else {
    Serial.println("Gagal connect ke ThingSpeak");
  }
}
