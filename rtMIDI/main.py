import time
import rtmidi
import serial


# Serial stuff
ser = serial.Serial()  # open serial port
print(ser.name)         # check which port was really used
ser.baudrate = 9600
ser.port = 'COM3'
ser.open()
print(ser)
print(ser.is_open)

# rtmidi stuff
midiout = rtmidi.MidiOut()
available_ports = midiout.get_ports()
print(available_ports)
midiout.open_port(1)

skipFirst = False


# time.sleep(1)
# # Flushed all MIDI hanging notes
# def FlushNotes():
#     for i in range(127):
#         midiout.send_message([0x80, i, 0])
#         # midiout2.send_noteon(0, i, 0)
#         print('flushnotes')


# FlushNotes()
time.sleep(1)
# print('hi')

# notes = [[64, 0.5], [62, 0.5], [60, 0.5], [62, 0.5], [64, 0.5], [64, 0.5], [64, 1], [62, 0.5],
#          [62, 0.5], [62, 1], [64, 0.5], [67, 0.5], [67, 1], [64, 0.5], [62, 0.5], [60, 0.5],
#          [62, 0.5], [64, 0.5], [64, 0.5], [64, 0.5], [64, 0.5], [62, 0.5], [62, 0.5], [64, 0.5],
#          [62, 0.5], [60, 2]]
#
# scales = [[60, 64, 67], [60, 63, 67], [60, 65, 69], [60, 67, 70]]
#
# def sendMultipleMessages(noteArray):
#     for i in range(len(noteArray[1])):
#         midiout.send_message([noteArray[0], noteArray[1][i], noteArray[2]])


# midiout.send_message([0xE0, 64, 0])  # pitch bend
# count = 0
#
# volume1 = 70
# volume2 = 30
# chordShift = 12
# chordShift2 = 7
#
# lastNote = 0

# for note in notes:
#     # midiout.send_message([0xE0, 64, count])  # pitch bend
#     print("test note " + str(note[0]))
#     midiout.send_message([0x90, note[0], 112])  # note on
#     midiout.send_message([0x90, note[0] + chordShift, 30])  # note on
#     midiout.send_message([0x90, note[0] + chordShift2, 50])  # note on
#     time.sleep(note[1]/2-0.05)
#     midiout.send_message([0x80, note[0], 0])  # note off
#     midiout.send_message([0x80, note[0] + chordShift, 0])  # note off
#     midiout.send_message([0x80, note[0] + chordShift2, 0])  # note off
#     time.sleep(0.05)

def retrieveNote():
    # print('retrieving')
    # ser.flushInput()
    # ser.flushOutput()
    time.sleep(0.05)
    s = ser.readline()
    # print(s)
    # s = ser.readline()  # do it twice because serial is a stream of bytes, the flush sometimes
                        # starts flushing from the middle of a byte. Read next input to bypass.
                        # since we have (time.sleep) > (1+arduino message output rate), it's fine
                        # this should also solve the empty input issue but I'll leave that uncommented
    # print(s)
    # tempvar = s.decode("utf-8")
    # # print(tempvar)
    # # print(tempvar == '\r\n')
    # # print(tempvar == '\n')
    # if tempvar == '\r\n' or tempvar == '\n':  # checks for empty input from arduino, not sure why they are empty
    #     return -1
    # # print(tempvar)
    #
    # tempvar = int(tempvar)


    tempvar = s.decode()
    string = tempvar.rstrip()

    return string


def parseString(inputString):

    # outputList = [0, 0, 0]

    outputList = inputString.split(",")
    # print(outputList)
    # for i in range(2):
    #     outputList[i] = int(outputList[i])
    outputList = [int(i) for i in outputList]

    return outputList




while 1:
    arduinoString = retrieveNote()
    # print(arduinoString)

    # midiMessage is in the format [int, int, int]
    print(arduinoString)
    if not skipFirst:
        skipFirst = True
    else:
        midiMessage = parseString(arduinoString)
        # print(midiMessage)
        midiout.send_message(midiMessage)



# for i in range(500):
#     noteValue = retrieveNote()
#     print(noteValue)

    # midiout.send_message([0x90, tempvar2, volume1])
    # time.sleep(0.5)
    # midiout.send_message([0x80, tempvar2, 0])

    # if noteValue == 0:
    #     break
    # if 0 <= noteValue < 25:
    #     noteValue = 0
    # elif 25 <= noteValue < 50:
    #     noteValue = 1
    # elif 50 <= noteValue < 75:
    #     noteValue = 2
    # elif 75 <= noteValue:
    #     noteValue = 3
    # # noteValue = noteValue % 4
    # print("notevalue: ", noteValue)
    #
    #
    # if noteValue != lastNote:
    #     sendMultipleMessages([0x80, scales[lastNote], 0])  # turn off previous note
    #
    #     sendMultipleMessages([0x90, scales[noteValue], volume1])  # turn on this note
    #     # time.sleep(0.5)
    #
    # lastNote = noteValue

# FlushNotes()


# midiout.send_message([0xE0, 64, 0])  # pitch bend

del midiout
