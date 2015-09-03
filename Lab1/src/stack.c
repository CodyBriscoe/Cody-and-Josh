/* stack.c: stack implem 
	from the utsa cs website
	http://www.cs.utsa.edu/~wagner/CS2213/stack/stack.html*/
#include "stack.h"


#define MAXSTACK 5
#define EMPTYSTACK -1
int top = EMPTYSTACK;
char items[MAXSTACK];




void push(char c) {
   items[++top] = c;
}

char pop() {
   return items[top--];
}

int full()  {
   return top+1 == MAXSTACK;
}

int empty()  {
   return top == EMPTYSTACK;
}
