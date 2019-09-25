#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

const char* ssid = "Erasmus";
const char* password = "Armandertett123";
const char* mqtt_server = "192.168.0.106";

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

  if (msgIN == "led1") {
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (msgIN == "led0") {
    digitalWrite(LED_BUILTIN, LOW);
  } else if (msgIN == "relay1") {
    digitalWrite(7, HIGH);
  } else if (msgIN == "relay0") {
    digitalWrite(7, LOW);
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
     client.publish("connections", "ESP connected");
     client.subscribe("topic");
   } else {
     Serial.print("failed, rc=");
     Serial.print(client.state());
     Serial.println(" try again in 2 seconds");
     delay(2000);
    }
  }
}

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  u8g2.begin();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop(void) {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  u8g2.clearBuffer();					// clear the internal oled display memory
}
