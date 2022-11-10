#include <SPI.h>
int incomingByte = 0; // for incoming serial data
int VolumePots[8] = {0};


void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);//clock
  pinMode(11, OUTPUT);//data
  pinMode(8, OUTPUT);//latch

  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
  SPI.transfer(255);
  SPI.transfer(0);
  digitalWrite(8, HIGH);
  digitalWrite(8, LOW);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  /* if (Serial.available() > 0) { */
  /*   // read the incoming byte: */
  /*   incomingByte = Serial.read() - 48; */

  /*   // say what you got: */
  /*   Serial.print("I received: "); */
  /*   Serial.println(incomingByte, DEC); */
  /*   SPI.transfer(incomingByte); */
  /* } */

  readVolPots(VolumePots);
  for(int i = 0; i < 5; i++){
	Serial.print("Vol Pot ");
	Serial.print(i, DEC);
	Serial.print(" -> ");
	Serial.println(VolumePots[i]);

  }
}

// Read the values of the potentiometers
void readVolPots(int* result){

  int t = 0b00000001;
  for (int i = 0; i < 4; i++) {
    
    SPI.transfer(t);
    digitalWrite(8, HIGH);
	delay(50);
    digitalWrite(8, LOW);
    delay(300);
    Serial.print(t);
    Serial.print(" - ");

    t = t << 1;

    int potValue = analogRead(A1);
    VolumePots[i] = potValue;

	Serial.println(potValue);

	SPI.transfer(0b00000000);
    digitalWrite(8, HIGH);
	delay(50);
    digitalWrite(8, LOW);
  }
}
