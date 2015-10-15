#include "SPI.h"
#include "Adafruit_GFX.h"
#include "ESP8266_ILI9341.h"

ESP8266_ILI9341 tft = ESP8266_ILI9341();

void setup() {
  pinMode(5,OUTPUT);
  pinMode(16,OUTPUT);
  digitalWrite(16,HIGH);
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(0);              //顯示方向
  tft.fillScreen(ILI9341_BLACK);   //填滿黑色

  tft.setCursor(0, 0);             //設定游標位置
  tft.setTextColor(ILI9341_WHITE); //設定文字顏色 
  tft.setTextSize(1);              //設定文字大小
  tft.println("Hello WF8266T-TFT");//列出
}

void loop() {
  // put your main code here, to run repeatedly:

}

