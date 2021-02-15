/*
 * Weather station project
 * 
 * 
 */

#include <SoftwareSerial.h>

// Serial setup
SoftwareSerial mySerial(2, 3); // RX, TX. Connect the MIDI lead to pin 3

// Variables to contain the sensor values. 
int windSensorValue = 50; // 0-1023
float temperatureSensorInValue = 25.5; // -10 - 40
float temperatureSensorOutValue = 25.5; // -10 - 40
float humiditySensorValue = 80.0; // 0-100
int lightSensorValue = 500; // 0 - 1023
float barometricSensorValue = 50; // Can also keep track of barometric data history, to see in what way is it changing. Also might not need to retrieve every loop

// cc repeat check variables
int cutoffOld = 0;
int attackOld = 0;
int releaseOld = 0;

// random setup
long randNumber;

// arrays to store the notes
int majorNotes[] = {60, 64, 67, 71, 72};
int minorNotes[] = {60, 63, 67, 70, 72};
int locrianNotes[] = {60, 63, 66, 70, 72};
int allTheNotes[3][5]; // change this number depending on the number of notes in the scales


// I renamed noteOn to sendMIDIMessage because this makes more sense
void sendMIDIMessage(int cmd, int pitch, int velocity) {
  mySerial.write(cmd);
  mySerial.write(pitch);
  mySerial.write(velocity);
}

void flushAllNotes(){
  for(int i=20; i<100; i++){
    sendMIDIMessage(0x90, i, 0x00);
  }
}


void setup() {
  // change to midi baud range
  Serial.begin(9600);

  // MIDI serial speed
  mySerial.begin(31250);

  
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  // Note from Kelvin: We don't have to have this, it's nice but not required, almost nobody will be able to tell the difference
  randomSeed(analogRead(0));

  // puts all 3 note arrays into a single array for easy access later on
  for(int j=0; j<5; j++){
    allTheNotes[0][j] = majorNotes[j];
    allTheNotes[1][j] = minorNotes[j];
    allTheNotes[2][j] = locrianNotes[j];
  }
}

void loop() {
  // read the sensor values here 
  windSensorValue = 50; // 0-1023
  temperatureSensorInValue = 25.5; // -10 - 40
  temperatureSensorOutValue = 25.5; // -10 - 40
  humiditySensorValue = 80.0; // 0-100
  lightSensorValue = 500; // 0 - 1023
  barometricSensorValue = 50; // Can also keep track of barometric data history, to see in what way is it changing. Also might not need to retrieve every loop


  // Need to implement sending serial messages out to the computer for the image/video group. 
  // I suggest mapping all the sensor data as was done for MIDI then send it all in a big array format, like 532, 123, 453, 620, 385, 205
  int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5, val6 = 6;
  String serialOutput;
  serialOutput = String(serialOutput + val1 + ", ");
  serialOutput = String(serialOutput + val2 + ", ");
  serialOutput = String(serialOutput + val3 + ", ");
  serialOutput = String(serialOutput + val4 + ", ");
  serialOutput = String(serialOutput + val5 + ", ");
  serialOutput = String(serialOutput + val6);
  Serial.println(serialOutput);

  

  // map can only output integer values, always floors the values. Output is inclusive of minimum, exclusive of maximum. Floors the values within a range
  float windValue = map(windSensorValue, 0, 1023, 200, 1500); // between 0.2-1.5, this is the delay in milliseconds
//  windValue /= 1000; // do this to get around map only outputting integer values
  int temperatureInValue = round(map(temperatureSensorInValue, -10, 40, 0, 127)); // release of note, between 0, 127
  int temperatureOutValue = round(map(temperatureSensorOutValue, -10, 40, 0, 3)); // map to 0/1/2
  int humidityValue = round(map(humiditySensorValue, 0, 100, 0, 60)); // map to attack, 0, ~60
  int lightValue = round(map(lightSensorValue, 0, 1023, 0, 3)); // map to 0/1/2
  int barometricValue = round(map(barometricSensorValue, 0, 1023, 1, 3)); // map to 1/2


  // Need to implement changing instrument/filter cutoff based on temperatureOutValue, do this by sending a different MIDI cc message based on temperatureOutValue
  if(temperatureOutValue != cutoffOld){
    sendMIDIMessage(0xB0, 74, temperatureOutValue);
    cutoffOld = temperatureOutValue;
  }
  
  // Need to implement changing a MIDI cc message based on humidity, whether it changes Attack/Reverb depends on group 3
  if(humidityValue != attackOld){
    sendMIDIMessage(0xB0, 73, humidityValue);
    attackOld = humidityValue;
  }
  
  // Need to implement changing a MIDI cc message based on temperatureInValue, changes the release of the note
  if(temperatureInValue != releaseOld){
    sendMIDIMessage(0xB0, 72, temperatureInValue);
    releaseOld = temperatureInValue;
  }

  // Might need (might not) to flush all notes when changing instrument, changing attack/reverb, changing the release of the note, changing major/minor/locrian
//  flushAllNotes();
  

  randNumber = random(5);
  int playingNote = allTheNotes[lightValue][randNumber]; // this is the base note to play
  // Need to implement the shifting of notes based on barometricValue, increase or decrease notes by 12 then play them again
  

  // Do noteOn, do noteOff before every note to make sure it is reset (not sure if required)
  sendMIDIMessage(0x90, playingNote, 0x45); // maximum is 7F, but 45 is ok. You can also use 0-127 fo the 3rd value
  
  delay(windValue); //windValue is delay in ms

  // Need to implement noteOff based off of the notes that are on.
  sendMIDIMessage(0x90, playingNote, 0x00);
}
