# ESP8266_mqtts

Arduino IDE project: send data from DS18B20 temperature sensor to mqtt.flespi.io via MQTT over SSL.
Read more in [flespi blog](https://flespi.com/blog/how-to-connect-esp8266-to-secure-mqtt-broker-know-it-all-and-get-it-done-approach)

## Scheme
![Screenshot](/conn_scheme.png?raw=true "ESP8266_DS18B20")

## Prerequisites:

- Arduino IDE
- Hardware: ESP8266 board, DS18B20 sensor

## Build Setup

### install additional board to Arduino IDE
[Arduino board project Howto](https://github.com/esp8266/Arduino/blob/master/README.md#installing-with-boards-manager)

Official Arduino IDE info on how to [install additional cores](https://www.arduino.cc/en/Guide/Cores)

### install additional libraries
go to Sketch -> Include Library -> Manage Libraries

type in "Search" and install:

 - [Adafruit_MQTT_Library](https://github.com/adafruit/Adafruit_MQTT_Library) MQTT library for ESP8266 with SSL support

 - [OneWire](https://github.com/PaulStoffregen/OneWire) 1-wire temperature sensors, memory and other chips

### select board
Arduino IDE: Tools -> Board -> Generic ESP8266 module

### get FlespiToken
register on [flespi platform](https://flespi.io), create a Token in a tokens Tab and paste token in [.ino file](https://github.com/flespi-software/ESP8266_mqtts/blob/master/sensor_to_flespi_mqtts/sensor_to_flespi_mqtts.ino) definitions:

```#define FLESPI_TOKEN    "FlespiToken REPLACE_WITH_YOUR_FLESPI_TOKEN" ```

### test
compile the project and flash the board

To test it works you can use flespi [online MQTT client](https://flespi.com/mqtt-broker#client): 

Paste yout token and subscribe to topic *"/ESP8266/test"*


## License
[MIT](https://github.com/flespi-software/ESP8266_mqtts/blob/master/LICENSE) license.
