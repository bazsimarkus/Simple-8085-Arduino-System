;Addtwonumbers
;Simple x85 assembly program to test the proper working of a 8085 microprocessor system
;written by Balázs Márkus in 2018

MVI B, 03h	; Move 03h to B register
MOV A, B	; Move the content of B register to the Accumulator
MVI B, 02h      ; Move 02h to B register
ADD B		; Add the value of register B to A
STA 20h		; Store the value of the Accumulator at address 20h