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
* developing communication between Raspberry pi 4B - Brain of entire system and motor controlling units - Raspberry pi PICO <em>~IN PROGRESS~</em>
* developing software for motion controll (maybe via website)


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

Without any changes in code (still using internal pullup) previous inertia changed into something like this:
![po](https://user-images.githubusercontent.com/96399051/181521899-1e8561e3-d804-4944-85c5-8afe1a2b95c3.jpg)

As You can see it is quite smaller - around 10 μs. And now readings from encoder are perfect at least for this project (until some quite big speed - fixing this requires faster optocouplers).

### Testing stage vol.2
 - Encoder readings are working ✔️
 - Status LEDs are working ✔️
 - Stepper motor controll ✔️
 - end switches ✔️

### Testing control algorithms
By analysing different approaches to controll stepper motor in closed loop I have found out that the best results were effect of using type P regulator. But first things first.

#### Hysteresis controll
- <em>Idea -</em> At first try, I have made hysteresis controll algorithm, which utilizes fixed speed of rotor and setting proper direction by sign of error position value. Due to limitation of my microcontroller I am able to reach interrupts of frequency near to 25 kHz. Generating square wave requires at least 2 interrupts. Because of this I am reaching square wave of max 12 kHz. It means that my max speed's (varying of different settings on stepper motor power controller which can do full revolution in 200 - 20 000 steps  - my choice is 10 000 steps) are somewhere at 60 rpm. It is not that much. But in this special application, thick kind of speed is perfect (at list I hope so). 
- <em>Issues</em> My hysteresis way of controll stepper motor caused a lot of vibrations which were caused by oscillating around setpoint due to too large speed near setpoint. I was able to get rid of this issue by increasing hysteresis value. Because of increased hysteresis I have lost resolution of my movements. With max speed I have had to set hysteresis to 0.25% of full revolution. 

#### P type regulator
- <em>Idea -</em> At this point I decided to implement proper regulator - only P type. One of downsides of implementing more calculations in my interrupts was increasing duration time of interrupt cycle. This is pretty unwanted behaviour, but there is no other way. Varying duration time of interrupt cycle fortunately does not affect P type regulator. Only time based calculations like in PID/PI/PD regulators may be affected by this. Iny approach I abandoned idea of counting steps and basing on them properly controlling motor position. I am more likely controlling it by speed (with discrete speed values). Speed is proportional to square wave generated by interrupts. Because of first level inertia speed characteristics I decided to utilize P type regulator. I will explain later why I haven't choose PI or PID controller. In regulator based approach I decided set a few important values which determine error-speed characteristic.

![test](https://user-images.githubusercontent.com/96399051/182250392-2f9c9feb-fdb3-4b74-9c44-8b268a21c03b.png)

- <em>Advantages</em> Basing on created characteristic I was able to achieve movements which were deprived of any shaky movements. At the end of every moves there is always decreasing tendency in error value so also speed is decreasing. By this rotor gently goes to setpoint. And I am able to get exact position without any error at the end. So this is big win for P type regulator.

#### TO DO: Przeprowadzić pomiary i zebrać dane, na ich podstawie wrzucić wykresy

