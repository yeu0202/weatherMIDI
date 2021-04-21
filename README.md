# weatherMIDI
Arduino to MIDI and images for weather station project

Multiple weather related sensors used on Arduino:
Wind sensor, temperature sensors, humidity sensor, light sensor, barometric sensor

Serial data sent from Arduino via USB cable to computer for MIDI messages to generate music
Serial data sent from USB cable to computer for image/video manipulation


ArduinoWeatherFinal is the Processing code for the image manipulation
rtMIDI is the Python code for translating serial output from the arduino to MIDI input for Ableton (requires pyserial and python-rtmidi libraries)
weatherArduinoMIDI is the Arduino code for MIDI output (to python)
weatherProcessingTest is the Arduino code for image manipulation
weather_project2.als is the Ableton file