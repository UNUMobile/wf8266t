#include <ESP8266WiFi.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "ESP8266_ILI9341.h"
#include <SD.h>

ESP8266_ILI9341 tft = ESP8266_ILI9341();
#define SD_CS 4
File root;

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

  tft.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    tft.println("failed!");
    return;
  }
  
  tft.println("OK!");
  root = SD.open("/txt");   //開啟 TXT 目錄
  printDirectory(root, 0);  //印出檔案清單
}

void loop() {
  // put your main code here, to run repeatedly:

}

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       tft.print(" ");
     }
     tft.print(entry.name());
     if (entry.isDirectory()) {
       tft.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       tft.print("  ");
       tft.println(entry.size(), DEC);
     }
     entry.close();
   }
}

