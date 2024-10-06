import serial
from time import sleep
import os
from intelhex import IntelHex

def main():
    ser  = serial.Serial()
    ser.baudrate = 115200
    ser.port = '/dev/ttyUSB0'
    ser.write_timeout = 1

    ser.open()

    ih = IntelHex('../app/build/app.hex')
    ih_bin_array = ih.tobinarray()
    ih_bin_array_size = len(ih_bin_array)
    ih_bin_array_size_bytes = ih_bin_array_size.to_bytes(4, byteorder='little')
    ih_bin_array_size_bytes = bytearray(ih_bin_array_size_bytes)
    ih_byte_array = bytearray(ih_bin_array)

    while True:
        line = ser.readline()
        print(line)

        if(line == bytes('Loader\n','utf-8')):
            ser.write(bytes("harsh\n",'utf-8'))
        
        if(line == bytes('Unlocked\n','utf-8')):

            # Start address 0x08001000
            ser.write(bytearray([0x00,0x10,0x00,0x08]))
            sleep(0.1)

            ser.write(ih_bin_array_size_bytes)
            sleep(0.1)

            for i in range(0, len(ih_byte_array)):
                ser.write(ih_byte_array[i].to_bytes(1,'little'))
                sleep(0.01)

main()
