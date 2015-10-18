#include <ESP8266WiFi.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "ESP8266_ILI9341.h"

ESP8266_ILI9341 tft = ESP8266_ILI9341();
#define T_CLK 0
#define T_CS 2
#define T_DIN 4
#define T_DOUT 1 //(TX)
#define T_IRQ 3  //(RX)
#define PREC_TOUCH_CONST 10
#define PixSizeX 13.78
#define PixOffsX 411
#define PixSizeY 11.01
#define PixOffsY 378
int TP_X, TP_Y;

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

  Touch_Init();

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int i, j;
  while (Touch_DataAvailable() == 1)
  {
    Touch_Read();
    i = Touch_GetX();
    j = Touch_GetY();
    //tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);  
    //tft.println("i="+String(i)+", j="+String(j));
    tft.drawPixel(i,j,0xFFFF);
  }
}

void Touch_Init(void)
{
  pinMode(T_CLK, OUTPUT);
  pinMode(T_CS, OUTPUT);
  pinMode(T_DIN, OUTPUT);
  pinMode(T_DOUT, INPUT);
  pinMode(T_IRQ, INPUT);
  digitalWrite(T_CS, HIGH);
  digitalWrite(T_CLK, HIGH);
  digitalWrite(T_DIN, HIGH);
  digitalWrite(T_CLK, HIGH);
}

void Touch_WriteData(unsigned char data)
{
  unsigned char temp;
  unsigned char nop;
  unsigned char count;
  temp = data;
  digitalWrite(T_CLK, LOW);
  for (count = 0; count < 8; count++)
  {
    if (temp & 0x80)
      digitalWrite(T_DIN, HIGH);
    else
      digitalWrite(T_DIN, LOW);
    temp = temp << 1;
    digitalWrite(T_CLK, LOW);
    nop++;
    digitalWrite(T_CLK, HIGH);
    nop++;
  }
}

unsigned int Touch_ReadData()
{
  unsigned char nop;
  unsigned int data = 0;
  unsigned char count;
  for (count = 0; count < 12; count++)
  {
    data <<= 1;
    digitalWrite(T_CLK, HIGH);
    nop++;
    digitalWrite(T_CLK, LOW);
    nop++;
    if (digitalRead(T_DOUT))
      data++;
  }

  return (data);
}

void Touch_Read()
{
  unsigned long tx = 0;
  unsigned long ty = 0;
  digitalWrite(T_CS, LOW);
  for (int i = 0; i < PREC_TOUCH_CONST; i++)
  {
    Touch_WriteData(0x90);
    digitalWrite(T_CLK, HIGH);
    digitalWrite(T_CLK, LOW);
    ty += Touch_ReadData();
    Touch_WriteData(0xD0);
    digitalWrite(T_CLK, HIGH);
    digitalWrite(T_CLK, LOW);
    tx += Touch_ReadData();
  }
  digitalWrite(T_CS, HIGH);
  TP_X = tx / PREC_TOUCH_CONST;
  TP_Y = ty / PREC_TOUCH_CONST;
}

char Touch_DataAvailable()
{
  char avail;
  avail = !digitalRead(T_IRQ);
  return avail;
}

int Touch_GetX()
{
  int value;
  value = ((TP_X - PixOffsX) / PixSizeX);
  if (value < 0)
    value = 0;
  return value;
}

int Touch_GetY()
{
  int value;
  value = ((TP_Y - PixOffsY) / PixSizeY);
  if (value < 0)
    value = 0;
  return value;
}
