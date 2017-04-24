import serial
import time
ser = serial.Serial('COM6', 9600, timeout=0)

s=ser.read(100)
print(s)
