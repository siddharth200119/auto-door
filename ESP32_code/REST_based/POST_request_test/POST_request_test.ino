#include <WiFi.h>
#include <ESP32Servo.h>
#include <HTTPClient.h>

const char* ssid = "Stain";
const char* password = "b$VJ518175";
const char* serverAddress = "192.168.1.6:6969/sidd_room/motion_detection";
unsigned const int timeDelay = 10000;

void setup() {
   Serial.begin(115200);
   delay(1000);
   
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   Serial.println("\nConnecting");

   if(WiFi.status()== WL_CONNECTED){
      Serial.println("\nConnected");
    }
}

void loop() {
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverAddress);
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"location\":\"sidd_room\",\"sensor_position\":\"door\",\"person_detected\":\"true\"}");
      http.end();
      delay(timeDelay);
    }
    else{
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      Serial.println("\nConnecting");
    }
}
