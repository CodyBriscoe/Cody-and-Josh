#include "test.h"

static int32_t xResult[100];
static int32_t yResult[100];
static int32_t xMax;
static int32_t yMax;
static int32_t xMin;
static int32_t yMin;
static int32_t length = 100;
static uint32_t boundsSet = 0;

void testGraph(void){
	for(int i = 0; i < 100; i += 1){
		int32_t x = i - 50;
		int32_t y = (x) + 100;//some function
		xResult[i] = x;//store the x point
		yResult[i] = y;//store the y point 
		if(!boundsSet){
			xMax = x;
			xMin = x;
			yMin = y;
			yMax = y;
			boundsSet = 1;
		}
		else{//reset the bounds of the graph
			if(x > xMax){
				xMax = x;
			}
			if(x < xMin){
				xMin = x;
			}
			if(y > yMax){
				yMax = y;
			}
			if(y < yMin){
				yMin = y;
			}
		}
	}
}

int32_t* getXData(void){return xResult;}
int32_t* getYData(void){return yResult;}
int32_t getXMax(){return xMax;}
int32_t getYMax(){return yMax;}
int32_t getXMin(){return xMin;}
int32_t getYMin(){return yMin;}
int32_t getLength(){return length;}
