#include <MIDI.h>
/* #include <Keypad.h> */

#define SWITCH 4

int volumeSensor = 0;
int volumeValue;
int MidiValue = 0;
int Old_midiValue;
int cc = 7;  // channel volume


int Notecounter = 0;
int val1 = 0;

int MIDI_ROOT_NOTE = 60;

MIDI_CREATE_DEFAULT_INSTANCE();

void SendMIDI (char cmd, char data1, char DATA2) {

  Serial.write (byte(cmd));
  Serial.write (byte(data1));
  Serial.write (byte(DATA2));
}

void Control() {

  val1 = digitalRead(SWITCH);

  if (val1 = HIGH) {
    Notecounter = Notecounter + 1;
    if (Notecounter == 1)
    {
      SendMIDI(0x90, MIDI_ROOT_NOTE, 127);
    }
  }
  else {
    SendMIDI(0x80, MIDI_ROOT_NOTE, 127);
    Notecounter = 0;

  }
}


void Volume() {

  volumeValue = analogRead(volumeSensor);

  MidiValue = volumeValue / 7.6;

  if (MidiValue > 127) {
    MidiValue = 127;
  }

  if (MidiValue != Old_midiValue) {
    SendMIDI (0xB0, 0x00, MidiValue);
    /* SendMIDI (0xB0, 0x07, MidiValue); */
  }
  Old_midiValue = MidiValue;

}

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

int keyState[ROWS][COLS] = {
  {0, 0, 0, 0}, 
  {0, 0, 0, 0}, 
  {0, 0, 0, 0}, 
  {0, 0, 0, 0}
};

byte rowPins[ROWS] = {10, 9, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

/* Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); */

void getKeypad() {
  /* char customKey = customKeypad.getKey(); */

// isPressed(char keyChar)	

  /* if(customKeypad.isPressed('1')){ */
  /*   SendMIDI (0x90, 0x40, 0x40); */
  /* }else{ */
  /*   SendMIDI (0x80, 0x40, 0x40); */
  /* } */
	



/*
  if (customKey == '1') {
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '2'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '3'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == 'A'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '4'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '5'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '6'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == 'B'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '7'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '8'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '9'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == 'C'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '*'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '0'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == '#'){
    SendMIDI (0x90, 0x40, 0x40);
  }else if (customKey == 'D'){
    SendMIDI (0x90, 0x40, 0x40);
  }*/
} 

void setup() {
  MIDI.begin (MIDI_CHANNEL_OFF);
  Serial.begin(115200);


}

void loop() {

  // Control();
  Volume();
  /* getKeypad(); */
  //delay (1);

}
