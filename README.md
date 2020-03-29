# Simple 8085 Arduino System v1.0 ![enter image description here](https://github.com/bazsimarkus/Simple-8085-Arduino-System/raw/master/Docs/8085-icon.png)
**This is the most simple version of a fully working Intel 8085-based microprocessor system. With only 3 chips, and an Arduino, the circuit can be built on a breadboard. Due to the limited number of Arduino pins, only 64 bytes of memory can be addressed, but it's enough for simple Assembly programs like adding numbers, and so on.**

Created by Balázs Márkus in 2018

*During the creation of this project, the main purpose was to keep the number of components and the amount of programming as low as possible, so even a beginner can wire up the circuit on a solderless breadboard and understand how a microprocessor system works. The following versions will contain more complex elements but this version (1.0) is directly a bare minimum, fully working system with only 4 chips.*

## Breadboard photo

![enter image description here](https://github.com/bazsimarkus/Simple-8085-Arduino-System/raw/master/Docs/8085-Arduino-breadboard.jpg)

## Components needed

 - Intel 8085 microprocessor
 - 74LS373N octal latch for address latching
 - 62256 static RAM as a memory unit
 - Arduino (Uno or Nano, both works)
 - 6MHz crystal
 - 3x 470Ohm resistors
 - 3x 10K resistors
 - 1x pushbutton

> This is the bare minimum amount of components in a 8085 system. We could leave the Arduino out, but in that case we should replace it with an EEPROM or EPROM to store the program we'd like to execute.

## Using

 1. Wire up the circuit based on schematics
 2. Upload the S-8085.ino source code to the Arduino board
 3. Open up Serial Monitor (baud 9600)
 4. Enjoy!

After wiring up the circuit, and uploading the code to the Arduino, open up a serial terminal with 9600 baud speed, and see the results of the program execution.
First, the Arduino initializes the RAM with zero values, then it fills up the RAM with our assembled 8085 code.
> You can find an online assembler to transform your 8085 Assembly code into uploadable machine code here: https://www.asm80.com/

After the program is successfully loaded into the memory, the Arduino goes sleeping, and lets the microprocessor work, and do its stuff. After a few seconds, the Arduino comes back and pauses the 8085 with the HOLD sign, so that the Arduino can read out the RAM, and see what's happened. If everything worked well, the result of the operation (in our case the sum of 2 and 3=5, that's 101b in binary) should appeard on memory address 32 (20h in hexadecimal).

If you want to do the operation again, press the microprocessor's RESET button then press the Arduino's reset button. **If you don't press the 8085's RESET before resetting the Arduino, the 8085 will stay in HALT mode, and the operation will not happen!**

Feel free to change the RAM code and try out what happens!

## Arduino Serial terminal screenshot
![enter image description here](https://github.com/bazsimarkus/Simple-8085-Arduino-System/raw/master/Docs/Serial-port-screenshot.png)
As the picture shows, the Arduino first writes the program in the RAM memory, then lets the CPU works, then reads out the memory and prints the memory table to the serial port.

## Schematic
![enter image description here](https://github.com/bazsimarkus/Simple-8085-Arduino-System/raw/master/Schematic/Schematic.png)

## Control and data signs, Arduino pins
The Arduino uses its pins for the following purposes:

 - D2-D9: the 8-bit data we'd like to write in the RAM in binary format. This is the so-called Data bus.
 - D10-D13 and A0-A1: the address where we would like to write in 6-bit binary format (we could have more memory, but unfortunately we ran out of Arduino pins, so only 2^6=64 bytes can be addressed) This is the so-called Address bus.
 - A2: Chip Select Negated: This pin signs the RAM chip when the Arduino is talking to it.
 - A3: Write Enable Negated: This pin prevents the RAM from being written, so when we're reading out data from the RAM, we won't overwrite it.
 - A4: Output Enable Negated: This pin disables the output pins of the RAM so we can write data using the data pins of the RAM.
 - A5: HOLD: It is connected to the 8085 microprocessor's HOLD pin. When this pin is high, the 8085 stops working, and puts its pins in High-Z mode so we can do stuff with the Arduino without interfering with the microprocessor.

## Functions
There are two main functions in the Arduino software, both of them are based on the Hitachi HM62256B static RAM datasheet's timing diagrams:

 - **void writeRam(int address, int data):** The function gets an address and a data as a parameter and writes the given data to a specific memory address. It selects the RAM with the control signs, then writes the desired address on the address bus, then writes the given data on the data bus. After that, it enables writing with the Write Enable sign, so the data gets stored in the memory on the given address. After writing the RAM it sets back every control sign, so the function can be called infinite times. When writing to the pins, we use Arduino's built-in bitRead function, so that we can convert the given data and address to binary, and choose any of its bits (1 or 0, HIGH or LOW) 

 - **void readRam(int address):**  The function gets an address as a parameter and reads the content of the memory on that address. It selects the RAM with the control signs, then writes the desired address on the address bus, then reads data that appeared on the output of the RAM. Before that it enables reading with the Output Enable sign, so the data appears on the output of the RAM. After reading it sets back every control sign, so the function can be called infinite times. When writing to the pins, we use Arduino's built-in bitRead function, so that we can convert the given data and address to binary, and choose any of its bits (1 or 0, HIGH or LOW)

## Assembly test code
**The test code add two numbers, and writes the result in the memory on address 0x20h.**

        ;Addtwonumbers
        ;Simple x85 assembly program to test the proper working of a 8085 microprocessor system
        ;written by Balázs Márkus in 2018
        
        MVI B, 03h	; Move 03h to B register
        MOV A, B	; Move the content of B register to the Accumulator
        MVI B, 02h      ; Move 02h to B register
        ADD B		; Add the value of register B to A
        STA 20h		; Store the value of the Accumulator at address 20h
 If you want to write your own programs, compile any Assembly code with an 8085 assembler like ASM80 (https://www.asm80.com/), then copy the machine code bytes to the writeRam function.  
   
## Development
The project was developed using **Arduino IDE 1.6.9** (https://www.arduino.cc/) and the assembly code was simulated using **GNUSIM8085** (https://gnusim8085.github.io/).

## Future goals

 - adding serial interface
 - adding seven segment displays
 - attaching 1602 LCD
 - attaching keyboards
 - adding DIP switches for the adjustment of operands

## Possible errors and solutions

 - If there is random data written in the RAM, try resetting the Arduino, it usually helps, because during the upload random bits can be written accidentally in the RAM.
 - If the result doesn't appear on address 32, try to comment out/in this line:

        writeRam(0x08, 0x76); // HALT OPCODE, doesn't needed in this case, but can be useful
      
      It is the HLT (HALT) instruction so when commenting out, the microprocessor will do the operation forever (but the result will be the same forever though)

 - If there is no data written in the RAM, check the wiring and try to plug off/plug in the Arduino and/or the CPU 5V power supply pin.

## Resources
Every datasheet can be found under folder "Datasheets".


