from gamepad_lib import Gamepad
from threading import Timer 
import time
import serial


# serial port object
serialPort = serial.Serial(port="COM7", baudrate=115200,
                           bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)

# serial string
serialString = ""

interval = 20
step = 10

position = 0
speed = int(step * interval * 1.25)

# gamepad object
gamepad = Gamepad()

# repeating timer
class RepeatTimer(Timer):
    """class maitaining repeating timer"""  
    def run(self):  
        while not self.finished.wait(self.interval):  
            self.function(*self.args,**self.kwargs)  

# sending info to uC
def cyclic_timer_function():
    # print(gamepad.R_analog_x)
    global position
    global speed
    global step

    # geting read from gamepad
    value = gamepad.R_analog_x
    
    # changing step value
    step = int(abs(value) / 1000)

    # changing speed value
    speed = int(step * interval * 1.25)
    
    if value > 1000:
        position += step
    elif value < -1000:
        if position >= step:
            position -= step

    string_ = str(position) + "x" + str(speed) + "n"
    serialPort.write(str.encode(string_))

timer = RepeatTimer(1/(2 * interval),cyclic_timer_function)
timer.start() #recalling run  
print("starting gamepad data acquisition")  
  
while 1:
    gamepad.update()
 
