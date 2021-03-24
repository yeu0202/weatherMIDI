/*
 * Weather station project
 * 
 * 
 */

#include <SoftwareSerial.h>
#include "Arduino_SensorKit.h"

// Serial setup
SoftwareSerial mySerial(8, 9); // RX, TX. Connect the MIDI lead to pin 9

// Variables to contain the sensor values. 
int windSensorValue = 0; // 0-1023
float temperatureSensorInValue = 25.5; // -10 - 40
float temperatureSensorOutValue = 25.5; // -10 - 40
float humiditySensorValue = 80.0; // 0-100
int lightSensorValue = 500; // 0 - 1023
float barometricSensorValue = 50; // Can also keep track of barometric data history, to see in what way is it changing. Also might not need to retrieve every loop

// cc repeat check variables
int instrumentOld = 0;
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


// Group 1 variables
int light_sensor = A3;
#define DHTPIN 3
//float pressure;


// Group 4 function and variables
int inByte = 0;   // incoming serial byte

void establishContact() {
 while (Serial.available() <= 0) {
 Serial.print('A'); // send a capital A
 delay(300);
 }
}


void setup() {
  // change to midi baud range
  Serial.begin(9600);

  // Group 1 Sensors initialization
  Environment.begin();  
  Pressure.begin();


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


  // Group 4 processing required code
  establishContact(); // send a byte to establish contact until receiver responds
}

void loop() {
/*
 * Wind sensor pin - A1
 * Light sensor pin - A3
 * Temperature and Humidity - Digital pin 3
 * Barometric and temperature  - Digital pin ?
 * 
 */
  
  // read the sensor values here 
  windSensorValue = analogRead(A1); // 0-200
  temperatureSensorInValue = Pressure.readTemperature(); // -10 - 40
  temperatureSensorOutValue = Environment.readTemperature(); // -10 - 40
  humiditySensorValue = Environment.readHumidity(); // 0-100
  lightSensorValue = analogRead(light_sensor); // 0 - 1023
  barometricSensorValue = Pressure.readPressure(); // Can also keep track of barometric data history, to see in what way is it changing. Also might not need to retrieve every loop
  // 99900 - 102000



  // Need to implement sending serial messages out to the computer for the image/video group. 
  // I suggest mapping all the sensor data as was done for MIDI then send it all in a big array format, like 532, 123, 453, 620, 385, 205
  int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5, val6 = 6;

  val1 = map(windSensorValue, 0, 200, 0, 255);
  val2 = map(temperatureSensorInValue, -10, 40, 0, 255);
  val3 = map(temperatureSensorOutValue, -10, 40, 0, 255);
  val4 = map(humiditySensorValue, 0, 100, 0, 255);
  val5 = map(lightSensorValue, 0, 1023, 0, 255);
  val6 = map(barometricSensorValue, 99900, 102000, 0, 255);

  // if we get a valid byte, read analog ins:
 if (Serial.available() > 0) { // we probably don't need this but I'll keep it in anyways
   // get incoming byte:
   inByte = Serial.read(); // I have no idea why we need this, seems to work without but I'll keep it in incase it actually does something
   // send sensor values
   Serial.write(val1);
   Serial.write(val2);
   Serial.write(val3);
   Serial.write(val4);
   Serial.write(val5);
   Serial.write(val6);
 }
  
//  String serialOutput;
//  serialOutput = String(serialOutput + val1 + ", ");
//  serialOutput = String(serialOutput + val2 + ", ");
//  serialOutput = String(serialOutput + val3 + ", ");
//  serialOutput = String(serialOutput + val4 + ", ");
//  serialOutput = String(serialOutput + val5 + ", ");
//  serialOutput = String(serialOutput + val6);
//  Serial.println(serialOutput);

  

  // map can only output integer values, always floors the values. Output is inclusive of minimum, exclusive of maximum. Floors the values within a range
  float windValue = map(windSensorValue, 0, 200, 200, 1500); // between 0.2-1.5, this is the delay in milliseconds
  int temperatureInValue = round(map(temperatureSensorInValue, -10, 40, 0, 127)); // release of note, between 0, 127
  int temperatureOutValue = round(map(temperatureSensorOutValue, -10, 40, 0, 3)); // map to 0/1/2
  int humidityValue = round(map(humiditySensorValue, 0, 100, 0, 60)); // map to attack, 0, ~60
  int lightValue = round(map(lightSensorValue, 0, 1023, 0, 3)); // map to 0/1/2
  int barometricValue = round(map(barometricSensorValue, 99900, 102000, 1, 3)); // map to 1/2


  // Need to implement changing instrument/filter cutoff based on temperatureOutValue, do this by sending a different MIDI cc message based on temperatureOutValue
  if(temperatureOutValue != instrumentOld){
    if(temperatureOutValue == 0){
      sendMIDIMessage(0xB0, 75, 127);
      sendMIDIMessage(0xB0, 76, 0);
      sendMIDIMessage(0xB0, 77, 0);
    }
    if(temperatureOutValue == 1){
      sendMIDIMessage(0xB0, 75, 0);
      sendMIDIMessage(0xB0, 76, 127);
      sendMIDIMessage(0xB0, 77, 0);
    }
    if(temperatureOutValue == 2){
      sendMIDIMessage(0xB0, 75, 0);
      sendMIDIMessage(0xB0, 76, 0);
      sendMIDIMessage(0xB0, 77, 127);
    }
    instrumentOld = temperatureOutValue;
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
  if(barometricValue == 2){
    playingNote = playingNote + 12;
  }

  // Do noteOn, do noteOff before every note to make sure it is reset (not sure if required)
  sendMIDIMessage(0x90, playingNote, 0x45); // maximum is 7F, but 45 is ok. You can also use 0-127 fo the 3rd value
  
  delay(windValue); //windValue is delay in ms

  // Need to implement noteOff based off of the notes that are on.
  sendMIDIMessage(0x90, playingNote, 0x00);
}
