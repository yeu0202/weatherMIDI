import time
import rtmidi



# rtmidi stuff
midiout = rtmidi.MidiOut()
available_ports = midiout.get_ports()
print(available_ports)
midiout.open_port(0)


# midiout.send_message([0xB0, 75, 0])  # instrument 1
# midiout.send_message([0xB0, 76, 1])  # instrument 2
# midiout.send_message([0xB0, 77, 2])  # instrument 3
# midiout.send_message([0xB0, 73, 30])  # attack range 0-60
midiout.send_message([0xB0, 72, 64])  # release range 0-127


del midiout