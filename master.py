import serial
import numpy as np
import time
import matplotlib.pyplot as plt

serialPort = serial.Serial(port="COM8", baudrate=115200,
                           bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

serialString = ""
i = 0

arr1 = np.linspace(0, 2000, 2000, dtype=int)
arr2 = np.linspace(2000, 0, 2000, dtype=int)

arr_pos = np.concatenate((arr1, arr2), axis=0, out=None)

plt.plot(arr_pos)
plt.show()

time_max = 4000     # ms

time_div = arr_pos.size / time_max
print(time_div)

arr_speed = [(np.abs(arr_pos[i] - arr_pos[i-1]) / time_div * 1000.0) for i in range(1, time_max)]

cnt = 0

while (True):

    string_ = str(arr_pos[cnt]) + "x" + str(arr_speed[cnt]) + "n"
    serialPort.write(str.encode(string_))
    cnt += 1
    
    time.sleep(time_div / 1000.0)

    print(serialPort.readline())
