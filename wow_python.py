import sys
import serial
import threading

ser = serial('/dev/ttyUSB0',baudrate=9600,timeout=1)

while True :
    input_string = ser.readline().decode()
    radar = int(input_string)

    input_string = ser.readline().decode()
    IR_data = int(input_string)

    print(radar)
    print(IR_data)
    print()
