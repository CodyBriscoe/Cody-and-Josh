#include "fixed.h"

void static setTable(){
	ST7735_DrawFastVLine(64, 0, 160, ST7735_YELLOW);
	ST7735_DrawFastHLine(0, 8, 128, ST7735_YELLOW);
	ST7735_SetCursor(1, 0);
	printf("Parameter");
	ST7735_SetCursor(13, 0);
	printf("Result");
}

uint32_t projFloor(uint32_t N){return (N/10) * 10;}

uint32_t projCeiling(uint32_t N){return ((N/10) + 1) *10;}

uint32_t projRound(uint32_t N){
	uint32_t P = N - projFloor(N);
	if(P < 5){return projFloor(N);}
	return projCeiling(N);
}

char convertIntToChar(uint32_t N){
	N += 48;
	return (char)N;
}

void uBinOut(uint32_t N){
	if(N > (256000 - 1)){
			for(int  i = 0; i < 5; i += 1){
				push('*');
			}
			return;
	}
	N = N * 1000;
	N = N / 256;
	N = projRound(N);
	N = N / 10;
	int push_count = 0;
	while(N > 10){
		uint32_t D = N % 10;
		push(convertIntToChar(D));
		push_count += 1;
		N = N / 10;
	}
	push(convertIntToChar(N));
	push_count += 1;
	for(uint32_t i = push_count; i < 5; i++){//5 is the maximum number of digits to display
		if(i < 3){
			push('0');
		}
		else{push(' ');}
	}
}

/*This is a comment*/
void sDecOut(int32_t N){
	if((N > 9999) || (N < -9999)){
		for(int i = 0; i < 4; i += 1){
			push('*');
		}
		push(' ');
	}
	else
	{
		int neg = 0;
		if (N < 0)
		{ 
			neg = 1; 
			N *= -1;
		} 
		int push_count = 0;
		while(N > 10){
			push(convertIntToChar(N % 10));
			N = N / 10;
			push_count += 1;
		}
		push(convertIntToChar(N));
		push_count += 1;
		for(int i = push_count; i < 4; i += 1){
			push('0');
		}
		if(neg){push('-');}
		else{push(' ');}
	}
}

void testBinary(void){
	int binary_input[] = {0,2,64,100,500,512,5000,30000,255997,256000};
	setTable();
	for(int i = 0; i < 10; i += 1){
		ST7735_SetCursor(1, (i+1));
		printf("%d  ", binary_input[i]);
		ST7735_SetCursor(12, (i+1));
		uBinOut(binary_input[i]);//puts the binary rep nicely on the stack
		for(int j = 0; j < 6; j += 1){
			if(j == 3)
			{
				ST7735_OutChar('.');
			}
			else
			{
				if(!empty())
				{ ST7735_OutChar(pop()); }
			}
		}
	}
}


void testDec(void){
	int dec_input[] = {-100000,-10000,-9999,-999,-1,0,123,1234,9999,10000};
	setTable();
	for(int i = 0; i < 10; i += 1){
		ST7735_SetCursor(1, (i+1));
		printf("%d  ", dec_input[i]);
		ST7735_SetCursor(12, (i+1));
		sDecOut(dec_input[i]);//puts the dec rep nicely on the stack
		for(int j = 0; j < 6; j += 1){
			if(j == 2)
			{
				ST7735_OutChar('.');
			}
			else
			{
				if(!empty())
				{ ST7735_OutChar(pop()); }
			}
		}
	}
}

// EdgeInterrupt.c
// Runs on LM4F120 or TM4C123
// Request an interrupt on the falling edge of PF4 (when the user
// button is pressed) and increment a counter in the interrupt.  Note
// that button bouncing is not addressed.
// Daniel Valvano
// September 14, 2013

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014
   Volume 1, Program 9.4
   
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
   Volume 2, Program 5.6, Section 5.5

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// user button connected to PF4 (increment counter on falling edge)

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

int32_t flag;
void Edge_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F
	flag = 0;
	while((SYSCTL_PRGPIO_R&0x20)==0){};
  GPIO_PORTF_DIR_R &= ~0x10;    // (c) make PF4 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x10;  //     disable alt funct on PF4
  GPIO_PORTF_DEN_R |= 0x10;     //     enable digital I/O on PF4   
  GPIO_PORTF_PCTL_R &= ~0x000F0000; // configure PF4 as GPIO
  GPIO_PORTF_AMSEL_R = 0;       //     disable analog functionality on PF
  GPIO_PORTF_PUR_R |= 0x10;     //     enable weak pull-up on PF4
  GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges
  GPIO_PORTF_IEV_R &= ~0x10;    //     PF4 falling edge event
  GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
  EnableInterrupts();           // (i) Clears the I bit
}
int32_t waiting = 0;
void GPIOPortF_Handler(void){
  GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
	if(waiting)
	{ flag ^= 1; }
}

void wait(int32_t x){
	waiting = x;
}

int32_t check = 0;
int32_t Switch_Pressed(){
	if(flag == check)
	{ return 0; }
	check = flag;
	return 1;
}
#define GRAPH_H 149
#define GRAPH_W 127


int32_t xScale = 0;
int32_t yScale = 0;
int32_t Xlow = 0;
int32_t Yhigh = 0;
void XYplotInit(uint8_t * stringy, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY){
	xScale = maxX - minX;
	yScale = maxY - minY;
	Xlow = minX;
	Yhigh = maxY;
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetCursor(1,0);
	ST7735_OutString(stringy);
	ST7735_DrawFastVLine((minX*GRAPH_W)/xScale, 10, 159, ST7735_YELLOW);
	ST7735_DrawFastHLine(0, 20 +(minY), 127, ST7735_YELLOW);
}

void XYplot(int32_t* xBuff, int32_t * yBuff, int32_t num){
	for(int32_t i = 0; i < num; i++)
	{
		int32_t x = ((xBuff[i] - Xlow) * GRAPH_W) / xScale;
		int32_t y = (10 +(((Yhigh - yBuff[i]) * GRAPH_H) / yScale));
		ST7735_DrawPixel(x, y + 1, ST7735_YELLOW);
		ST7735_DrawPixel(x, y, ST7735_YELLOW);
		ST7735_DrawPixel(x + 1, y + 1, ST7735_YELLOW);
		ST7735_DrawPixel(x + 1, y, ST7735_YELLOW);
	}
}
