#include <MIDI.h>
#include <Keypad.h>

#define DEBUG false
#define SEND_MIDI true

/* Midi Values */
#define MIDI_MSG_NOTE_OFF 128	 // or 0x80
#define MIDI_MSG_NOTE_ON 144	 // or 0x90
#define MIDI_MSG_CTRL_CHANGE 176 // or 0xB0

/* Button Matrix Pins */
const byte rows = 5;
const byte cols = 4;

byte rowPins[rows] = {3, 4, 5, 6, 7};
byte colPins[cols] = {8, 9, 10, 11};

char keys[rows][cols] = {
	{'A', 'B', 'C', 'D'},
	{'E', 'F', 'G', 'H'},
	{'I', 'J', 'K', 'L'},
	{'M', 'N', 'O', 'P'},
	{'Q', 'R', 'S', 'T'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

#define DEBOUNCE_MS 100

/* - - - - - - - - - - - - - - - - */


MIDI_CREATE_DEFAULT_INSTANCE();

void SendMIDI(char cmd, char data1, char DATA2)
{
	if (SEND_MIDI == true)
	{
		Serial.write(byte(cmd));
		Serial.write(byte(data1));
		Serial.write(byte(DATA2));
		/* delay(10); */
	}
}

// MIDI------------------------------------------
void sendNoteOn(byte note, byte channel)
{
	sendChannelVoiceMsg(MIDI_MSG_NOTE_ON | (channel - 1), note, 127);
}

/* void sendNoteOff(byte note,byte velocity,byte channel){ */
void sendNoteOff(byte note, byte channel)
{
	sendChannelVoiceMsg(MIDI_MSG_NOTE_OFF | (channel - 1), note, 0);
}

void sendCtrlChange(byte cc, byte value, int channel)
{
	sendChannelVoiceMsg(MIDI_MSG_CTRL_CHANGE | (channel - 1), cc, value);
}

void sendChannelVoiceMsg(byte command, byte note, byte velocity)
{
	// send MIDI Channel Voice Message
	if (SEND_MIDI == true){
		Serial.write(command);
		Serial.write(note);
		Serial.write(velocity);
	}
}

/* ! https://github.com/Chris--A/Keypad/blob/master/examples/MultiKey/MultiKey.ino */
void keypadEvent(KeypadEvent key)
{
	int keyIndex = key - 65;
	switch (keypad.getState())
	{
		case PRESSED:
			sendNoteOn(0x39 + keyIndex, 1);
			break;
		case RELEASED:
			sendNoteOff(0x39 + keyIndex, 1);
			break;
		case HOLD:
			break;
	}
}

/* Potentiometers */
int selectPins[3] = {17, 18, 19};
void selectMuxPin(byte pin) {
	for (int i = 0; i < 3; i++) {
		if (pin & (1 << i))
			digitalWrite(selectPins[i], HIGH);
		else
			digitalWrite(selectPins[i], LOW);
	}
	delay(10);
}


int AnalogRead(int pin){
	int out = 0;
	for(int i = 0; i < 5; i++){
		out += analogRead(pin - 14);
		out /= 2;
	}
	return out;
}

// AnalogToMidi
int ATM(int value){
	int out = -1;

	out = value > 100 ? value : 0;
	out = value < 980 ? value : 1023;
	out = map(out, 0, 1023, 0, 126);

	return out;
}
const int zInput = 14;
const int wInput = 15;
const int PotNum = 16;
const int PotTolerance = 5;
const int PotMidi = 0x39;

int oldPotValues[16] = {-1};
void Potentiometers(){
	for (byte pin = 0; pin < 8; pin++){
		selectMuxPin(pin);

		int zValue = ATM(AnalogRead(zInput));
		if(abs(zValue - oldPotValues[pin]) > PotTolerance){
			sendCtrlChange(PotMidi + pin, zValue, 1);
			oldPotValues[pin] = zValue;
		}

		int wValue = ATM(AnalogRead(wInput));
		if(abs(wValue - oldPotValues[8 + pin]) > PotTolerance){
			sendCtrlChange(PotMidi + 8 + pin, wValue, 1);
			oldPotValues[pin + 8] = wValue;
		}
	}
}

void setup()
{
	MIDI.begin(MIDI_CHANNEL_OFF);
	Serial.begin(38400);

	// Keypad Setup
	keypad.addEventListener(keypadEvent);
	keypad.setDebounceTime(100);

	// Mux Input Pins
	pinMode(zInput, INPUT);
	pinMode(wInput, INPUT);
	for (int i = 0; i < 3; i++){
		pinMode(selectPins[i], OUTPUT);
	}
}

void Buttons()
{
	char key = keypad.getKey();

	if (key != NO_KEY)
	{
		int keyIndex = key - 65;

		/* sendNoteOn(0x39 + keyIndex, 1); */

		/* switch(keyIndex){ */
		/* 	case 12: */
		/* 		sendNoteOn(0x39 + keyIndex, ) */
		/* 		break; */
		/* 	case 13: */
		/* 		break; */
		/* } */

		/* Serial.println(key); */
	}
}
void loop()
{
	Buttons();
	// Volume();
	// DebugPrint();
	Potentiometers();
	/* getKeypad(); */
	/* delay(300); */ 
}
