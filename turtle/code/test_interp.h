#pragma once
#include "general.h"
#include "stack.h"
#include "stringlist.h"
#include "neillsdl2.h"
#define program list

#define PI acos(-1)
#define PI_DEG 180
#define MAX_LOOP 10
#define PAUSE_TIME 1000

typedef struct dat{
    float var[26][2];
    int round;
    double x0;
    double y0;
    double x1;
    double y1;
    double ang;
}dat;
/*variable pool*/

bool iProg(program *p, SDL_Simplewin* sw);
void iInstrctlst(program *p, dat *d, SDL_Simplewin* sw);
void iInstruction(program *p, dat *d, SDL_Simplewin* sw);
void iFd(program *p, dat *d, SDL_Simplewin* sw);
void iLt(program *p, dat *d);
void iRt(program *p, dat *d);
void iDo(program *p, dat *d, SDL_Simplewin* sw);
void iSet(program *p, dat *d);
void iPolish(program *p, stack *s, dat *d);
float data_num(char* s, dat* d);
dat* ini_data();
