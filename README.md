# Prosthetic-Hand-Game

This project consists of three main components: the mechanical, electrical and software designs of an articulating hand that plays Rock-Paper-Scissors with the user. The hand also has the ability to control the fingers individually (the ring and pinky finger move together).

# Mechanical Design

The mechanical design was based on a normal human hand. The file was modified from an existing design by grossrc on Thingiverse (https://www.thingiverse.com/thing:1691704). Modifications were done by removing the supports that were originally in the design for the circuits. This was done to allow for a change to the arm portion which held the 4 servo-motors(DS939MG  -Digital Servo) used in this design. The 3D printed hands can be seen below.

![hand front](https://github.com/AI-Bobby/Robotic-Hand-Game/assets/112420196/1992f99b-942c-4936-b25d-1e69c21921c3)
![hand back](https://github.com/AI-Bobby/Robotic-Hand-Game/assets/112420196/419f0fc9-ea79-40fd-90aa-86dc851bd038)

The fingers are articulated by 2 things, the fishing line attached to the servo-motors, and the nylon string running through the fingers and attached to the hand. The nylon string acts like a tendon that holds the finger in place when not in use. The fishing line is pulled by the servo-motor when it rotates to cause a "muscle" contraction causeing the finger(s) to close. The servo-motors were positioned to ensure that there would be no collission when the fingers are actuating. The position and the connections of the servo-motors can be seen in the image below. tre

![motors](https://github.com/AI-Bobby/Robotic-Hand-Game/assets/112420196/300b0739-e6c4-4f28-8ddf-f570c16d11d3)


# Electrical Design

For the electrical design of the design a Arduino Uno was used as the microcontroller to run the processes for controlling the hand. The Arduino Uno only has 13 I/O digital pins and 6 analog pins. Based on the design there would be a total of 6 buttons, 6 LEDs, 4 servo-motors, a potentiometer, a multiplexer(CD74HC4051E), and a shift-register(SN74AHC595). Based on these components 16 digital pins would have been needed so the multiplexor and the shift-register were added which reduces the number of pins needed. Now the number of digital pins is reduced to 11 digital pins with the shift-register being used for the LEDs and the multiplexor being used for the buttons. The systems wiring diagram can be seen below.

![Final Schematic](https://github.com/AI-Bobby/Robotic-Hand-Game/assets/112420196/9ab858dc-2fb6-4ecf-b1f0-eebdd740ee9a)

The servo-motors can be seen to be attached to their own separate 9V battery to allow for there to be no current drop if all four of the servo-motors are active at the same time. This schematic also shows how the multiplexer and the shift-register are wired to help reduce the number of input wires to the Arduino Uno. The multiplexer has 6 buttons each with their own respective function attached that until pressed is reading low. Once the button is pressed the multiplexer will read the value of the button as high.The buttons have pull-down resistors attached to limit current going into the Arduino and to allow for when the button is pressed the Arduino will read high. 


# Software Design

The software design for this system is a simplistic streamlined process to allow for reduced memory usage and ease of use. The file XXX.h contains the commented code that has the specific aspects to the code. The code uses the use of enumeration to select cases for the multiple switch cases determined by the buttons. Within the code the shift register is used to shift binary strings representing the locations of each of the LEDs to light up based on the specific function being performed. The multiplexer is referenced each loop of the Arduino code and checks each LED to see if a button has been pressed. The pressing of a button influences the specific binary string that is then sent to the shift register to change the LEDs that are lit.

The code consists of 2 modes and 5 functions: Game, Control, setup, loop, gameMove, updateSystem, and shifter. Each function has its respective repetitive functionality. The mode Game allows for the mode button and the random selection of the hand shape button to be pressed. The hand selection uses the microcontrollers clock to get a random seed on each loop so that the selection of the hand shape is completely random. In the Control mode the individual fingers are controlled based on a mapping of the potentiometers analog input and the angles of each servo-motors respectively.  Setup sets the needed pins to there respective I/O setting. Loop checks the button to see if one has been pressed as well as when in the control mode allows for the movement of the fingers based on the reading from the potentiometer mapped to the angles of the servo-motors. The gameMove function randomly chooses the shape for the hand to be performed by the hand and also reset the hand if changed from the control mode to the game mode. The updateSystem function handles the different functionalities based on the button that was pressed. Shifter is a function that sends the binary string to the shift register to change the LED output. 
