#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "ESP8266_ILI9341.h"

ESP8266_ILI9341 tft = ESP8266_ILI9341();

//定義 AP 名稱和密碼
const char *ssid = "WF8266R-AP";
const char *password = "1234567890";
//宣告 Web Server 服務
ESP8266WebServer server(80);

//Web Server 回應處理
void handleRoot() {
  server.send(200, "text/html", F("<h1>Hi, I am WF8266R.</h1><a href='wf8266r?p1=hi&p2=wf8266r'>/wf8266r</a>"));
}

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

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);     //開啟 AP
  delay(500);
  tft.print("Server IP : ");
  tft.println(WiFi.softAPIP());    //伺服器 IP
  tft.print("AP : ");
  tft.println(ssid);

  serverEvent();                //提供服務的網址
  server.begin();                  //啟用 SERVER
}

void loop() {
  server.handleClient();           //監聽連線

}

void serverEvent(){
  server.on("/", handleRoot);      //http://192.168.4.1

  server.on("/wf8266r", []() {     //http://192.168.4.1/wf8266r
    String p1="",p2="";
    server.sendHeader("Connection", "close");
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "p1")
        p1 = server.arg(i);
      if (server.argName(i) == "p2")
        p2 = server.arg(i);
    }

    String message = "{\"p1\":\"" + p1 + "\",\"p2\":\""+p2+"\"}";
    server.send(200, "text/html", message);
    tft.println(message);
  });

  server.on("/gpio", []() {     //http://192.168.4.1/gpio
    String pin="",value="";
    server.sendHeader("Connection", "close");
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "pin")
        pin = server.arg(i);
      if (server.argName(i) == "value")
        value = server.arg(i);  
    }

    String message = "{\"pin\":\"" + pin + "\"value\":\""+value+"\"}";
    server.send(200, "text/html", message);
    tft.println(message);

    pinMode(pin.toInt(),OUTPUT);
    digitalWrite(pin.toInt(),value.toInt());
  });  
}


