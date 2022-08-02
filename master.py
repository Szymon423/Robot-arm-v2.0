import serial
import numpy as np
import time

serialPort = serial.Serial(port="COM8", baudrate=115200,
                           bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

serialString = ""
arr = []
i = 0

value = 0

while (True):

    string_ = str(value) + "n"
    serialPort.write(str.encode(string_))
    value += 100

    time.sleep(0.1)
