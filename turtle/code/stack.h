#pragma once

#include "general.h"
typedef float stacktype;
typedef struct stack stack;

#define FORMATSTR "%d"
#define ELEMSIZE 20
#define STACKTYPE "Linked"

struct dataframe {
   stacktype i;
   struct dataframe* next;
};
typedef struct dataframe dataframe;

struct stack {
   /* Underlying array */
   dataframe* start;
   int size;
};

/* Create an empty stack */
stack* stack_init(void);
/* Add element to top */
void stack_push(stack* s, stacktype i);
/* Take element from top */
bool stack_pop(stack* s, stacktype* d);
/* Clears all space used */
bool stack_free(stack* s);
