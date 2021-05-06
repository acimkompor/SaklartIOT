
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const int lam = 2;
const int sak = 4;
boolean state = true;
HTTPClient http;

//const char* ssid = "Wifi virus";
//const char* pass = "sukasukague";
//const char* ssid = "realme 7i";
//const char* pass = "sumbangan";
const char* ssid = "VILLA SANTO";
const char* pass = "AA19080401";

String app_id = "877b2df5-ee74-4d6e-ba70-f89c9bf5d767";

void setup() {
  Serial.begin(115200);
  pinMode(lam, OUTPUT);
  pinMode(sak, OUTPUT);
  digitalWrite(lam, HIGH);
  digitalWrite(sak, HIGH);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menyambungkan...");
  }
  Serial.println("koneksi sukses");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    
    http.begin("http://keysya.com/tunel/onesignal/saklar.json");
    String payload;
    int httpCode = http.GET();

    if (httpCode > 0) {
      char json[500];
      payload = http.getString();
      Serial.println(httpCode);
      payload.toCharArray(json, 500);

      StaticJsonDocument<200> doc;
      deserializeJson(doc, json);
      int saklar1 = doc["sts"];

      Serial.println(payload);

      if (saklar1 == 1){
        digitalWrite(lam, LOW);
        digitalWrite(sak, LOW);
        Serial.println("Lampu hidup bos");
        pushNotifOff();
      } else {
        digitalWrite(lam, HIGH);
        digitalWrite(sak, HIGH);
        Serial.println("Ha... ha.. lampu mati");
        pushNotifOn();
      }
      Serial.println("=========================");
      Serial.println("");
      payload = "";
    }

    http.end();
  }
  delay(15000);
}

void pushNotifOff() {
  if (state) {
    Serial.println("Start");
    String respon;
    http.begin("https://keysya.com/tunel/onesignal/push.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode1 = http.POST("app_id=" + app_id);
  
    if (httpCode1 > 0) {
      
      respon = http.getString();
      Serial.println(httpCode1);
      
    }
    state = false;
//    delay(1000);
    http.end();
  }
}


void pushNotifOn() {
  if (!state) {
    Serial.println("Start");
    String respon;
    http.begin("https://keysya.com/tunel/onesignal/push_on.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode2 = http.POST("app_id=" + app_id);
    
    if (httpCode2 > 0) {
      
      respon = http.getString();
      Serial.println(httpCode2);
      
    }
    state = true;
//    delay(1000);
    http.end();
  }
}
