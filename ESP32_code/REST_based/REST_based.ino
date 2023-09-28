#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* ssid = "Stain";
const char* password = "b$VJ518175";

char* door_status = "open";
int sensor_output;

static const int servoPin = 3;
static const int pirPin = 2;

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
    sensor_output = digitalRead(pirPin);
    if(sensor_output == LOW){
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
   delay(1000);

   WiFi.mode(WIFI_STA); //Optional
   WiFi.begin(ssid, password);
   Serial.println("\nConnecting");

   while(WiFi.status() != WL_CONNECTED){
       delay(100);
   }

   setup_routing();
}

void loop() {
  server.handleClient();  
}
