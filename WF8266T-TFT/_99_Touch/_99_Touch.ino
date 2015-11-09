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
#define PixOffsY 200//378
int TP_X, TP_Y;
boolean RecordOn = false;
#define FRAME_X 210
#define FRAME_Y 10
#define FRAME_W 100
#define FRAME_H 50

#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H

uint8_t btns[4] = { 0 , 0 , 0 , 0};
bool isRequesting = false, btnChange = false, isConnect = false;
unsigned long checkTime = 0, touchTime = 0;

void setup() {

  pinMode(5, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);              //顯示方向
  tft.fillScreen(ILI9341_BLACK);   //填滿黑色

  tft.setCursor(0, 0);             //設定游標位置
  tft.setTextColor(ILI9341_WHITE); //設定文字顏色
  tft.setTextSize(1);              //設定文字大小

  Touch_Init();
  redBtn(0, 0);
  redBtn(60, 1);
  redBtn(120, 2);
  redBtn(180, 3);
  connectWiFi();
}

void connectWiFi()
{
  tft.setCursor(0, 5);
  tft.setTextSize(1);
  WiFi.begin("WF8266R-DEMOAP", "1234567890");
  tft.println("WF8266Touch");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  tft.println(WiFi.localIP());

  isConnect = true;
}

void toggleRelay()
{
  if (isRequesting)
    return;

  char buf[20];
  uint8_t times = 0;
  sprintf(buf, "5=%d&4=%d&12=%d&13=%d", btns[0] == 0 ? 1 : 0, btns[1] == 0 ? 1 : 0, btns[2] == 0 ? 1 : 0, btns[3] == 0 ? 1 : 0);
  WiFiClient wifiClient;
  if (!wifiClient.connect("192.168.1.3", 80)) {
    isRequesting = false;
    return ;
  }

  wifiClient.print(String("GET /gpio?") + String(buf) + " HTTP/1.1\r\n" +
                   "Host: 192.168.1.3\r\n" +
                   "Connection: close\r\n\r\n");


  while (!wifiClient.available() && times < 10) {
    delay(500);
    times++;
  }

  isRequesting = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int i, j;

  while (Touch_DataAvailable() == 1 && millis() - touchTime > 500)
  {
    touchTime = millis();
    Touch_Read();
    i = Touch_GetX();
    j = Touch_GetY();
    //tft.fillScreen(ILI9341_BLACK);
    tft.fillRect(0, 0, 160, 50, ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(1);
    tft.println("x=" + String(i) + ", y=" + String(j));
    //tft.drawPixel(240-i,j,0xFFFF);

    if (i > 30 && i < 50 && j > 200 && j < 295) //btn1
    {
      tft.print("Relay 1 ");
      if (btns[0] == 0)
        greenBtn(0, 0);
      else
        redBtn(0, 0);
    }
    else if (i > 58 && i < 120 && j > 200 && j < 295) //btn2
    {
      tft.print("Relay 2 ");
      if (btns[1] == 0)
        greenBtn(60, 1);
      else
        redBtn(60, 1);
    }
    else if (i > 99 && i < 184 && j > 200 && j < 295) //btn3
    {
      tft.print("Relay 3 ");
      if (btns[2] == 0)
        greenBtn(120, 2);
      else
        redBtn(120, 2);
    }
    else if (i > 190 && i < 240 && j > 200 && j < 295) //btn4
    {
      tft.print("Relay 4 ");
      if (btns[3] == 0)
        greenBtn(180, 3);
      else
        redBtn(180, 3);
    }
  }

  if (millis() - checkTime  > 500 && btnChange && isConnect)
  {
    checkTime = millis();
    toggleRelay();
    btnChange = false;
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
}

void drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
}


void redBtn(uint8_t top, uint8_t btnNumber)
{
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y + top, REDBUTTON_W, REDBUTTON_H, ILI9341_RED);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y + top, GREENBUTTON_W, GREENBUTTON_H, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + top + (GREENBUTTON_H / 2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("ON");
  btns[btnNumber] = 0;
  btnChange = true;
}

void greenBtn(uint8_t top, uint8_t btnNumber)
{
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y + top, GREENBUTTON_W, GREENBUTTON_H, ILI9341_GREEN);
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y + top, REDBUTTON_W, REDBUTTON_H, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + top + (REDBUTTON_H / 2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("OFF");
  btns[btnNumber] = 1;
  btnChange = true;
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
