#define Button_A D5
#define Button_B D2
#define Button_C D1

void  ICACHE_RAM_ATTR A();
void  ICACHE_RAM_ATTR B();
void  ICACHE_RAM_ATTR C();

void setup() {
  pinMode(Button_A, INPUT_PULLUP);
  pinMode(Button_B, INPUT_PULLUP);
  pinMode(Button_C, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(Button_A), A, FALLING);
  attachInterrupt(digitalPinToInterrupt(Button_B), B, FALLING);
  attachInterrupt(digitalPinToInterrupt(Button_C), C, FALLING);
}

void loop() {
  delay(500);
}

void A() {
  Serial.println("A");
}
void B() {
  Serial.println("B");
}
void C() {
  Serial.println("C");
}
