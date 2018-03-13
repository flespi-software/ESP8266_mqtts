#include <ESP8266WiFi.h>

/************************* MAIN DEFINITIONS *********************************/
#define ONEWIRE_PIN      14

#define MQTT_SERVER      "mqtt.flespi.io"
#define MQTT_SERVERPORT  8883
#define FLESPI_CERT_FINGERPRINT "3B BC 95 33 E5 AB C1 1C C8 FC 37 57 F2 94 2C 43 8E 3B 66 F3"
#define DEST_TOPIC       "ESP8266/test"
#define FLESPI_TOKEN    "FlespiToken REPLACE_WITH_YOUR_FLESPI_TOKEN"

#define WLAN_SSID       "SSID"
#define WLAN_PASS       "PASS"

/**************************** Local files ***********************************/
#include "temperature.h"
#include "adafruit_mqtts.h"

/********************* Global connection instances **************************/
// WiFiFlientSecure for SSL/TLS support
WiFiClientSecure client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, FLESPI_TOKEN, "");
// Setup a feed 'flespi' to publish messages to flespi MQTT broker.
Adafruit_MQTT_Publish flespi = Adafruit_MQTT_Publish(&mqtt, DEST_TOPIC);

/*************************** Sketch Code ************************************/
// Generic example for ESP8266 wifi connection
void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("ESP8266: flespi MQTT over SSL example");

  // Connect to WiFi access point.
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  delay(1000);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  delay(2000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // connect to server check the fingerprint of flespi.io's SSL cert (from Adafruit MQTTS example)
  verify_fingerprint(client);
}

void loop() {
  char temperature_string_buf[8]; // buffer for sensor's temperature value
  int8_t rc;                      // return code for get temperature function

  // establish MQTT connection
  MQTT_connect(mqtt);

  // check for new value on OneWire connected pin
  rc = get_temperature(temperature_string_buf);
  if (rc != 0) {
    //Serial.println(rc);
    return;
  }
  Serial.print("Temperature value = ");
  Serial.println(temperature_string_buf);

  // send data to flespi MQTT broker via secure connection
  flespi.publish(temperature_string_buf);

  // wait a second before repeat
  delay(1000);
}

