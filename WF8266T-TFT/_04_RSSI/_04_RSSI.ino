#include <ESP8266WiFi.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "ESP8266_ILI9341.h"

ESP8266_ILI9341 tft = ESP8266_ILI9341();

void setup() {
  pinMode(5, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(0);              //顯示方向
  tft.setTextColor(ILI9341_WHITE); //設定文字顏色
  tft.setTextSize(3);              //設定文字大小
}

void loop() {
  // put your main code here, to run repeatedly:
  getRSSI("UNUMoblie.com.1F");

}

void getRSSI(String SSIDName)
{
  int n = WiFi.scanNetworks();
  if (n == 0)
    tft.println("no networks found");
  else
  {
    for (int i = 0; i < n; ++i)
    {
      if (String(WiFi.SSID(i)) == SSIDName)
      {
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.println(WiFi.RSSI(i));
        break;
      }
    }
  }
}

