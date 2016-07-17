#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "Timer.h"
#include "Switch.h"
#include "MomentarySwitch.h"
#include "Garage.h"
#include "ChangeNotifier.h"

const int lightPin = 5;
const int openerPin = 4;

const int relayEnablePin = 16;

const int bigDoorPin = 13;
const int backDoorPin = 12; 
const int basementDoorPin = 14;

#include "Secrets.h"
/*
const char* wifi_ssid     = "<SSID>";
const char* wifi_password = "<PW>";
const char* mqtt_server = "<SERVER>";
const char* mqtt_user = "<USER>";
const char* mqtt_password = "<PW>";
const char* mqtt_client_name = "<NAME>";
*/

static const char* DoorTopicRoot = "garage/door/";
static const char* ButtonTopic = "garage/button";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

Garage garage(lightPin, openerPin, bigDoorPin, backDoorPin, basementDoorPin);
ChangeNotifier notifier(&garage, &mqtt, DoorTopicRoot);

// Debug function to return amount of free ram during runtime
int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup() {
  Serial.begin(115200);

  pinMode(lightPin, OUTPUT);
  pinMode(openerPin, OUTPUT);
  pinMode(relayEnablePin, OUTPUT);
  digitalWrite(relayEnablePin, HIGH);
  delay(125);

  pinMode(bigDoorPin, INPUT_PULLUP);
  pinMode(backDoorPin, INPUT_PULLUP);
  pinMode(basementDoorPin, INPUT_PULLUP);

  digitalWrite(lightPin, HIGH);
  digitalWrite(openerPin, HIGH);
  delay(125);
  digitalWrite(relayEnablePin, LOW);

  wifi_connect();

  mqtt.setServer(mqtt_server, 1883);
  mqtt.setCallback(mqtt_callback);

  mqtt_connect();

  notifier.start();
}

void wifi_connect() {
  Serial.println("Starting wifi");
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_connect() {
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt.connect(mqtt_client_name, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      mqtt.subscribe(ButtonTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strcmp(topic, ButtonTopic) == 0) {
    Serial.println("Pressing opener button...");
    garage.pressGarageOpener();
  }
}

void loop() {
  if (!mqtt.connected()) {
    mqtt_connect();
  }
  mqtt.loop();

  garage.update();
  notifier.update();
}
