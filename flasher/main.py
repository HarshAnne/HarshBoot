import serial
from time import sleep

def main():
    ser  = serial.Serial()
    ser.baudrate = 115200
    ser.port = '/dev/ttyUSB0'

    ser.open()

    while True:
        line = ser.readline()
        print(line)

        if(line == bytes('Loader\n','utf-8')):
            ser.write(bytes("harsh\n",'utf-8'))
        
        if(line == bytes('Unlocked\n','utf-8')):

            ser.write(bytearray([0x00,0x10,0x00,0x08]))
            sleep(0.2)

            ser.write(bytearray([0x04,0x00,0x00,0x00]))
            sleep(0.2)

            ser.write(bytearray([0xFE,0xED,0xAB,0xBA]))
            sleep(0.2)

main()
