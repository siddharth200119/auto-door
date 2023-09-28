#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

//pins
const int PIR_pin = 2;
const int servo_pin = 3;
int sensor_output;

// WiFi
const char* network_ssid = "Stain";
const char* network_password = "b$VJ518175";

//MQTT
const char* mqtt_server = "192.168.1.6";
const char* PIR_data = "home/sidd_room/doorside_esp32/motion_detection";
const char* swtich_control = "Home/sidd_room/doorside_esp32/switches";
const char* mqtt_username = "raspberryPI";
const char* mqtt_password = "M.A.R.I.A.";
const char* clientID = "sidd_room_doorside_esp32";

//setting up clients and IO devices
WiFiClient esp32C3Client;
PubSubClient client(mqtt_server, 1883, esp32C3Client);
Servo MyServo;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(network_ssid);

  WiFi.begin(network_ssid, network_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageContent;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageContent += (char)message[i];
  }
  Serial.println();

  if (String(topic) == swtich_control) {
    Serial.print("Changing output to ");
    if(messageContent == "on"){
      Serial.println("on");
      MyServo.write(0);
    }
    else if(messageContent == "off"){
      Serial.println("off");
      MyServo.write(120);
    }
  }
}

void setup() {
  Serial.begin(115200);
  MyServo.attach(servo_pin);
  pinMode(PIR_pin, INPUT);
  sensor_output = digitalRead(PIR_pin);
  delay(1000);
  setup_wifi();
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed…");
  }
  client.setCallback(callback);
}

void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(sensor_output == HIGH){
      client.publish(PIR_data, "person detected");
    }
}
