
#include <Keypad.h>

/* Button Matrix Pins */
const byte rows = 5;
const byte cols = 4;

byte rowPins[rows] = { 3, 4, 5, 6, 7 };
byte colPins[cols] = { 8, 9, 10, 11 };


char keys[rows][cols] = {
  { 'A', 'B', 'C', 'D' },
  { 'E', 'F', 'G', 'H' },
  { 'I', 'J', 'K', 'L' },
  { 'M', 'N', 'O', 'P' },
  { 'Q', 'R', 'S', 'T' }
};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

void setup() {
  // put your setup code here, to run once:
  	Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Buttons();
}

void Buttons() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    int keyIndex = key;

    /* sendNoteOn(0x39 + keyIndex, 1); */

    /* switch(keyIndex){ */
    /* 	case 12: */
    /* 		sendNoteOn(0x39 + keyIndex, ) */
    /* 		break; */
    /* 	case 13: */
    /* 		break; */
    /* } */

    Serial.println(key);
  }
}