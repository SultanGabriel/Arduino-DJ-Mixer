#include <SPI.h>
byte Input, Output, Check = 1;
int j;
void setup() {
  pinMode(13, OUTPUT);//clock
  pinMode(11, OUTPUT);//data
  pinMode(4, OUTPUT);//latch
  pinMode(2, INPUT);//Input from buttons


  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
  SPI.transfer(255);
  SPI.transfer(0);
  digitalWrite(4, HIGH);
  digitalWrite(4, LOW);
  Serial.begin(9600);
  //  attachInterrupt(0, pin_read, RISING);

}//setup
int n = 0;
void loop() {
  //  SPI.transfer(n);
  //  digitalWrite(4, HIGH);
  //  digitalWrite(4, LOW);
  //  n++;
  //  if (n == 255) {
  //    n = 0;
  //  }
  //  delay(200);
  read_keypad();
}//loop


/**
   Keypad - 74HC595

   ABCD EFGH
   0000 0000

*/

int test[32];
int TEST[32];
void read_keypad() {
  int t = 0b00000001;
  for (int i = 0; i < 4; i++) {
    
    SPI.transfer(t);
    digitalWrite(4, HIGH);
    digitalWrite(4, LOW);
    delay(10);
    t = t << 1;
    
    if (digitalRead(2)){
		Serial.println(i);
		test.append(t);
	}
  }

  int t = 0b00010000;
  for (int j = 0; j < 4; j++){
  	SPI.transfer(t);

    digitalWrite(4, HIGH);
    digitalWrite(4, LOW);

	t = t << 1; 

    if (digitalRead(3)){
		Serial.println(i);
		TEST.append(t);
	}
  }
  Serial.println(test);
  Serial.println(TEST);
}

//
//void pin_read() {
//
//  for (j = 0; j < 50; j++)
//    delayMicroseconds(1000);
//
//  Check = 1;
//  for (j = 0; j < 8; j++) {
//    SPI.transfer(Check);
//    SPI.transfer(Output);
//    digitalWrite(4, HIGH);
//    digitalWrite(4, LOW);
//    delayMicroseconds(500);
//    if (digitalRead(2) == HIGH) {
//      if (bitRead(Output, j) == 1)
//        bitWrite(Output, j, 0);
//      else
//        bitWrite(Output, j, 1);
//    }//dig check
//
//    Check = Check << 1;
//  }//j
//
//  SPI.transfer(255);
//  SPI.transfer(Output);
//  digitalWrite(4, HIGH);
//  digitalWrite(4, LOW);
//
//  while (digitalRead(2) == HIGH) {}
//
//}//pin_read
