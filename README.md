# Robot-arm-v2.0 🦾
An attempt to my Masters thesis

## What is my goal? 🎯

### Long story short ⌛️
I want to fix some issues which occured in my bachelor thesis. My bachelor thesis was an attemtp in designign my own robot-arm. I designed my own mechanical structure and built it, I also created electrical site of my robot and develop entire (almost working XD) environment for cooperating with robot and succesfully graduated. 

### Historical background 📜
Unfortunately there were some issues which made this project not as good as I wanted to. Main issue was usage of chinise parts, especially in motion-controll part of electrical structure. Driveres for stepper motors were working in closed loop. Due to source of those parts, they were not working as they suppose to. I had to tune PID controllers, but due to issues in communication between my PC and controllers I was not able to change anything becouse communication wasn't working. Becouse of wrong default values in PID controllers movement of my robot arm was far from perfect. There were a lot of overshooting at the begining and in the end of every move. Becouse of that I had to limit efector speed.

### How I want to fix everything? 🛠️
I want to create my own closed loop controllers for stepper motors. For this purpouse I've order open loop controller and my goal is to design hardware and software necessary for proper closed loop controll of every motor in my robot.

### Other changes I want to make 🆕
Also I want to redo entire software side of project. I want to be able of controlling robot via website, and use my Raspberry PI 4B 2Gb as brain of my robot. How it's all going to turn out? We will see.


## Work schedule: 📅
* creating my own system of closed-loop stepper motor control ✔️DONE✔️
  - creating test bench ✔️
  - creating bradboard circuit ✔️
  - turning breadboard into PCB ✔️
  - collecting all necessary components ✔️
  - waiting months for order on JLCPCB ✔️
  - soldering all components ✔️
  - realising that i've made a mistake ✔️
  - XD.exe ✔️
  - repairing shit ✔️
  - testing PID control ✔️
* developing communication between Raspberry pi 4B - Brain of entire system and motor controlling units - Raspberry pi PICO 
  - developing general idea of communication ✔️
  - do all necesarry speed/frequency/position/error calculations, ✔️
  - simple testing program for checking if it works, ✔️
  - implementing gamepad controll, ✔️
  - implementing inverse kinematics
  - <em>IN PROGRESS</em>
* developing software for motion controll (maybe via website)
* making it look good - some kind of enclosure


## Work progress 📈

### Test bench
![test-bench](https://user-images.githubusercontent.com/96399051/177652449-25893234-0528-44b8-856f-3d2c438a6627.png)

### Designed circuit
![Schematic_stepper-motor controller_2022-07-05-1](https://user-images.githubusercontent.com/96399051/177652500-a6ee2a78-83a3-4abc-b8a6-fbfa1ffb963f.png)

### Breadboard circuit
![breadboard circuit](https://user-images.githubusercontent.com/96399051/177652489-3f0df4c5-2719-41ff-b1b8-697fa5be17bf.jpg)

### Designed PCB
![PCB-rev-1 3](https://user-images.githubusercontent.com/96399051/177648128-81e2d887-7d0c-41ad-a370-b2cb5ed316cc.png)

### Collecting all components
![components](https://user-images.githubusercontent.com/96399051/178782339-38a9e303-52ff-405e-9f5b-0f3172e58be2.png)

### Recieving tiny package from JLC PCB
![IMG20220717200212](https://user-images.githubusercontent.com/96399051/179423982-54247350-67dc-4910-ad82-a00b74e1ac0e.jpg)
![IMG20220717200700](https://user-images.githubusercontent.com/96399051/179423987-50d1d0ac-7d39-4e72-b129-96932e67ab95.jpg)

### Soldering all components to PCB
![ready-PCB](https://user-images.githubusercontent.com/96399051/179424211-2a6c7309-6533-4897-8044-15ed67c86128.png)

### Testing stage (hope is going to be smooth)
 - Encoder readings are working ❌
 - Status LEDs are working ✔️
 - Stepper motor controll ✔️
 - end switches ✔️

### Realising that i've made a mistake
My encoder readings were good until I was trying to move rotor faster than 20 rpm. While moving it faster counters started loosing steps.
My first thought was that my programm whitch was written in python was to slow to maitain IRQ from encoder (which has 1000 ppr - quite a lot). 
Unfortunatelly when i wrote the same program in c/c++ same shit happend. Even after using of program delivered by Raspberry pi in examples programs (prorgam which was using PIO of my PICO, which were programmed in ASEMBLY language, this shit still didn't want to cooperate. At this moment I realised that it was hardware issue.

### XD.exe
Next photo shows output of optocoupler which was driven by differential encoder output.
![przed](https://user-images.githubusercontent.com/96399051/181518017-7adf4cc8-371d-4013-b149-62f21c12ca23.jpg)

As You can easli see, there is fucking big as fuck inertia. Time responses of my optocoupler acording to the datasheet were maximally around 20 μs. In the photo abowe falling edge was pretty quick but rising edge was around 250 μs wide. It means that it exceedes over 10 time max value from datasheet.

### Repairing shit
After some time and help of my colleagues we discovered that same situation is taking place with different optocouplers. rising edge still looked not good. Becouse of quite well known shape of rising edge - inertial behaviour we figured out that there must be some not wanted cappacity.

<em>How it turned out?</em>

By looking on my microcontroller datasheet I've found that internal pullup resistor was something like 50k - 100k (I couldn't found any proper values). If there was any cappacity, while discharging (rising edge of optocoupler) current from this cappacity had to flow via this big resistor. So to make it faster I decided to add external pullup resistor with drasticly lower value of 1k. 

<p align="center">
  <img width="500" src="https://user-images.githubusercontent.com/96399051/182685368-577f4c6d-a894-4835-a821-7c9ba84bb7b8.jpg">
</p>

Without any changes in code (still using internal pullup) previous inertia changed into something like this:

<p align="center">
  <img width="1000" src="https://user-images.githubusercontent.com/96399051/181521899-1e8561e3-d804-4944-85c5-8afe1a2b95c3.jpg">
</p>

As You can see it is quite smaller - around 10 μs. And now readings from encoder are perfect at least for this project (until some quite big speed - fixing this requires faster optocouplers).

### Testing stage vol.2
 - Encoder readings are working ✔️
 - Status LEDs are working ✔️
 - Stepper motor controll ✔️
 - end switches ✔️

### Testing control algorithms
By analysing different approaches to controll stepper motor in closed loop I have found out that the best results were effect of using type P regulator. But first things first.

#### Hysteresis controll
- <em>Idea -</em> At first try, I have made hysteresis controll algorithm, which utilizes fixed speed of rotor and setting proper direction by sign of error position value. Due to limitation of my microcontroller I am able to reach interrupts of frequency near to 25 kHz. Generating square wave requires at least 2 interrupts. Because of this I am reaching square wave of max 12.5 kHz. It means that my max speed's (varying of different settings on stepper motor power controller which can do full revolution in 200 - 20 000 steps  - my choice is 10 000 steps) are somewhere at 75 rpm. It is not that much. But in this special application, thick kind of speed is perfect (at list I hope so). 

<p align="center">
  <img width="1000" src="https://user-images.githubusercontent.com/96399051/182342252-c4d15834-c4f1-4b06-b740-944580dfc92b.jpg">
</p>

- <em>Issues</em> My hysteresis way of controll stepper motor caused a lot of vibrations which were caused by oscillating around setpoint due to too large speed near setpoint. I was able to get rid of this issue by increasing hysteresis value. Because of increased hysteresis I have lost resolution of my movements. With max speed I have had to set hysteresis to 0.25% of full revolution. 

#### P type regulator
- <em>Idea -</em> At this point I decided to implement proper regulator - only P type. One of downsides of implementing more calculations in my interrupts was increasing duration time of interrupt cycle. This is pretty unwanted behaviour, but there is no other way. Varying duration time of interrupt cycle fortunately does not affect P type regulator. Only time based calculations like in PID/PI/PD regulators may be affected by this. Iny approach I abandoned idea of counting steps and basing on them properly controlling motor position. I am more likely controlling it by speed (with discrete speed values). Speed is proportional to frequency of square wave generated by interrupts. Because of first level inertia speed characteristics I decided to utilize P type regulator. With given controll loop model:

<p align="center">
  <img width="1000" src="https://user-images.githubusercontent.com/96399051/182338735-32e1ed78-0daf-421e-ab7e-89b710a62dbc.jpg">
</p>

In this approach I decided to set a few important values which determine error-speed characteristic in way that make it possible for microcontroller to handle. By this I mean lower and upper limitations. These values can ba adjusted to improve stability with different loads - different stages of robot.

<p align="center">
  <img width="1000" src="https://user-images.githubusercontent.com/96399051/183686434-0e95f153-8f3b-4f30-a677-0e85a73da3e2.jpg">
</p>

- <em>Advantages</em> Basing on created characteristic I was able to achieve movements which were deprived of any shaky movements. At the end of every moves there is always decreasing tendency in error value so also speed is decreasing. By this rotor gently goes to setpoint. And I am able to get exact position without any error at the end. So this is big win for P type regulator.

#### Why not PID regulator?
Well.. I see two major disadvantages. In classis analogue control theory using integration of error leads to disposal of any offense. This behaviour is achived by fact that when controlled object opposes to control signal which is proportional to error, and this signal is not enough to push object to setpoint. Integration of offense increases the control signal over time and thus pushes object to desired position. However when it comes to discrete system like stepper motor, where every step is deterministic and the ony way of control is to make a proper number of steps by controlling it with square waveform. So in this example I can assume that my speed controll loop is working well with very little inertia. Assuming that I would use PI type regulator, I would have to deal with constantly increasing value of my controll signal when my motor would try to follow desired speed. It will work great while moving but when approaching to the setpoint I would get pretty big overshoot due to stored value in my integral. It would lead to oscilations around my setpoint and this is pretty unwalnted behaviour.

Second disadvantage is increased cycle time of my IRQ becouse of need to constantly calculating integral with trapezoidal method. Which requires measuring time between current and las IRQ. Which will lead to decrease in speed of my motor.

## Developing communication between Raspberry pi 4B - brain of entire system and motor controlling units - Raspberry pi PICOs

### Developing general idea of communication
I want to use microUSB port of my PICOs. Luckly there is library <stdio.h> which allows for this type commuinication. In my approach i decided to send to microcontroller two basic informations:
* position - target position for microcontroller in which I want the rotor to be,
* speed - more specificly it is max angular velocity in which the rotor should spin

Becouse of my decision to stay with 12-bit resolution of position controll (allmost 12-bit actually it's 4000 states/revolution which means 0,09° accuracy) if I want to make full revolution in 1 second, I need to make certain number of steps in one second and this means that I need to drive my motor with signal of X Hz, where X is number of steps necesarry to make full revolution. So to move rotor from position 0 to position 4000, with speed of 1 rev/s I have to send this two informations to microcontroller.

### Do all necesarry speed/frequency/position/error calculations
At this stage I need to teach my microcontroller how to understeand and interpret commands from master and what to do with them. My idea was to send information in format of:
<p align="center">
    <em>position</em> x <em>speed</em> n
</p>

Where position is any positive integer and speed is also any positive integer but in this case speed doesn't mean exact angular velocity but desired waveform frequency. Microcontroller sets position value as target via simple smoothing function. Then it calculates maximal error and minimal duty cycle which correspond to read frequency. Basing on this values it changes controll signalfrequency - error characteristics into given shape:

<p align="center">
  <img width="1000" src="https://user-images.githubusercontent.com/96399051/183921153-0078f84d-379a-473f-a26e-2afb63866919.jpg">
</p>

### Implementing gamepad controll
in this part I want to make use of gamepad as one of the ways to controll robot. In this case I want it to be able to controll robot efector position in X, Y, Z, epsilon coordinates. For this purpouse I want the analogue sticks to be interface. Additionally I want to use triggers as Dead Man Switch to add more safety. For this purpose I used <em>inputs</em> library, and write simple script utilising multithreading for asynchronous code execution - cyclic sending position and speed to microcontroller. Works quite well.

<p align="center">
  <img width="700" src="https://user-images.githubusercontent.com/96399051/184115663-e32a59c7-d5dc-4362-ab84-686d0e243441.jpg">
</p>

## To do
* try to run master.py from linux - will be easier / adapt for both platforms (win10/11 and linux)
* update pcb schematic with proper Pull-up's
* implement inverse kinematics calculation


## Side ideas
* make it work as G-code - let the inverse kinematics caluculate entire path, but uC will recieve target points with desired speed and accelerations. It will make communication bus simpler and will not relay entirely on communication during every move.
* use pyGame as simple gui
* try to implement calculating inverse speed kinematisc
