#pragma once
#include "general.h"
#include "stack.h"
#include "stringlist.h"
#include "neillsdl2.h"
#define program list

#define PI acos(-1)
#define PI_DEG 180
#define MAX_LOOP 10
#define FINAL_DELAY_TIME 5000
#define INI_X 400
#define INI_Y 300
#define INI_ANGLE 180

typedef struct data{
    float var[26][2];
    bool defined_var[26];
    double x0;
    double y0;
    double x1;
    double y1;
    double ang;
}data;

bool Prog(program *p, SDL_Simplewin* sw);
program* inport_file(char argv[]);
void Instrctlst(program *p, data *d, SDL_Simplewin* sw);
void Instruction(program *p, data *d, SDL_Simplewin* sw);
void Fd(program *p, data *d, SDL_Simplewin* sw);
void Lt(program *p, data *d);
void Rt(program *p, data *d);
void Do(program *p, data *d, SDL_Simplewin* sw);
void Set(program *p, data *d);
void Polish(program *p, stack *s, data *d);
bool Op(program *p);
float data_num(char* s, data* d);
void Varnum(program *p);
void Var(program *p);
data* init_data();
