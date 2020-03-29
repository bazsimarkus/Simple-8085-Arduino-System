/* 
 *  S-8085
 *  Simple 4-chip Intel 8085 system with RAM writing and program execution with Arduino control
 *   
 *  written by Balázs Márkus in 2018
 *  
 *  The program is the base of a fully working Intel 8085-based system. The Arduino is used to load the program in the RAM attached to the i8085,
 *  then the Arduino "goes sleeping", while the CPU is working. After that, the Arduino "wakes up" and takes over control again, so we can read the results
 *  of the CPU operations.
 *  
 *  The whole system contains 3 chips + an Arduino, every resource and schematics are attached.
 */

// Defining every control sign, to we can control pins in DigitalWrite functions like the actual control bus.

//Data bus:
#define D0 2
#define D1 3
#define D2 4
#define D3 5
#define D4 6
#define D5 7
#define D6 8
#define D7 9

// Address bus: (Due to the limited numbers of Arduino Uno pins, only 64 bytes of memory can be addressed)
#define AD0 10
#define AD1 11
#define AD2 12
#define AD3 13
#define AD4 A0
#define AD5 A1
//#define AD6 A2 //in case of using MEGA
//#define AD7 A3 //in case of using MEGA

//Control bus:
#define CSN A2
#define WEN A3
#define OEN A4

//Hold sign for the CPU to sign when it can run or when it should stop:
#define HOLD A5

/*  writeRam function
 *  based on Hitachi HM62256B datasheet
 *  
 *  The function gets an address and a data as a parameter and writes the given data to a specific memory address. 
 *  
 *  It selects the RAM with the control signs, then writes the desired address on the address bus, then writes the given data on the data bus.
 *  After that, it enables writing with the Write Enable sign, so the data gets stored in the memory on the given address.
 *  
 *  After writing the RAM it sets back every control sign, so the function can be called infinite times.
 *  
 *  When writing to the pins, we use Arduino's built-in bitRead function, so that we can convert the given data and address to binary, and choose any of its bits (1 or 0, HIGH or LOW) 
 */
void writeRam(int address, int data)
{
    delay(5);

    setAddressBusToOutput();
    setDataBusToOutput();
    setControlsToOutput();

    delay(5);

    digitalWrite(CSN, LOW);
    digitalWrite(WEN, HIGH);
    digitalWrite(OEN, HIGH);

    delay(5);

    digitalWrite(AD0, bitRead(address, 0));
    digitalWrite(AD1, bitRead(address, 1));
    digitalWrite(AD2, bitRead(address, 2));
    digitalWrite(AD3, bitRead(address, 3));
    digitalWrite(AD4, bitRead(address, 4));
    digitalWrite(AD5, bitRead(address, 5));
    //digitalWrite(AD6,bitRead(address,6));
    //digitalWrite(AD7,bitRead(address,7));

    delay(5);

    digitalWrite(D0, bitRead(data, 0));
    digitalWrite(D1, bitRead(data, 1));
    digitalWrite(D2, bitRead(data, 2));
    digitalWrite(D3, bitRead(data, 3));
    digitalWrite(D4, bitRead(data, 4));
    digitalWrite(D5, bitRead(data, 5));
    digitalWrite(D6, bitRead(data, 6));
    digitalWrite(D7, bitRead(data, 7));

    delay(5);

    digitalWrite(WEN, LOW);

    delay(5);

    digitalWrite(WEN, HIGH);

    delay(5);

    //Eredetileg itt a végén volt egy OEN low ittmaradva, de emiatt csak az első byte kiolvasása ment, valamiért megőrült a RAM-ba írás, ezért kiszedtem, most jó!
}

/*  readRam function
 *  based on Hitachi HM62256B datasheet
 *  
 *  The function gets an address as a parameter and reads the content of the memory on that address. 
 *  
 *  It selects the RAM with the control signs, then writes the desired address on the address bus, then reads data that appeared on the output of the RAM.
 *  Before that it enables reading with the Output Enable sign, so the data appears on the output of the RAM.
 *  
 *  After reading it sets back every control sign, so the function can be called infinite times.
 *  
 *  When writing to the pins, we use Arduino's built-in bitRead function, so that we can convert the given data and address to binary, and choose any of its bits (1 or 0, HIGH or LOW) 
 */
void readRam(int address)
{

    int inputData[8];

    delay(5);

    setDataBusToInput();
    setAddressBusToOutput();
    setControlsToOutput();

    delay(5);

    digitalWrite(CSN, LOW);
    digitalWrite(WEN, HIGH);
    digitalWrite(OEN, HIGH);

    delay(5);

    digitalWrite(AD0, bitRead(address, 0));
    digitalWrite(AD1, bitRead(address, 1));
    digitalWrite(AD2, bitRead(address, 2));
    digitalWrite(AD3, bitRead(address, 3));
    digitalWrite(AD4, bitRead(address, 4));
    digitalWrite(AD5, bitRead(address, 5));
    //digitalWrite(AD6,bitRead(address,6));
    //digitalWrite(AD7,bitRead(address,7));

    delay(5);

    digitalWrite(OEN, LOW);

    delay(5);

    inputData[0] = digitalRead(D0);
    inputData[1] = digitalRead(D1);
    inputData[2] = digitalRead(D2);
    inputData[3] = digitalRead(D3);
    inputData[4] = digitalRead(D4);
    inputData[5] = digitalRead(D5);
    inputData[6] = digitalRead(D6);
    inputData[7] = digitalRead(D7);

    delay(5);

    digitalWrite(OEN, HIGH);

    delay(5);

    Serial.print("Address: ");
    Serial.print(address);
    Serial.print("       ");

    Serial.print("Input data: ");
    Serial.print(inputData[7]);
    Serial.print(inputData[6]);
    Serial.print(inputData[5]);
    Serial.print(inputData[4]);
    Serial.print(inputData[3]);
    Serial.print(inputData[2]);
    Serial.print(inputData[1]);
    Serial.println(inputData[0]);

    delay(5);
}

// Address bus setter functions: we can set the pins in either output mode to transfer data or in High-Z (High Impedance, Not connected) mode when we don't want to interfere with the CPU
void setAddressBusToHighZ()
{
    pinMode(AD0, INPUT);
    pinMode(AD1, INPUT);
    pinMode(AD2, INPUT);
    pinMode(AD3, INPUT);
    pinMode(AD4, INPUT);
    pinMode(AD5, INPUT);
    //  pinMode(AD6, INPUT); //Due to the limited number of Arduino pins, only 64 bytes of memory are addressable.
    //  pinMode(AD7, INPUT); //Due to the limited number of Arduino pins, only 64 bytes of memory are addressable.
}

void setAddressBusToOutput()
{
    pinMode(AD0, OUTPUT);
    pinMode(AD1, OUTPUT);
    pinMode(AD2, OUTPUT);
    pinMode(AD3, OUTPUT);
    pinMode(AD4, OUTPUT);
    pinMode(AD5, OUTPUT);
    //  pinMode(AD6, OUTPUT); //Due to the limited number of Arduino pins, only 64 bytes of memory are addressable.
    // pinMode(AD7, OUTPUT); //Due to the limited number of Arduino pins, only 64 bytes of memory are addressable.
}

// Data bus setter functions: we can set the pins in either output mode to transfer data or in High-Z (High Impedance, Not connected) mode when we don't want to interfere with the CPU
void setDataBusToInput()
{
    pinMode(D0, INPUT);
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(D4, INPUT);
    pinMode(D5, INPUT);
    pinMode(D6, INPUT);
    pinMode(D7, INPUT);
}

void setDataBusToOutput()
{
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
}

// Control bus setter functions: we can set the pins in either output mode to transfer data or in High-Z (High Impedance, Not connected) mode when we don't want to interfere with the CPU
void setControlsToHighZ()
{
    pinMode(CSN, INPUT);
    pinMode(WEN, INPUT);
    pinMode(OEN, INPUT);
}

void setControlsToOutput()
{
    pinMode(CSN, OUTPUT);
    pinMode(WEN, OUTPUT);
    pinMode(OEN, OUTPUT);
}

// Setup function: it runs only once, so that is perfect for our task since we need to run our program once, too.
void setup()
{
    // First we are setting the buses in output mode in order to be able to fill up the RAM

    // Data bus:
    setDataBusToOutput();

    //Address bus:
    setAddressBusToOutput();

    // Control bus:
    setControlsToOutput();

    //We also need a HOLD sign to sign the CPU when it can run or when it should stop
    pinMode(HOLD, OUTPUT);

    delay(100);

    digitalWrite(HOLD, HIGH); // First the CPU needs to stop beacuse at the beginning we're going to fill up the RAM with our program

    Serial.begin(9600); // Start serial connection so that we will be able to see the results

    delay(400);
    Serial.println("----------------------------------");
    Serial.println("i8085 SRAM writer system");
    Serial.println("written by Balazs Markus - 2018");
    Serial.println();
    Serial.println("Writing RAM...");

    delay(400);

    //Initialize the RAM with zero values

    for (int j = 0; j < 64; j++) {
        writeRam(j, 0x00);
    }

    /* Write our program in the RAM, beginning from address 0x0000
   With our writeRam function, we can write in the RAM byte-by-byte
   
   In this case, the preloaded software is a simple program that adds two numbers, then stores the result in the memory.
        
   The test program in 8085 Assembly:
    
   MVI B, 03h  ; Move 03h to B register
   MOV A, B  ; Move the content of B register to the Accumulator
   MVI B, 02h      ; Move 02h to B register
   ADD B   ; Add the value of register B to A
   STA 20h   ; Store the value of the Accumulator at address 20h

   This code is assembled into machine code (opcodes+operands) with a standard 8085 assembler, then we write the machine code in the memory byte-by-byte.
 */

    writeRam(0x00, 0x06);
    writeRam(0x01, 0x03); // 1. operandus
    writeRam(0x02, 0x78);
    writeRam(0x03, 0x06);
    writeRam(0x04, 0x02); // 2. operandus
    writeRam(0x05, 0x80);
    writeRam(0x06, 0x32);
    writeRam(0x07, 0x20); // melyik címen tároljuk el
    writeRam(0x08, 0x00);
    writeRam(0x08, 0x76); // HALT OPCODE, doesn't needed in this case, but can be useful
 
    delay(100);

    // We set the Arduino outputs High-Z, so that the CPU can take over the control of all buses

    setControlsToHighZ();
    setDataBusToInput();
    setAddressBusToHighZ(); //a 0-és címen az utasítás átíródik, viszont eltárolódik a kapott eredmény a megadott címen szóval semmi gond

    delay(100);

    digitalWrite(HOLD, LOW); // We set the HOLD sign LOW so that the CPU can start running, and executing our program

    Serial.println("CPU is working...");
    
    delay(200); // We let the CPU work for a while

    digitalWrite(HOLD, HIGH); // After some time, we set the HOLD sign HIGH again so the CPU stops running, and sets its buses in High-Z mode, so the Arduino can take the control back

    Serial.println("Reading RAM...");

    delay(100);

    // We read the first 40 bytes of the RAM, hopefully at address 32 (0x20 in hexadecimal) we will find the result of the operation
    for (int i = 0; i < 40; i++) {
        readRam(i);
    }
}

//We don't need to calculate more than one, so the loop function here isn't needed.
void loop()
{
}

