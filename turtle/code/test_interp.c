#include "test_interp.h"
#include "parser.h"

bool iProg(program *p, SDL_Simplewin* sw){
    dat *d = ini_data();

    if(!strsame(cur_word(p), "{"))
        on_error("No beginning brace ?");
    to_next_word(p);
    iInstrctlst(p, d, sw);
    free(d);
    return true;
}

void iInstrctlst(program *p, dat *d, SDL_Simplewin* sw){
    if(strsame(cur_word(p), "}")) return;
    iInstruction(p, d, sw);
    to_next_word(p);
    iInstrctlst(p, d, sw);
}
void iInstruction(program *p, dat *d, SDL_Simplewin* sw){
    if(!strcmp(cur_word(p), "FD")) iFd(p, d, sw);
    else if(!strcmp(cur_word(p), "LT")) iLt(p, d);
    else if(!strcmp(cur_word(p), "RT")) iRt(p, d);
    else if(!strcmp(cur_word(p), "DO")) iDo(p, d, sw);
    else if(!strcmp(cur_word(p), "SET")) iSet(p, d);
    else on_error("Fatal error: Expecting FD/LT/RT/DO/SET");
}
void iFd(program *p, dat *d, SDL_Simplewin* sw){
    float f;
    to_next_word(p);
    Varnum(p);
    f = data_num(cur_word(p), d);
    d->x0 = d->x1;
    d->y0 = d->y1;
    d->x1 = d->x1 + f * sin(d->ang * PI / PI_DEG);
    d->y1 = d->y1 + f * cos(d->ang * PI / PI_DEG);

    SDL_RenderDrawLine(sw->renderer, d->x0, d->y0, d->x1, d->y1);
    printf("Fd : %f\n", f);
    return;
}
void iLt(program *p, dat *d){
    float f;
    to_next_word(p);
    Varnum(p);
    f = data_num(cur_word(p), d);
    printf("Lt : %f\n", f);
    d->ang = d->ang + f;
    return;
}
void iRt(program *p, dat *d){
    float f;
    to_next_word(p);
    Varnum(p);
    f = data_num(cur_word(p), d);
    printf("Rt : %f\n", f);
    d->ang = d->ang - f;
    return;
}
void iDo(program *p, dat *d, SDL_Simplewin* sw){
    int v;
    char c;
    str* pc;
    printf("————Do loop %d————\n",d->round);
    to_next_word(p);
    Var(p);
    v = cur_index(*cur_word(p));
    c = *cur_word(p);
    to_next_word(p);
    if(!strsame(cur_word(p), "FROM"))
        on_error("Expecting 'FROM'.");
    to_next_word(p);
    Varnum(p);
    d->var[v][0] = get_number(cur_word(p));
    to_next_word(p);
    if(!strsame(cur_word(p), "TO"))
        on_error("Expecting 'TO'.");
    to_next_word(p);
    Varnum(p);
    d->var[v][1] = get_number(cur_word(p));
    to_next_word(p);
    if(!strsame(cur_word(p), "{"))
        on_error("Expecting '{'.");
    d->round++;
    to_next_word(p);
    printf("%c : FROM %f TO %f\n", c, d->var[v][0], d->var[v][1]);

    pc = p->current;
    while((int)d->var[v][0] <= (int)d->var[v][1]){
        p->current = pc;
        printf("【Loop】 %f\n", d->var[v][0]);
        iInstrctlst(p, d, sw);
        d->var[v][0] = d->var[v][0] + 1;
    }
    return;
}
void iSet(program *p, dat *d){/*给常量赋值*/
    stack* s;
    int v;
    to_next_word(p);
    v = cur_index(*cur_word(p));
    to_next_word(p);
    if(!strsame(cur_word(p), ":="))
        on_error("Expecting ':='.");
    to_next_word(p);
    s = stack_init();
    iPolish(p, s, d);
    if(s->size != 1)
        on_error("Wrong notation. Please use Reverse Polish notation.");
    stack_pop(s,&d->var[v][0]);
    stack_free(s);
    return;
}
void iPolish(program *p, stack* s, dat *d){ /* 改成return double的一个东西 */
    float ans = 0, x, y;
    if(strsame(cur_word(p), ";")) return;
    if(Op(p)){
        if(stack_pop(s, &x) && stack_pop(s, &y)){
            switch(*cur_word(p)){
                case '+': ans = y + x; break;
                case '-': ans = y - x; break;
                case '*': ans = y * x; break;
                case '/': ans = y / x; break;
            }
            stack_push(s, ans);
        }
        else{
            on_error("Wrong notation. Please use Reverse Polish notation.");
        }
        to_next_word(p);
    iPolish(p, s, d);
    }
    else{
        if(is_number(cur_word(p))){
            stack_push(s, get_number(cur_word(p)));
        }
        if(is_capital_letter(cur_word(p))){
            stack_push(s, d->var[cur_index(*cur_word(p))][0]);
        }
        to_next_word(p);
        iPolish(p, s, d);
    }
}

float data_num(char* s, dat* d){
    if(is_capital_letter(s)){
        return d->var[cur_index(*s)][0];
    }
    if(is_number(s)){
        return get_number(s);
    }
    return 0;
}

dat* ini_data(){
    int x, y;
    dat *d = ncalloc(1, sizeof(data));
    d->round = 0;
    for(x = 0; x < 26; x++){
        for(y = 0; y < 2; y++){
            d->var[x][y] = -1;
        }
    }
    d->x1 = 400;
    d->y1 = 300;
    d->ang = 180;

    return d;
}
