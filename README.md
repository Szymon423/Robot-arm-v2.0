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
* creating my own system of closed-loop stepper motor control
  - creating test bench ✔️
  - creating bradboard circuit ✔️
  - turning breadboard into PCB ✔️
  - collecting all necessary components ✔️
  - waiting months for order on JLCPCB ✔️
  - soldering all components 2/4
  - realising that i've made a mistake (hope I didn't)
  - XD.exe
  - repairing shit
  - testing PID control (only P will work better?)
* developing communication between Raspberry pi 4B - Brain of entire system and motor controlling units - Raspberry pi PICO
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
Unfortunatelly when i wrote the same program in c/c++ same shit happend. Even after using of program delivered by Raspberry pi in examples programs (prorgam which was using PIO of my PICO, which were programmed in ASEMBLY language, this shit still didn't want to cooperate. At this moment I realised that it was hardware issue. Next photo shows output of optocoupler which was driven by differential encoder output.
![przed](https://user-images.githubusercontent.com/96399051/181518017-7adf4cc8-371d-4013-b149-62f21c12ca23.jpg)


### XD.exe

### Repairing shit

