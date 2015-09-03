#ifndef FIXED_H
#define FIXED_H

#include "stack.h"
#include "inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include <stdio.h>
#include <stdint.h>


/*Given an unsigned int returns a 
rounded down version to the nearest 10s
place*/
uint32_t projFloor(uint32_t N);

/*Given an usigned int returns a rounded up
version to the nearest 10s place*/
uint32_t projCeiling(uint32_t N);

/*Given an unsigned int returns a rounded
version to the nearest 10's place*/
uint32_t projRound(uint32_t N);

/*Converts a unsigned integer between
0 and 9 to its acii charecter code
DONOT pass anything besides 0- 9 into 
this function*/
char convertIntToChar(uint32_t N);

/*Handles numbers that get converted into 
unsigned fixed point notation in a binary scheme
Includes a range detector that outputs astericks to
stack if the number input was to big */
void uBinOut(uint32_t N);

/*Takes a signed integer 
and places the individual digits
onto a global stack starting with the
least significant digit
will pass a - at the end if it was 
originally a negative number
will pass a space " " onto the
stack if the number is positive*/
void sDecOut(int32_t N);

/*tests the binaray representation
by poping off the global stack and 
putting a decimal at a fix point*/
void testBinary(void);

/*test the decimal representation 
by popping off the global stack and
putting a decimal at a fixed point*/
void testDec(void);

/*function that is high when the switch is pressed 
low when the switch isn't pressed*/
int32_t Switch_Pressed(void);

/*intializes edge interrupts for the 
button at PF4*/
void Edge_Init(void);

#endif

