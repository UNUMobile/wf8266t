#include <SPI.h>
#include "Adafruit_GFX.h"
#include "MFRC522.h"
#include "Adafruit_ILI9341.h"

/* wiring the MFRC522 to WF8266R (ESP-12 E F)
RST     = GPIO15
SDA(SS) = GPIO5
MOSI    = GPIO13
MISO    = GPIO12
SCK     = GPIO14
GND     = GND
3.3V    = 3.3V
*/

// For the WF8266TFT shield, these are the default.
#define TFT_DC 5
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

MFRC522 mfrc522(TFT_DC, TFT_CS); // Create MFRC522 instance

void setup() {
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  Serial.begin(115200);    // Initialize serial communications
  delay(250);

  tft.begin();
  tft.setRotation(0);              //顯示方向
  tft.fillScreen(ILI9341_BLACK);   //填滿黑色

  tft.setCursor(0, 0);             //設定游標位置
  tft.setTextColor(ILI9341_WHITE); //設定文字顏色
  tft.setTextSize(1);              //設定文字大小

  tft.println(F("Ready!"));
  tft.println(F("========================================"));
  tft.println(F("Scan for Card and print UID:"));

}

void loop() {
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  // Show some details of the PICC (that is: the tag/card)
  tft.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  tft.println();
}

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    tft.print(buffer[i] < 0x10 ? " 0" : " ");
    tft.print(buffer[i], HEX);
  }
}
