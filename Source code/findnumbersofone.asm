;Find numbers of one
;Task: Write a program to count number of l’s in the contents of D register and store the count in the B register.
;written by Balázs Márkus in 2018

MVI B, 00H
MVI C, 08H
MVI D, 05H
MOV A, D
BACK: RAR
JNC SKIP
INR B
SKIP: DCR C
JNZ BACK
MOV A, B
STA 20h
HLT 