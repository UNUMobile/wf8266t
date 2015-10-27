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
  tft.fillScreen(ILI9341_BLACK);   //填滿黑色

  tft.setCursor(0, 0);             //設定游標位置
  tft.setTextColor(ILI9341_WHITE); //設定文字顏色
  tft.setTextSize(1);              //設定文字大小

  scanWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void scanWiFi()
{
  WiFi.mode(WIFI_STA);              //設定為 AP 樣式
  WiFi.disconnect();                //斷開所有連線
  delay(100);

  tft.println("AP OPEN");
  tft.print("Scanning...");

  int n = WiFi.scanNetworks();
  tft.println(" done.");
  if (n == 0)
    tft.println("no networks found");
  else
  {
    tft.print(n);
    tft.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      tft.print(i + 1);
      tft.print(": ");
      tft.print(WiFi.SSID(i));      // 第 i 台 AP 的名稱 (SSID)
      tft.print(" (");
      tft.print(WiFi.RSSI(i));      // 第 i 台 AP 的訊號強度 (RSSI)
      tft.print(")");
      tft.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*"); // 加密方式
      delay(10);
    }
  }
}
