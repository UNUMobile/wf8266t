const int clk = 5;
const int stb = 6;
const int dio = 7;

uint8_t fullBuffer[3][16] = {
  { 190, 138, 96, 134, 225, 133, 199, 11, 15, 0, 224, 15, 4, 10, 254, 0 }, // 0 conn l1
  { 190, 138, 96, 134, 225, 133, 199, 11, 15, 0, 224, 15, 4, 202, 147, 70 }, // 1 conn l2
  { 254, 249, 249, 66, 192, 252, 199, 17, 175, 7, 30, 1, 7, 192, 144, 48 } // 2 Arduino
};

void setup() {
  pinMode(stb, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(dio, OUTPUT);
}

void loop() {
  displayClear(255);
  delay(1000);
  displayClear(0);
  delay(1000);

  for (int i = 0; i < 3; i++)
  {
    displayFull(i, 0x8f); // light 0x80 ~ 0x8f 最暗 到 最亮
    delay(5000);
  }
}

void displayFull(uint8_t n, uint8_t light)
{
  uint8_t index = 0;
  for (uint8_t i = 0; i < 8; i++)
  {
    index =  i * 2;

    led_begin(0x44);
    shiftOut(dio, clk, LSBFIRST, 0xC0 + index);
    shiftOut(dio, clk, LSBFIRST, fullBuffer[n][i]);
    led_end(light);
    led_begin(0x44);
    shiftOut(dio, clk, LSBFIRST, 0xC0 + index + 1);
    shiftOut(dio, clk, LSBFIRST, fullBuffer[n][8 + i]);
    led_end(light);
  }
}

void displayClear(uint8_t n)
{
  for (uint8_t i = 0; i < 16; i++)
  {
    led_begin(0x44);
    shiftOut(dio, clk, LSBFIRST, 0xC0 + i);
    shiftOut(dio, clk, LSBFIRST, n);
    led_end(0x8f);
  }
}

void led_begin(uint8_t addr)
{
  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, addr); //data control FIXED 44 AUTO 40
  digitalWrite(stb, HIGH);
  digitalWrite(stb, LOW);
}

void led_end(uint8_t light)
{
  digitalWrite(stb, HIGH);
  //delay(2);
  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, light); //display control
  digitalWrite(stb, HIGH);
}
