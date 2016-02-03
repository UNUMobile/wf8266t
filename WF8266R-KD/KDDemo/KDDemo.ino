void setup() {
  kdInit();
  Serial.begin(115200);

  lightDEMO();
  numberDemo();
}

void loop() {

  kdShow(0x88);
  kdKey();
  delay(100);
  Serial.println(getKey());
}

void lightDEMO() {
  for (int i = 0; i < 8; i++)
  {
    kdShow(0x88 + i);
    delay(1000);
  }
}

void numberDemo() {
  for(int i = 0; i< 8; i++)
  {
    displayNumberByIndex(i,i);
    kdShow(0x88);
    delay(1000);
  }
}

