#include "extension.h"

int main(int argc, char *argv[]){
    SDL_Simplewin sw;

    if((argc != 2) || (argv[1][0] == '\0')){
        on_error("Please input a program to execute.");
    }
    srand((unsigned)time(NULL));
    Neill_SDL_Init(&sw);
    Neill_SDL_SetDrawColour(&sw, rand()%255, rand()%255, rand()%255);
    Prog(inport_file(argv[1]), &sw);
    Neill_SDL_UpdateScreen(&sw);
    Neill_SDL_Events(&sw);
    SDL_Delay(FINAL_DELAY_TIME);
    SDL_Quit();
    atexit(SDL_Quit);
    return 0;
}
program* inport_file(char argv[]){
    FILE *fp;
    program* p = init_list();
    char tmp[MAX_EACH_WORD];
    fp = nfopen(argv, "r");
    while(fscanf(fp, "%s", tmp) == 1){
        if(add(p, tmp));
        else on_error("Error:Inport_file");
    };
    fclose(fp);
    p->current = p->start;
    return p;
}
bool Prog(program *p, SDL_Simplewin* sw){
    data *d = init_data();
    if(!strsame(cur_word(p), "{"))
        on_error("Error: No beginning brace ?");
    to_next_word(p);
    if(strsame(cur_word(p), "OUTPUT")){
        to_next_word(p);
        if(strsame(cur_word(p), "IMAGE"));
        else if (strsame(cur_word(p), "GIF")) d->is_image = false;
        else on_error("Error: Expecting IMAGE / GIF");
        to_next_word(p);
    }
    Instrctlst(p, d, sw);
    free(d);
    return true;
}
void Instrctlst(program *p, data *d, SDL_Simplewin* sw){
    if(strsame(cur_word(p), "}")) return;
    if(p->current->next){
        Instruction(p, d, sw);
        to_next_word(p);
        Instrctlst(p, d, sw);
    }
}
void Instruction(program *p, data *d, SDL_Simplewin* sw){
    if(!strcmp(cur_word(p), "FD")) Fd(p, d, sw);
    else if(color(p, sw)) Fd(p, d, sw);
    else if(!strcmp(cur_word(p), "LT")) Lt(p, d);
    else if(!strcmp(cur_word(p), "RT")) Rt(p, d);
    else if(!strcmp(cur_word(p), "DO")) Do(p, d, sw);
    else if(!strcmp(cur_word(p), "SET")) Set(p, d);
    else on_error("Error: Expecting FD/LT/RT/DO/SET or color.");
}
void Fd(program *p, data *d, SDL_Simplewin* sw){
    float f;
    to_next_word(p);
    Varnum(p);
    if(!d->defined_var[cur_index(*cur_word(p))])
        on_error("Error: You are using undefined variables!\n");
    f = data_num(cur_word(p), d);
    d->x0 = d->x1;
    d->y0 = d->y1;
    d->x1 = d->x1 + f * sin(d->ang * PI / PI_DEG);
    d->y1 = d->y1 + f * cos(d->ang * PI / PI_DEG);

    SDL_RenderDrawLine(sw->renderer, d->x1, d->y1, d->x0, d->y0);
    if(!d->is_image){
        Neill_SDL_UpdateScreen(sw);
        Neill_SDL_Events(sw);
        SDL_Delay(DELAY_TIME);
    }
    return;
}
void Lt(program *p, data *d){
    float f;
    to_next_word(p);
    Varnum(p);
    if(!d->defined_var[cur_index(*cur_word(p))])
        on_error("Error: You are using undefined variables!\n");
    f = data_num(cur_word(p), d);
    d->ang = d->ang + f;
    return;
}
void Rt(program *p, data *d){
    float f;
    to_next_word(p);
    Varnum(p);
    if(!d->defined_var[cur_index(*cur_word(p))])
        on_error("Error: You are using undefined variables!\n");
    f = data_num(cur_word(p), d);
    d->ang = d->ang - f;
    return;
}
void Do(program *p, data *d, SDL_Simplewin* sw){
    int v;
    str* pc;

    to_next_word(p);
    Var(p);
    v = cur_index(*cur_word(p));
    d->defined_var[v] = true;

    to_next_word(p);
    if(!strsame(cur_word(p), "FROM"))
        on_error("Error: Expecting 'FROM'.");

    to_next_word(p);
    Varnum(p);
    d->var[v][0] = get_number(cur_word(p));

    to_next_word(p);
    if(!strsame(cur_word(p), "TO"))
        on_error("Error: Expecting 'TO'.");

    to_next_word(p);
    Varnum(p);
    d->var[v][1] = get_number(cur_word(p));

    to_next_word(p);
    if(!strsame(cur_word(p), "{"))
        on_error("Error: Expecting '{'.");

    to_next_word(p);
    pc = p->current;
    while((int)d->var[v][0] <= (int)d->var[v][1]){
        p->current = pc;
        Instrctlst(p, d, sw);
        d->var[v][0] = d->var[v][0] + 1;
    }
    return;
}
void Set(program *p, data *d){
    stack* s;
    int v;
    to_next_word(p);
    v = cur_index(*cur_word(p));
    to_next_word(p);
    if(!strsame(cur_word(p), ":="))
        on_error("Error: Expecting ':='.");
    to_next_word(p);
    s = stack_init();
    Polish(p, s, d);
    if(s->size != 1)
        on_error("Error: Wrong notation. Please use Reverse Polish notation.");
    stack_pop(s,&d->var[v][0]);
    d->defined_var[v] = true;
    stack_free(s);
    return;
}
void Varnum(program *p){
    if(is_number(cur_word(p))) return;
    Var(p);
}
void Var(program *p){
    if(is_capital_letter(cur_word(p))) return;
    on_error("Error: Expecting a number or a single capital letter.");
}
void Polish(program *p, stack* s, data *d){
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
            on_error("Error: Wrong notation. Please use Reverse Polish notation.");
        }
    to_next_word(p);
    Polish(p, s, d);
    }
    else{
        if(is_number(cur_word(p))){
            stack_push(s, get_number(cur_word(p)));
        }
        if(is_capital_letter(cur_word(p))){
            stack_push(s, d->var[cur_index(*cur_word(p))][0]);
        }
        to_next_word(p);
        Polish(p, s, d);
    }
}
bool Op(program *p){
    bool i = false;
    if (strsame(cur_word(p), "+")) i = true;
    if (strsame(cur_word(p), "-")) i = true;
    if (strsame(cur_word(p), "*")) i = true;
    if (strsame(cur_word(p), "/")) i = true;
    return i;
}
float data_num(char* s, data* d){
    if(is_capital_letter(s)){
        return d->var[cur_index(*s)][0];
    }
    if(is_number(s)){
        return get_number(s);
    }
    return 0;
}
data* init_data(){
    int x, y;
    data *d = ncalloc(1, sizeof(data));
    for(x = 0; x < 26; x++){
        for(y = 0; y < 2; y++){
            d->var[x][y] = -1;
        }
    }
    d->x1 = INI_X;
    d->y1 = INI_Y;
    d->ang = INI_ANGLE;
    d->is_image = true;
    return d;
}
bool color(program *p, SDL_Simplewin* sw){
    if(strsame(cur_word(p), "WHITE")){
        Neill_SDL_SetDrawColour(sw, 255, 255, 255);
        to_next_word(p);
        return true;
    }
    else if(strsame(cur_word(p), "RED")){
        Neill_SDL_SetDrawColour(sw, 255, 0, 0);
        to_next_word(p);
        return true;
    }
    else if(strsame(cur_word(p), "ORANGE")){
        Neill_SDL_SetDrawColour(sw, 255, 160, 0);
        to_next_word(p);
        return true;
    }
    else if(strsame(cur_word(p), "KHAKI")){
        Neill_SDL_SetDrawColour(sw, 240, 230, 124);
        to_next_word(p);
        return true;
    }
    else if(strsame(cur_word(p), "GREEN")){
        Neill_SDL_SetDrawColour(sw, 144, 238, 144);
        to_next_word(p);
        return true;
    }
    else if(strsame(cur_word(p), "CYAN")){
        Neill_SDL_SetDrawColour(sw, 0, 200, 200);
        to_next_word(p);
        return true;
    }
    return false;
}
