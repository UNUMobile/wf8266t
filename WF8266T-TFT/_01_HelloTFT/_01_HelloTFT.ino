#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the WF8266TFT shield, these are the default.
#define TFT_DC 5
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
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

