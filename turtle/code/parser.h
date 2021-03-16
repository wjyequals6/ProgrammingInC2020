#include "general.h"
#include "stringlist.h"
#include "stack.h"

#define strsame(A,B) (strcmp(A, B)==0)
#define program list
#define MAX_EACH_WORD 20

typedef struct data{
    float var[26][2];
    bool defined_var[26];
}data;

program* inport_file(char argv[]);
bool Prog(program *p);
void Instrctlst(program *p, data *d);
void Instruction(program *p, data *d);
void Fd(program *p, data *d);
void Lt(program *p, data *d);
void Rt(program *p, data *d);
void Do(program *p, data *d);
void Set(program *p, data *d);
void Varnum(program *p);
void Var(program *p);
void Polish(program *p, stack* s, data *d);
bool Op(program *p);
data* init_data();
