#include <MIDI.h>
#include <Keypad.h>

#define SWITCH 4
#define DEBUG true
#define SEND_MIDI false

/* Midi Values */
#define MIDI_MSG_NOTE_OFF 128	 // or 0x80
#define MIDI_MSG_NOTE_ON 144	 // or 0x90
#define MIDI_MSG_CTRL_CHANGE 176 // or 0xB0

/* Linear Volume Potentiometers */
#define VolSensors 3

int volumeSensors[VolSensors] = {A0, A1, A2};
int volumeValues[VolSensors] = {0};
int oldVolumeValues[VolSensors] = {-1};


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
unsigned long btnTimer[rows][cols] = {0};

/* - - - - - - - - - - - - - - - - */

int MidiValue = 0;
int Old_midiValue;
int cc = 7; // channel volume

int Notecounter = 0;
int val1 = 0;

int MIDI_ROOT_NOTE = 60;

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
/* - - - - - - - - - - - - - - - - */

/* - - - - - - - - - - - - - - - - */

int fml[] = {0x3F, 0x40, 0x41}; 
void Volume()
{
	// Loop through every input
	for (int i = 0; i < VolSensors; i++)
	{
		// Read and calculate the appropiate amount
		int current = analogRead(volumeSensors[i]);
		/* current = current * 0.124; */
		/* current = constrain(current, 0, 127); */
		current = map(current, 0, 1023, 0, 126);
		
		current = (current - 3) <= 0 ? 0 : current;
		current = (current + 4) >= 126 ? 126 : current;

		volumeValues[i] = current;

		// If the value has changed by more than 2 then send the value over
		if (abs(volumeValues[i] - oldVolumeValues[i]) > 1)
		{
			sendCtrlChange(0x39 + i, volumeValues[i], 1);

			/* SendMIDI (0xB0, fml[i], volumeValues[i]); */

			/* SendMIDI (0xB0, volumeSensors[i], volumeValues[i]); */
			oldVolumeValues[i] = volumeValues[i];
		}
	}
}
/* ! https://github.com/Chris--A/Keypad/blob/master/examples/MultiKey/MultiKey.ino */
void keypadEvent(KeypadEvent key)
{
	/* Serial.print("-> "); */
	/* Serial.println(key); */

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

/* Potentiometers */
#define pots 3
int potPins[pots] = {3,4,5};
int potValues[pots] = {0};
int OldPotValues[pots] = {-1};
void Potentiometers(){
	for (int i = 0; i < pots; i++)
	{
		// Read and calculate the appropiate amount
		int current = analogRead(potPins[i]);
		/* current = current * 0.124; */
		/* current = constrain(current, 0, 127); */
		current = map(current, 0, 1023, 0, 126);
		
		current = (current - 3) <= 0 ? 0 : current;
		current = (current + 4) >= 126 ? 126 : current;

		potValues[i] = current;

		// If the value has changed by more than 2 then send the value over
		if (abs(potValues[i] - OldPotValues[i]) > 4)
		{
			sendCtrlChange(0x3C + i, potValues[i], 1);

			/* SendMIDI (0xB0, fml[i], volumeValues[i]); */

			/* SendMIDI (0xB0, volumeSensors[i], volumeValues[i]); */
			OldPotValues[i] = potValues[i];
		}
	}
}

void DebugPrint()
{
	if (DEBUG == true)
	{
		Serial.println("Volume Sensors:");
		for (int i = 0; i < VolSensors; i++)
		{
			Serial.print(" ");
			Serial.print(i, DEC);
			Serial.print(" = ");
			Serial.println(volumeValues[i]);
		}
	}
}

void setup()
{
	MIDI.begin(MIDI_CHANNEL_OFF);
	Serial.begin(115200);
	keypad.addEventListener(keypadEvent);
	keypad.setDebounceTime(100);
}

void loop()
{
	Buttons();
	Volume();
	DebugPrint();
  Potentiometers();
	/* getKeypad(); */
	 delay (300); 
}
