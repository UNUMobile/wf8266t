const int clk = 14;
const int stb = 15;
const int dio = 16;

void setup() {
  pinMode(stb, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(dio, OUTPUT);
}

void loop() {
  displayClear(255); // SHOW ALL LED
  delay(1000);

  //Light 0x88 ~ 0x8F
  for(int i =0;i<8;i++)
  {
    led_end(0x88+i);
    delay(1000);
  }

  displayClear(0);   // CLOSE ALL LED
  delay(1000);

  //Display Full screen
  for (int i = 0; i < 4; i++)
  {
    displayFull(i, 0x8f); // light 0x88 ~ 0x8f 最暗 到 最亮
    delay(3000);
  }

  //Display number by index
  for (int i = 0; i < 7; i++)
  {
    displayNumber(i, i);
    delay(1000);
    displayClear(0);
  }

  String number = "";
  for (int i = 0; i < 7; i++)
  {
    number += String(i);
    displayNumber(number);
    delay(1000);
  }  

  //Display number 12 3456
  // _ is space
  displayNumber("12_3456");
  delay(1000);

  //Display icon
  for (int i = 0; i < 16; i++)
  {
    displayIcon(i);
    delay(1000);
  }

}

