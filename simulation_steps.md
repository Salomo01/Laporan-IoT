#include <WiFi.h>   // gunakan < > jika "" error

WiFiClient client;

// Pin Ultrasonik
const int trigPin = 13;
const int echoPin = 12;

// Pin LED
const int ledPin = 2;   // sesuaikan GPIO LED (misalnya GPIO2 = LED internal ESP32)

// Sound speed
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

// ThingSpeak
String thingSpeakAddress = "api.thingspeak.com";
String request_string;

const char* ssid = "ITS-WIFI-TW2";        // ganti dengan SSID WiFi
const char* password = "5027241068@student.its.ac.id"; // ganti dengan password WiFi
String writeAPIKey = "OOHZBWVVHY7WCDHV"; // ganti dengan Write API Key dari ThingSpeak

void setup() {
  Serial.begin(115200);

  // WiFi
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Pin setup
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

  // Baca durasi echo
  duration = pulseIn(echoPin, HIGH);

  // Hitung jarak
  distanceCm = duration * SOUND_SPEED / 2;
  distanceInch = distanceCm * CM_TO_INCH;

  // Print Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  // Kontrol LED
  if (distanceCm < 10.0) {
    digitalWrite(ledPin, HIGH);  // LED menyala
  } else {
    digitalWrite(ledPin, LOW);   // LED mati
  }

  // Kirim data ke ThingSpeak
  kirim_thingspeak(distanceCm, distanceInch);

  Serial.println("--------------------");

  delay(20000); // sesuai aturan ThingSpeak: minimal 15 detik per update
}

void kirim_thingspeak(float discm, float disinch) {
  if (client.connect("api.thingspeak.com", 80)) {
    request_string = "/update?";
    request_string += "api_key=";
    request_string += writeAPIKey;
    request_string += "&field1=";
    request_string += discm;
    request_string += "&field2=";
    request_string += disinch;

    // Kirim HTTP GET
    client.print(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("Data terkirim ke ThingSpeak");
  } else {
    Serial.println("Gagal connect ke ThingSpeak");
  }
}
