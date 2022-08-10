import serial
import numpy as np
import time
import matplotlib.pyplot as plt

serialPort = serial.Serial(port="COM7", baudrate=115200,
                           bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

serialString = ""
i = 0


steps = 1000
arr1 = np.linspace(0, 2000, steps+1, dtype=int)
arr2 = np.linspace(2000, 0, steps+1, dtype=int)
x = np.linspace(0, 4000, steps)
y = 4000 - 4000 * np.cos(x / (2 * np.pi * 50))


# arr_pos = np.concatenate((arr1, arr2), axis=0, out=None)

arr_pos = np.asarray(y, dtype=int)

# print(arr_pos)
time_max = 10000     # ms

time_div = time_max / (arr_pos.size - 2) # 40000 / 40 = 1000
print("time_div: " + str(time_div))
print("różnica: " + str(np.abs(arr_pos[2] - arr_pos[1])))

arr_speed =[0.625 * (np.abs(arr_pos[i+1] - arr_pos[i]) / time_div * 1000.0) for i in range(0, arr_pos.size-1)]
arr_speed.insert(0, arr_speed[0])
arr_speed = np.asarray(arr_speed)

arr_speed_int = np.array(arr_speed, dtype=int)

# print(arr_speed_float)
print("speed is set to: " + str(arr_speed_int[1]))
cnt = 0
dir = 1
first = True

while (True):

    if cnt >= arr_pos.size or (cnt == 0 and not first):
        dir *= -1
        first = False
    

    start = int(round(time.time() * 1000))
    string_ = str(arr_pos[cnt]) + "x" + str(arr_speed_int[cnt]) + "n"
    serialPort.write(str.encode(string_))
    
    time.sleep(time_div / 1000.0)   # 1000 / 1000 = 1s
    
    stop = int(round(time.time() * 1000))
    print("pos: " + str(arr_pos[cnt]) + " spe: " + str(arr_speed_int[cnt]) + " time: " + str(stop-start))
    cnt += dir
