#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Fibaro";
const char* password = "";
const char* mqtt_server = "192.168.1.80";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msgIN = "";
  for (int i=0;i<length;i++)
  {
    msgIN += (char)payload[i];
  }
  String msgString = msgIN;
  Serial.println(msgString);

  // --- code for execution goes here ---

  if (msgString == "led1") {
    digitalWrite(14, HIGH);
    Serial.println("triggered if");
  } else if (msgString == "led0") {
    digitalWrite(14, LOW);
    Serial.println("triggered elsif");
  } else if (msgString == "relay1") {
    digitalWrite(12, HIGH);
  } else if (msgString == "relay0") {
    digitalWrite(12, LOW);
  }

  // ------------------------------------

}

void reconnect() {
  while (!client.connected()) {
   Serial.print("Attempting MQTT connection...");
   String clientId = "ESP-";
   clientId += String(random(0xffff), HEX);
   if (client.connect(clientId.c_str())) {
     Serial.println("connected");
     client.publish("testTopic", "ESP connected");
     client.subscribe("testTopic");
     digitalWrite(LED_BUILTIN, LOW);
     delay(250);
     digitalWrite(LED_BUILTIN, HIGH);
     delay(250);
     digitalWrite(LED_BUILTIN, LOW);
     delay(250);
     digitalWrite(LED_BUILTIN, HIGH);
     delay(250);
     digitalWrite(LED_BUILTIN, LOW);
     delay(250);
     digitalWrite(LED_BUILTIN, HIGH);
     delay(250);
     digitalWrite(LED_BUILTIN, LOW);
   } else {
     Serial.print("failed, rc=");
     Serial.print(client.state());
     Serial.println(" try again in 2 seconds");
     delay(2000);
    }
  }
}

void setup(void) {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(14, OUTPUT);
}

void loop(void) {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}
