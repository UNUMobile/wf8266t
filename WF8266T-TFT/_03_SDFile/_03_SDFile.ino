#include <ESP8266WiFi.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "ESP8266_ILI9341.h"
#include <SD.h>

ESP8266_ILI9341 tft = ESP8266_ILI9341();
#define SD_CS 4
File dataFile;

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

  writeFile("/txt/Record.txt", "Hello my world"); //寫入資料
  delay(3000);
  tft.fillScreen(ILI9341_BLACK);                  //填滿黑色
  tft.setCursor(0, 0);                            //回到 0,0
  tft.println("Read /txt/Record.txt"); 
  readFile("/txt/Record.txt");                    //讀取
}

void loop() {
  // put your main code here, to run repeatedly:

}

void writeFile(String fileName, String message)
{
  dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile) {
    dataFile.println(message);
    dataFile.close();

    tft.println(message);
  }
  // if the file isn't open, pop up an error:
  else {
    tft.println("error opening file");
  }
}

void readFile(String fileName)
{
  String message = "";
  dataFile = SD.open(fileName);

  if (dataFile) {
    while (dataFile.available()) {
      message += (char)dataFile.read();
    }
    dataFile.close();
    tft.println(message);
  }
  // if the file isn't open, pop up an error:
  else {
    tft.println("error opening file");
  }

}

