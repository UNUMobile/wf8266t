//顯示資料，共7位數。要顯示的數字放這進這個陣列，對應位置 1~8
uint8_t displayBuffer[] = {0, 0, 0, 0, 0, 0, 0, 0}; //255 全暗不顯示
//顯示位址，不需要更動
uint8_t displayAddr [] = {0xc1, 0xc3, 0xc5, 0xc7, 0xc9, 0xcb, 0xcd, 0xcf};
//0 ~ 9 字型陣列
uint8_t numberCode [] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
//按鍵值
uint8_t key = 0;
//顯示亮度 0x88 ~ 0x8f
uint8_t lightLevel = 0x88;
//控制 IO 不需更動
const uint8_t clk = 14;
const uint8_t stb = 15;
const uint8_t dio = 16;

void kdInit(){
  pinMode(clk, OUTPUT);
  pinMode(dio, OUTPUT);
  pinMode(stb, OUTPUT);
}

void displayNumberByIndex(uint8_t index, uint8_t value)
{
  displayBuffer[index] = value;
}

uint8_t getKey()
{
  return key;
}
void displayClear(uint8_t n)
{
  for (uint8_t i = 0; i < 16; i++)
  {
    led_begin(0x44);
    shiftOut(dio, clk, LSBFIRST, 0xC0 + i);
    shiftOut(dio, clk, LSBFIRST, n);
    led_end(lightLevel);
  }
}

void kdShow(uint8_t level) 
{
  lightLevel = level;
  
  for (uint8_t i = 0; i < 8; i++)
  {
    uint8_t current = numberCode[displayBuffer[i]];

    if (displayBuffer[i] == 255)
      current = 0;

    led_begin(0x44);
    shiftOut(dio, clk, LSBFIRST, displayAddr[i]);
    shiftOut(dio, clk, LSBFIRST, current);
    led_end(lightLevel);
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
  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, light); //display control
  digitalWrite(stb, HIGH);
}

void kdKey() {
  digitalWrite(stb, HIGH);
  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, 0x42); //read key
  delay(2);
  key = key_read();
  digitalWrite(stb, HIGH);
}


uint8_t key_read()
{
  uint8_t temp = 0;
  digitalWrite(dio, HIGH);
  for (uint8_t i = 0; i < 8; i++)
  {
    temp >>= 1;
    digitalWrite(clk, LOW);
    if (digitalRead(dio) == HIGH)
      temp |= 0x80;
    digitalWrite(clk, HIGH);
  }
  return temp;
}


