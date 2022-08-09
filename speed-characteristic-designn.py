from operator import truediv
import matplotlib.pyplot as plt
import numpy as np


t_max = 1000.0
t_min = 1.0

e_max = 1000.0
e_min = 3.0

# b = (t_min * e_max - t_max * e_min) / (t_max - t_min)
# a = t_max * (e_min - b)

# c = -0.1  #duty cycle f(1000) =  0.088 good: 0.08      a:  1097.9075775775777  b:  1.9022022022022023  c:  -0.1
c = -0.1
b = (t_min * e_max - t_max * e_min + c * (e_min - e_max)) / (t_min - t_max)
a = (t_min - c) * (e_max - b)

print("a: ", a, " b: ", b, " c: ", c)

time_base = 40  # 40us - duty cykle of interrupt -> min 80us for proper pulse for time_cnt = 1
time_cnt = 1    

error = np.linspace(3, 1000, 998, dtype=int)
time = a / (error - b) + c

time_sub = np.asarray(time * 2 * time_base, dtype=int)  # times 2 bcoz of need of two edges rising and falling for proper step
time_cnt_arr = np.array(time_sub / 1000.0, dtype=float) # div by 1000.0 us -> ms

plt.figure(1)
plt.plot(error, time_cnt_arr)
plt.grid()
plt.xlabel("error value")
plt.ylabel("duty cycle [ms]")
plt.title("Duty cycle")
# plt.xlim([3, 1000])
print("duty cycle f(1000) = ", time_cnt_arr[-1], "good: 0.08")
print("duty cycle f(3) = ", time_cnt_arr[0], "good: 80.0")

## frequency plot
freq = 1 / time_cnt_arr

plt.figure(2)
plt.plot(error, freq)
plt.grid()
plt.xlabel("error value")
plt.ylabel("frequency [kHz]")
plt.title("Control signal frequency")
# plt.xlim([3, 1000])
print("frequency f(1000) = ", freq[-1], "good: 12.5")

plt.show()
