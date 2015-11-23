#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMesh.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "ESP8266_ILI9341.h"
#include <OneWire.h>

ESP8266_ILI9341 tft = ESP8266_ILI9341();
float dsC[4] = {0, 0, 0, 0};

void setup() {
  pinMode(5, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(3);              //顯示方向
  tft.fillScreen(ILI9341_BLACK);   //填滿黑色

  tft.setTextColor(ILI9341_WHITE); //設定文字顏色
  tft.setTextSize(5);              //設定文字大小
 
}

void loop(void) {
  readDS(2);
  delay(1000);
  tft.fillRect(70, 70, 180, 50, ILI9341_BLACK);
  tft.setCursor(70, 70);
  tft.print(dsC[0]);
  tft.println(" C");
}

String readDS(uint8_t pin)
{
  OneWire ds(pin);
  bool reading = true;
  uint8_t count = 0;
  String message = "", rtnMessage = "";

  while (reading)
  {
    message = readDSAll(&ds, count);
    if (message == "")
      reading = false;
    else
    {
      rtnMessage += message + ",";
      count++;
    }
  }

  if (rtnMessage != "")
    rtnMessage = rtnMessage.substring(0, rtnMessage.length() - 1);

  return "\"Count\":" + String(count) + ",\"Devices\":[" + rtnMessage + "]";
}

String readDSAll(OneWire* ds, uint8_t bufferIndex)
{
  byte present = 0;
  byte addr[8];
  char addrString[17];
  byte data[12];
  byte type_s;
  float celsius, fahrenheit;
  String chip = "";


  if (!ds->search(addr))
  {
    ds->reset_search();
    return "";
  }

  switch (addr[0]) {
    case 0x10:
      chip = "DS18S20";
      type_s = 1;
      break;
    case 0x28:
      chip = "DS18B20";
      type_s = 0;
      break;
    case 0x22:
      chip = "DS1822";
      type_s = 0;
      break;
    default:
      return chip;
  }

  ds->reset();
  ds->select(addr);
  ds->write(0x44, 1);        // start conversion, with parasite power on at the end
  delay(800);

  present = ds->reset();
  ds->select(addr);
  ds->write(0xBE);         // Read Scratchpad

  for (uint8_t i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds->read();
  }

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
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;

  dsC[bufferIndex] = celsius;

  for (uint8_t i = 0; i < 8; i++)
  {
    if (i == 0)
      sprintf(addrString, "%02X", addr[i]);
    else
      sprintf(addrString, "%s%02X", addrString, addr[i]);
  }

  return "{\"Addr\":\"" + String(addrString) + "\",\"Chip\":\"" + chip + "\",\"C\":" + String(celsius) + ",\"F\":" + String(fahrenheit) + "}";
}

