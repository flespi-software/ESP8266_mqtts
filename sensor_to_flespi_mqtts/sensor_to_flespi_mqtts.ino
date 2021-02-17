// This project shows how to send DS18B20 temperature data from ESP8266 board to flespi MQTT broker
// Read more here https://flespi.com/blog/how-to-connect-esp8266-to-secure-mqtt-broker-know-it-all-and-get-it-done-approach

#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>

/************************* MAIN DEFINITIONS *********************************/
#define ONEWIRE_PIN      14
#define STATIC_MESSAGE_BUF_LEN 256

#define MQTT_SERVER      "mqtt.flespi.io"
#define MQTT_SERVERPORT  8883
#define FLESPI_CERT_FINGERPRINT "6B 4B 7D 8B 78 EC D7 B7 DF 25 3E 96 9D 5F 1F 9D 3C B1 51 57"
#define DEST_TOPIC       "ESP8266/test"
#define FLESPI_TOKEN    "FlespiToken REPLACE_WITH_YOUR_FLESPI_TOKEN"
#define DEVICE_IDENTIFICATION_STRING "flespi"

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

// JSON message creation part
JSONVar message_object;         // to store message parameters
String json_msg_string;         // to stringify JSON message object
char message_string_buf[STATIC_MESSAGE_BUF_LEN];   // to use in mqtt publish function

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

  client.setFingerprint(FLESPI_CERT_FINGERPRINT);

  message_object["ident"] = DEVICE_IDENTIFICATION_STRING;
}

void loop() {
  float cels_degrees;             // temperature sensor value
  int8_t rc;                      // return code for get temperature function
  // add your sensor variable here e.g.
  // int my_sensor_value;

  // establish MQTT connection
  MQTT_connect(mqtt);

  // check for new value on OneWire connected pin
  rc = get_temperature(&cels_degrees);
  if (rc != 0) {
    Serial.print("Failed to read one-wire temperature sensor with code: ");
    Serial.println(rc);
  } else {
    // add temperature sensor value to message object
    message_object["temperature"] = cels_degrees;
  }

  // read add your sensor value and add it to message object, e.g.
  //my_sensor_value = analogRead(A0);
  //message_object["analog.input"] = my_sensor_value;

  // send data to flespi MQTT broker via secure connection
  json_msg_string = JSON.stringify(message_object);
  json_msg_string.toCharArray(message_string_buf, json_msg_string.length() + 1);
  Serial.print("Publishing message to broker: ");
  Serial.println(message_string_buf);
  flespi.publish(message_string_buf);

  // cleanup memory used
  memset(message_string_buf, 0, STATIC_MESSAGE_BUF_LEN);

  // wait a second before repeat
  delay(1000);
}
