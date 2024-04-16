# ECE 4180 Final Project: Hybrid Robo Guide Dog
### By Emmanuel Etukudoh, Tawhid Ahmad, Byron Pritchett

## Description: 
Our goal with this project will be adding aditional functionality to the robot kit available in the lab to make it into a bluetooth controlled guiding robot. We already knew how to interface with the motors of the robot, but for this design project we had to figure out how to add object avoidance, a remote control mode, visual and auditory indicators to the existing robot frame. For object avoidance, we utilized the sonar sensor (HC-SR04) from Lab 3 and fine tuned it to allow for avoidance of a wide variety of objects. When toggled, this modes runs continuously as the robot explores its environment, avoiding objects when they are 250 mm away. We also used the Adafruit Bluetooth LE UART module for the remote control aspect of the robot, as we used the provided controller within the UART app, to manually control the movement of the robot. Both the uLCD and speaker indicate the robot's current mode.

## Equipment:
* mBED
* 4DGL-uLCD-SE
* MicroSD Breakout Board
* Breadboard Mount Speaker
* HC-SR04 Sonar Sensor
* Adafruit Bluetooth LE UART
* ECE 4180 Robot Kit
* DC Motors

## Connections:


## Button Mappings:
<img width="482" alt="image" src="https://github.com/emmanuel-et/mbedHybridRoboGuideDog/assets/115197111/76860d23-350b-44e5-b7e7-139218079771">


## Demo:


## Code:
In order to utilize the code you will have to clone this repositiory into Kiel Studio and then compile onto the mBED. 

Steps for cloning:
Kiel Studio -> File -> Clone -> Input repository URL 

## References:
* https://os.mbed.com/users/4180_1/notebook/sparkfun-magician-robot-base-kit/
* https://os.mbed.com/users/4180_1/notebook/using-the-hc-sr04-sonar-sensor/
* https://os.mbed.com/components/Adafruit-Bluefruit-LE-UART-Friend/
