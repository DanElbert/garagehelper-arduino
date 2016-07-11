#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "Timer.h"
#include "Switch.h"
#include "MomentarySwitch.h"
#include "Garage.h"
#include "ChangeNotifier.h"

const int relay1Pin = 9;
const int relay2Pin = 8;

const int switch1Pin = 7;
const int switch2Pin = 6;
const int switch3Pin = 5;

const char* wifi_ssid     = "<SSID>";
const char* wifi_password = "<PW>";

const char* mqtt_server = "<MQTT>";
const char* mqtt_user = "<MQTT USER>";
const char* mqtt_password = "<MQTT PW>";
const char* mqtt_client_name = "<MQTT CLIENT ID>";

static const char* DoorTopicRoot = "garage/door/";
static const char* ButtonTopic = "garage/button";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

Garage garage(relay1Pin, relay2Pin, switch1Pin, switch2Pin, switch3Pin);
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

  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);

  pinMode(switch1Pin, INPUT_PULLUP);
  pinMode(switch2Pin, INPUT_PULLUP);
  pinMode(switch3Pin, INPUT_PULLUP);

  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);

  wifi_connect();

  mqtt.setServer(_server, 1883);
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
      client.subscribe(ButtonTopic);
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
