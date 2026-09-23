#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "pins.h"




WiFiClient wifiClient;
PubSubClient mqtt_client(wifiClient);


const char* ssid = "DeviceNet";
const char* password = "HappyPuppy1105";

const char *mqtt_broker = "192.168.8.201";
const char *mqtt_username = "37rwMQTT";
const char *mqtt_password = "D\"n1elw3lch";
const char *mqtt_topic  = "cookerhood";
int        mqtt_port     = 1883;
long buttontime = 0;


#define MSG_BUFFER_SIZE  (250)
char msg[MSG_BUFFER_SIZE];

// register if the lght button  pushed:
bool lightpressed = false;
// register if the onoff button pushed:
bool onoffpressed = false;
// register if the lght button  pushed:
bool speedpressed = false;
// register if the lght button  pushed:
bool timerpressed = false;

// MQTT HANDLER
void callback(char* topic, byte* payload, unsigned int length);

// RELAY SET
void setRelay(int relay);

// WIFI CONNECTING
void connect();
void connectToMQTTBroker();



void setup() {
  
  Serial.begin(9600);
  


  // put your setup code here, to run once:
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(Relay3,OUTPUT);
  pinMode(Relay4,OUTPUT);

  pinMode(light_button, INPUT_PULLUP);
  pinMode(onoff_button, INPUT_PULLUP);
  pinMode(speed_button, INPUT_PULLUP);
  pinMode(timer_button, INPUT_PULLUP);

  setRelay(0);
  
  connect();
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(callback);
  connectToMQTTBroker();

  
  mqtt_client.publish("cookerhood/state/mode", "OFF", true);
  mqtt_client.publish("cookerhood/state/speed", "1", true);

}



void loop() {
  // put your main code here, to run repeatedly:

  if (!mqtt_client.connected()) {
    Serial.print(mqtt_client.state());
    connectToMQTTBroker();
  }
  mqtt_client.loop();
  
  if (lightpressed == false && digitalRead(light_button) == 0) {
    lightpressed=true;
    buttontime = millis();
   // mqtt_client.publish("cookerhood/light_button", "ON", false);
  }
  if (lightpressed == true && digitalRead(light_button) == 1) {
    lightpressed=false;
    if (millis()-buttontime > 250) mqtt_client.publish("cookerhood/light_button", "ON", false);
  }

  if (speedpressed == false && digitalRead(speed_button) == 0) {
    speedpressed=true;
    buttontime = millis();
    // mqtt_client.publish("cookerhood/speed_button", "ON", false);
  }
  if (speedpressed == true && digitalRead(speed_button) == 1) {
    speedpressed=false;
    if (millis()-buttontime > 250) mqtt_client.publish("cookerhood/speed_button", "ON", false);
  }

  if (onoffpressed == false && digitalRead(onoff_button) == 0) {
    onoffpressed=true;
    buttontime = millis();
    // mqtt_client.publish("cookerhood/onoff_button", "ON", false);
  }
  if (onoffpressed == true && digitalRead(onoff_button) == 1) {
    onoffpressed=false;
    if (millis()-buttontime > 250) mqtt_client.publish("cookerhood/onoff_button", "ON", false);
  }

  if (timerpressed == false && digitalRead(timer_button) == 0) {
    timerpressed=true;
    buttontime = millis();
    // mqtt_client.publish("cookerhood/timer_button", "ON", false);
  }
  if (timerpressed == true && digitalRead(timer_button) == 1) {
    timerpressed=false;
    if (millis()-buttontime > 250) mqtt_client.publish("cookerhood/timer_button", "ON", false);
  }


}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  int speed = 0;
  char msg[5];

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);
  // Test if parsing succeeds
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  if (strcmp(topic,"cookerhood/cmnd/mode")==0 ) {
    if (doc["mode"]=="ON") {
      setRelay(1);
      mqtt_client.publish("cookerhood/state/mode", "ON", true);
      mqtt_client.publish("cookerhood/state/speed", "1", true);
    }
    if (doc["mode"]=="OFF") {
      setRelay(0);
      mqtt_client.publish("cookerhood/state/mode", "OFF", true);
      mqtt_client.publish("cookerhood/state/speed", "1", true);
    }
  }
  
  if (strcmp(topic,"cookerhood/cmnd/speed")==0 ) {
        speed = doc["speed"];
        setRelay(speed);

        if (speed==0)  mqtt_client.publish("cookerhood/state/mode", "OFF", true);
        else mqtt_client.publish("cookerhood/state/mode", "ON", true);
        snprintf (msg, 5, "%d", speed);
        mqtt_client.publish("cookerhood/state/speed", msg, true);
      }

}

void setRelay(int relay) {

  switch (relay) 
  {
    case 1: 
      // digitalWrite(Relay1,HIGH);
      digitalWrite(Relay2,HIGH);
      digitalWrite(Relay3,HIGH);
      digitalWrite(Relay4,HIGH);
      delay(100);
      digitalWrite(Relay1,LOW);
      Serial.println("Relay 1 ON");
      break;
    
    case 2:
      digitalWrite(Relay1,HIGH);
      // digitalWrite(Relay2,HIGH);
      digitalWrite(Relay3,HIGH);
      digitalWrite(Relay4,HIGH);
      delay(100);
      digitalWrite(Relay2,LOW);
      Serial.println("Relay 2 ON");
      break;
    
    case 3:
      digitalWrite(Relay1,HIGH);
      digitalWrite(Relay2,HIGH);
      // digitalWrite(Relay3,HIGH);
      digitalWrite(Relay4,HIGH);
      delay(100);
      digitalWrite(Relay3,LOW);
      Serial.println("Relay 3 ON");
      break;
    
    case 4:
      digitalWrite(Relay1,HIGH);
      digitalWrite(Relay2,HIGH);
      digitalWrite(Relay3,HIGH);
      // digitalWrite(Relay4,HIGH);
      delay(100);
      digitalWrite(Relay4,LOW);
      Serial.println("Relay 5 ON");
      break;
    
    case 0:
      digitalWrite(Relay1,HIGH);
      digitalWrite(Relay2,HIGH);
      digitalWrite(Relay3,HIGH);
      digitalWrite(Relay4,HIGH);
      Serial.println("All OFF");
      break;

    default:
      digitalWrite(Relay1,HIGH);
      digitalWrite(Relay2,HIGH);
      digitalWrite(Relay3,HIGH);
      digitalWrite(Relay4,HIGH);
      Serial.println("All OFF");
      break;

  }
}


void connect() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }

  Serial.println("success!");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());
}


void connectToMQTTBroker() {
  while (!mqtt_client.connected()) {
      String client_id = "esp8266-client-" + String(WiFi.macAddress());
      Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
      if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Connected to MQTT broker");

          mqtt_client.setKeepAlive(30);
          mqtt_client.setSocketTimeout(120);

          mqtt_client.subscribe("cookerhood/cmnd/#");
          mqtt_client.setBufferSize(512);

          // Publish message upon successful connection
      } else {
          Serial.print("Failed to connect to MQTT broker, rc=");
          Serial.print(mqtt_client.state());
          Serial.println(" try again in 5 seconds");
          delay(5000);
      }
  }
}