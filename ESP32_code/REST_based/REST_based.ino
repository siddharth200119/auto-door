#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <HTTPClient.h>

const char* ssid = "Stain";
const char* password = "b$VJ518175";
const char* serverAddress = "192.168.1.6:6969/sidd_room/motion_detection";

char* door_status = "open";

unsigned const int servoPin = 3;
unsigned const int pirPin = 2;
unsigned const int timeDelay = 10000;

WebServer server(80);
Servo MyServo;

void setup_routing() {     
  server.on("/close_door", close_door);     
  server.on("/open_door", open_door);     
  server.on("/door_status", getDoor_status);         
  server.on("/motion_sensor_data", PIR_data);           
  server.begin();    
}

//dummy functions

void PIR_data(){
    if(digitalRead(pirPin) == LOW){
        server.send(200, "text/plain", "No person detected");
      }
    else{
        server.send(200, "text/plain", "person detected");
      }
  }

void close_door(){
    server.send(200, "text/plain", "door closed");
    MyServo.write(120);
    door_status = "closed";
  }

void open_door(){
    server.send(200, "text/plain", "door opened");
    MyServo.write(0);
    door_status = "open";
  }

void getDoor_status(){
    Serial.println("Get Door Status");
    server.send(200, "text/plain", door_status);
  }

void setup() {
   Serial.begin(115200);
   MyServo.attach(servoPin);
   pinMode(pirPin, INPUT);
   open_door();
   delay(1000);
   
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   Serial.println("\nConnecting");

   setup_routing();
}

void loop() {
  server.handleClient();  
  if(digitalRead(pirPin) == HIGH){
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
}
