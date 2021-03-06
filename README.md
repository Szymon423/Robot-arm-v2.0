# Robot-arm-v2.0 π¦Ύ
An attempt to my Masters thesis

## What is my goal? π―

### Long story short βοΈ
I want to fix some issues which occured in my bachelor thesis. My bachelor thesis was an attemtp in designign my own robot-arm. I designed my own mechanical structure and built it, I also created electrical site of my robot and develop entire (almost working XD) environment for cooperating with robot and succesfully graduated. 

### Historical background π
Unfortunately there were some issues which made this project not as good as I wanted to. Main issue was usage of chinise parts, especially in motion-controll part of electrical structure. Driveres for stepper motors were working in closed loop. Due to source of those parts, they were not working as they suppose to. I had to tune PID controllers, but due to issues in communication between my PC and controllers I was not able to change anything becouse communication wasn't working. Becouse of wrong default values in PID controllers movement of my robot arm was far from perfect. There were a lot of overshooting at the begining and in the end of every move. Becouse of that I had to limit efector speed.

### How I want to fix everything? π οΈ
I want to create my own closed loop controllers for stepper motors. For this purpouse I've order open loop controller and my goal is to design hardware and software necessary for proper closed loop controll of every motor in my robot.

### Other changes I want to make π
Also I want to redo entire software side of project. I want to be able of controlling robot via website, and use my Raspberry PI 4B 2Gb as brain of my robot. How it's all going to turn out? We will see.


## Work schedule: π
* creating my own system of closed-loop stepper motor control βοΈDONEβοΈ
  - creating test bench βοΈ
  - creating bradboard circuit βοΈ
  - turning breadboard into PCB βοΈ
  - collecting all necessary components βοΈ
  - waiting months for order on JLCPCB βοΈ
  - soldering all components βοΈ
  - realising that i've made a mistake βοΈ
  - XD.exe βοΈ
  - repairing shit βοΈ
  - testing PID control βοΈ
* developing communication between Raspberry pi 4B - Brain of entire system and motor controlling units - Raspberry pi PICO <em>~IN PROGRESS~</em>
* developing software for motion controll (maybe via website)
* making it look good - some kind of enclosure


## Work progress π

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
 - Encoder readings are working β
 - Status LEDs are working βοΈ
 - Stepper motor controll βοΈ
 - end switches βοΈ

### Realising that i've made a mistake
My encoder readings were good until I was trying to move rotor faster than 20 rpm. While moving it faster counters started loosing steps.
My first thought was that my programm whitch was written in python was to slow to maitain IRQ from encoder (which has 1000 ppr - quite a lot). 
Unfortunatelly when i wrote the same program in c/c++ same shit happend. Even after using of program delivered by Raspberry pi in examples programs (prorgam which was using PIO of my PICO, which were programmed in ASEMBLY language, this shit still didn't want to cooperate. At this moment I realised that it was hardware issue.

### XD.exe
Next photo shows output of optocoupler which was driven by differential encoder output.
![przed](https://user-images.githubusercontent.com/96399051/181518017-7adf4cc8-371d-4013-b149-62f21c12ca23.jpg)

As You can easli see, there is fucking big as fuck inertia. Time responses of my optocoupler acording to the datasheet were maximally around 20 ΞΌs. In the photo abowe falling edge was pretty quick but rising edge was around 250 ΞΌs wide. It means that it exceedes over 10 time max value from datasheet.

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

As You can see it is quite smaller - around 10 ΞΌs. And now readings from encoder are perfect at least for this project (until some quite big speed - fixing this requires faster optocouplers).

### Testing stage vol.2
 - Encoder readings are working βοΈ
 - Status LEDs are working βοΈ
 - Stepper motor controll βοΈ
 - end switches βοΈ

### Testing control algorithms
By analysing different approaches to controll stepper motor in closed loop I have found out that the best results were effect of using type P regulator. But first things first.

#### Hysteresis controll
- <em>Idea -</em> At first try, I have made hysteresis controll algorithm, which utilizes fixed speed of rotor and setting proper direction by sign of error position value. Due to limitation of my microcontroller I am able to reach interrupts of frequency near to 25 kHz. Generating square wave requires at least 2 interrupts. Because of this I am reaching square wave of max 12 kHz. It means that my max speed's (varying of different settings on stepper motor power controller which can do full revolution in 200 - 20 000 steps  - my choice is 10 000 steps) are somewhere at 60 rpm. It is not that much. But in this special application, thick kind of speed is perfect (at list I hope so). 

<p align="center">
  <img width="700" src="https://user-images.githubusercontent.com/96399051/182342252-c4d15834-c4f1-4b06-b740-944580dfc92b.jpg">
</p>

- <em>Issues</em> My hysteresis way of controll stepper motor caused a lot of vibrations which were caused by oscillating around setpoint due to too large speed near setpoint. I was able to get rid of this issue by increasing hysteresis value. Because of increased hysteresis I have lost resolution of my movements. With max speed I have had to set hysteresis to 0.25% of full revolution. 

#### P type regulator
- <em>Idea -</em> At this point I decided to implement proper regulator - only P type. One of downsides of implementing more calculations in my interrupts was increasing duration time of interrupt cycle. This is pretty unwanted behaviour, but there is no other way. Varying duration time of interrupt cycle fortunately does not affect P type regulator. Only time based calculations like in PID/PI/PD regulators may be affected by this. Iny approach I abandoned idea of counting steps and basing on them properly controlling motor position. I am more likely controlling it by speed (with discrete speed values). Speed is proportional to frequency of square wave generated by interrupts. Because of first level inertia speed characteristics I decided to utilize P type regulator. With given controll loop model:

<p align="center">
  <img width="700" src="https://user-images.githubusercontent.com/96399051/182338735-32e1ed78-0daf-421e-ab7e-89b710a62dbc.jpg">
</p>

In this approach I decided to set a few important values which determine error-speed characteristic in way that make it possible for microcontroller to handle. By this I mean lower and upper limitations. These values can ba adjusted to improve stability with different loads - different stages of robot.

<p align="center">
  <img width="700" src="https://user-images.githubusercontent.com/96399051/182684876-36ce1fbf-8fc3-45dc-a4c2-4f8b7b15adc4.jpg">
</p>

- <em>Advantages</em> Basing on created characteristic I was able to achieve movements which were deprived of any shaky movements. At the end of every moves there is always decreasing tendency in error value so also speed is decreasing. By this rotor gently goes to setpoint. And I am able to get exact position without any error at the end. So this is big win for P type regulator.

#### Why not PID regulator?
Well.. I see two major disadvantages. In classis analogue control theory using integration of error leads to disposal of any offense. This behaviour is achived by fact that when controlled object opposes to control signal which is proportional to error, and this signal is not enough to push object to setpoint. Integration of offense increases the control signal over time and thus pushes object to desired position. However when it comes to discrete system like stepper motor, where every step is deterministic and the ony way of control is to make a proper number of steps by controlling it with square waveform. So in this example I can assume that my speed controll loop is working well with very little inertia. Assuming that I would use PI type regulator, I would have to deal with constantly increasing value of my controll signal when my motor would try to follow desired speed. It will work great while moving but when approaching to the setpoint I would get pretty big overshoot due to stored value in my integral. It would lead to oscilations around my setpoint and this is pretty unwalnted behaviour.

Second disadvantage is increased cycle time of my IRQ becouse of need to constantly calculating integral with trapezoidal method. Which requires measuring time between current and las IRQ. Which will lead to decrease in speed of my motor.

### Developing communication between Raspberry pi 4B - brain of entire system and motor controlling units - Raspberry pi PICOs
To be continiued...
