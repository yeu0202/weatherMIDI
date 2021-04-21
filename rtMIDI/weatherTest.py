import time
import rtmidi



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
        # print('flushnotes')


FlushNotes()
time.sleep(1)
# print('hi')



notes2 = [[64, 0.5], [62, 0.5], [60, 0.5], [62, 0.5], [64, 0.5], [64, 0.5], [64, 1]]


def sendMultipleMessages(noteArray):
    for i in range(len(noteArray[1])):
        midiout.send_message([noteArray[0], noteArray[1][i], noteArray[2]])


# midiout.send_message([0xE0, 64, 0])  # pitch bend
# count = 0

volume1 = 70
volume2 = 30
chordShift = 12
chordShift2 = 7

lastNote = 0

midiout.send_message([0xB0, 73, 30])
midiout.send_message([0xB0, 72, 64])

midiout.send_message([0xB0, 75, 127])
midiout.send_message([0xB0, 76, 0])
midiout.send_message([0xB0, 77, 0])

for note in notes2:
    # midiout.send_message([0xE0, 64, count])  # pitch bend
    print("test note " + str(note[0]))
    midiout.send_message([0x90, note[0], 45])  # note on
    time.sleep(note[1]/4-0.02)
    midiout.send_message([0x80, note[0], 0])  # note off
    time.sleep(0.02)

time.sleep(1)

print("testing instrument")

for i in range(3):  # instrument change
    if i == 0:
        midiout.send_message([0xB0, 75, 127])
        midiout.send_message([0xB0, 76, 0])
        midiout.send_message([0xB0, 77, 0])
    if i == 1:
        midiout.send_message([0xB0, 75, 0])
        midiout.send_message([0xB0, 76, 127])
        midiout.send_message([0xB0, 77, 0])
    if i == 2:
        midiout.send_message([0xB0, 75, 0])
        midiout.send_message([0xB0, 76, 0])
        midiout.send_message([0xB0, 77, 127])




    for j in range(4):
        midiout.send_message([0x90, 60, 0x45])
        time.sleep(0.3)
        midiout.send_message([0x90, 60, 0x00])
        time.sleep(0.3)
    time.sleep(0.5)

time.sleep(1)

midiout.send_message([0xB0, 75, 127])
midiout.send_message([0xB0, 76, 0])
midiout.send_message([0xB0, 77, 0])


print("testing attack")

for i in range(6):
    midiout.send_message([0xB0, 73, 25*i])
    midiout.send_message([0x90, 60, 0x45])
    time.sleep(0.3)
    midiout.send_message([0x90, 60, 0x00])
    time.sleep(0.3)
midiout.send_message([0xB0, 73, 30])

time.sleep(1)
print("testing release")

for i in range(6):
    midiout.send_message([0xB0, 72, 25*i])
    midiout.send_message([0x90, 60, 0x45])
    time.sleep(0.3)
    midiout.send_message([0x90, 60, 0x00])
    time.sleep(0.3)
midiout.send_message([0xB0, 72, 64])


FlushNotes()



del midiout
