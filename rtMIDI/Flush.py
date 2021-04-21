import time
import rtmidi
import serial

# rtmidi stuff
midiout = rtmidi.MidiOut()
available_ports = midiout.get_ports()
print(available_ports)
midiout.open_port(1)



time.sleep(1)
# Flushed all MIDI hanging notes
def FlushNotes():
    for i in range(127):
        midiout.send_message([0x80, i, 0])
        # midiout2.send_noteon(0, i, 0)
        print('flushnotes')


FlushNotes()
time.sleep(1)