#pragma once
#include "inc/tm4c123gh6pm.h"

/*function that generates two parralel arrays
to use in the graphing function*/
void testGraph(void);

/*gets the x array data*/
int32_t* getXData(void);
/*gets the y array data*/
int32_t* getYData(void);
int32_t getXMax(void);
int32_t getYMax(void);
int32_t getXMin(void);
int32_t getYMin(void);
int32_t getLength(void);
