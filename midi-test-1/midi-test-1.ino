#include <MIDI.h>
/* #include <Keypad.h> */

#define SWITCH 4
#define DEBUG false
#define SEND_MIDI true 

/* Midi Values */

#define MIDI_MSG_NOTE_OFF 128 //or 0x80
#define MIDI_MSG_NOTE_ON 144 //or 0x90
#define MIDI_MSG_CTRL_CHANGE 176 //or 0xB0

/* Linear Volume Potentiometers */
#define VolSensors 5

int volumeSensors[VolSensors] = {0, 1, 2, 3, 4};
int volumeValues[VolSensors] = {0};
int oldVolumeValues[VolSensors] = {-1};

int MidiValue = 0;
int Old_midiValue;
int cc = 7;  // channel volume

int Notecounter = 0;
int val1 = 0;

int MIDI_ROOT_NOTE = 60;

MIDI_CREATE_DEFAULT_INSTANCE();

void SendMIDI (char cmd, char data1, char DATA2) {
  if(SEND_MIDI == true){
	  Serial.write (byte(cmd));
	  Serial.write (byte(data1));
	  Serial.write (byte(DATA2));
	  /* delay(10); */
  }
}

//MIDI------------------------------------------
void sendNoteOn(byte note,byte velocity,byte channel){
	sendChannelVoiceMsg(MIDI_MSG_NOTE_ON|(channel-1),note,velocity);
}

void sendNoteOff(byte note,byte velocity,byte channel){
	sendChannelVoiceMsg(MIDI_MSG_NOTE_OFF|(channel-1),note,velocity);
}

void sendCtrlChange(byte cc,byte value,int channel) {
	sendChannelVoiceMsg(MIDI_MSG_CTRL_CHANGE|(channel-1),cc,value);
}

void sendChannelVoiceMsg(byte command, byte note, byte velocity) {
	//send MIDI Channel Voice Message
	Serial.write(command);
	Serial.write(note);
	Serial.write(velocity);
}

//
// Not Used ?

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

int fml[] = {0x3F, 0x40, 0x41, 0x42, 0x57};
void Volume() {
	// Loop through every input
	for(int i = 0; i < VolSensors; i++){
	// Read and calculate the appropiate amount
	int current = analogRead(volumeSensors[i]);
	current = current * 0.124;	
    current = constrain(current, 0, 127);	

	volumeValues[i] = current;

	// If the value has changed by more than 2 then send the value over 
	if (abs(volumeValues[i] - oldVolumeValues[i]) > 2){
		SendMIDI (fml[i], 0x00, volumeValues[i]);

		/* SendMIDI (0xB0, fml[i], volumeValues[i]); */
		/* SendMIDI (0xB0, volumeSensors[i], volumeValues[i]); */
		oldVolumeValues[i] = volumeValues[i];
	}
}

  /* if (MidiValue > 127) { */
  /*   MidiValue = 127; */
  /* } */

  /* if (MidiValue != Old_midiValue) { */
  /*   SendMIDI (0xB0, 0x00, MidiValue); */
  /*   /1* SendMIDI (0xB0, 0x07, MidiValue); *1/ */
  /* } */
  /* Old_midiValue = MidiValue; */

}

void DebugPrint() {
	if(DEBUG == true){
		Serial.println("Volume Sensors:");
		for(int i = 0; i < VolSensors; i++){
			Serial.print("	" + int(i));
			Serial.println(" = " + int(volumeValues[i]));
		}
	}
}

void setup() {
	MIDI.begin(MIDI_CHANNEL_OFF);
	Serial.begin(115200);
}

void loop() {

  // Control();
  Volume();
  DebugPrint();
  /* getKeypad(); */
  /* delay (100); */

}
