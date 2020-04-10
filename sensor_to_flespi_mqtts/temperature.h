// http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

OneWire  ds(ONEWIRE_PIN); // sensor's data wire connected to pin 14

float get_temperature(float* cels_degrees) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  String temperature;
  
  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return 1; // no new data from sensors, everything's ok
  }
  
  if (OneWire::crc8(addr, 7) != addr[7])
      return -1; //error: onewire data corrupted

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      return -2; // error: unsupported sensor
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);
  delay(1000);
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);

  for ( i = 0; i < 9; i++)
    data[i] = ds.read();

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  *cels_degrees = (float)raw / 16.0;
  return 0; // data written to passed variable
}
