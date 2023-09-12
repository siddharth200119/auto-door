#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Stain";
const char* password = "b$VJ518175";
char* door_status = "open";

WebServer server(80);

void setup_routing() {     
  server.on("/close_door", close_door);     
  server.on("/open_door", open_door);     
  server.on("/door_status", getDoor_status);         
  //server.on("/motion_sensor_data", HTTP_POST, handlePost);           
  server.begin();    
}

//dummy functions

void close_door(){
    Serial.println("Close Door");
    server.send(200, "text/plain", "door closed");
  }

void open_door(){
    Serial.println("Open Door");
    server.send(200, "text/plain", "open closed");
  }

void getDoor_status(){
    Serial.println("Get Door Status");
    server.send(200, "text/plain", door_status);
  }

void setup() {
   Serial.begin(115200);
   delay(1000);

   WiFi.mode(WIFI_STA); //Optional
   WiFi.begin(ssid, password);
   Serial.println("\nConnecting");

   while(WiFi.status() != WL_CONNECTED){
       Serial.print(".");
       delay(100);
   }

   Serial.println("\nConnected to the WiFi network");
   Serial.print("Local ESP32 IP: ");
   Serial.println(WiFi.localIP());

   setup_routing();
}

void loop() {
  server.handleClient();  
}
