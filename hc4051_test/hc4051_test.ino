int potPins[] = {17, 18,19};

void setup(){
	Serial.begin(115200);

	for (int i = 0; i < 3; i++){
		pinMode(potPins[i], OUTPUT);
	}
}

void loop(){
	for(byte i = 0; i < 8; i++){
		digitalWrite(potPins[0], HIGH && (i & B00000001));
		digitalWrite(potPins[1], HIGH && (i & B00000010));
		digitalWrite(potPins[2], HIGH && (i & B00000100));
		delay(100);

		int value = analogRead(A0);

		Serial.print(i, DEC); 
    Serial.print(" - ");
    Serial.println(value, DEC);
    // if(i == 7){
    //   Serial.println(value);
    // }
    // delay(100);
  
  }

}
