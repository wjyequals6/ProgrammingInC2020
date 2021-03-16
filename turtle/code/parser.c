#include "parser.h"

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
bool Prog(program *p){
    data *d = init_data();
    if(!strsame(cur_word(p), "{"))
        on_error("Error: Error:No beginning brace ?");
    to_next_word(p);
    Instrctlst(p, d);
    return true;
}
void Instrctlst(program *p, data *d){
    if(strsame(cur_word(p), "}")) return;
    if(p->current->next){
        Instruction(p, d);
        to_next_word(p);
        Instrctlst(p, d);
    }
}
void Instruction(program *p, data *d){
    if(!strcmp(cur_word(p), "FD")) Fd(p, d);
    else if(!strcmp(cur_word(p), "LT")) Lt(p, d);
    else if(!strcmp(cur_word(p), "RT")) Rt(p, d);
    else if(!strcmp(cur_word(p), "DO")) Do(p, d);
    else if(!strcmp(cur_word(p), "SET")) Set(p, d);
    else on_error("Error: Expecting FD/LT/RT/DO/SET/}");
}
void Fd(program *p, data *d){
    to_next_word(p);
    Varnum(p);
    if(!d->defined_var[cur_index(*cur_word(p))])
        on_error("Error: You are using undefined variables!\n");
}
void Lt(program *p, data *d){
    to_next_word(p);
    Varnum(p);
    if(!d->defined_var[cur_index(*cur_word(p))])
        on_error("Error: You are using undefined variables!\n");
}
void Rt(program *p, data *d){
    to_next_word(p);
    Varnum(p);
    if(!d->defined_var[cur_index(*cur_word(p))])
        on_error("Error: You are using undefined variables!\n");
}
void Do(program *p, data *d){
    int v;

    to_next_word(p);
    Var(p);
    v = cur_index(*cur_word(p));
    if(!d->defined_var[v]) d->defined_var[v] = true;
    else printf("Warning: Variable Redefinition!\n");

    to_next_word(p);
    if(!strsame(cur_word(p), "FROM"))
        on_error("Error: Expecting 'FROM'.");

    to_next_word(p);
    Varnum(p);

    to_next_word(p);
    if(!strsame(cur_word(p), "TO"))
        on_error("Error: Expecting 'TO'.");

    to_next_word(p);
    Varnum(p);

    to_next_word(p);
    if(!strsame(cur_word(p), "{"))
        on_error("Error: Expecting '{'.");
    to_next_word(p);
    Instrctlst(p, d);
}
void Set(program *p, data *d){
    stack* s;
    int v;

    to_next_word(p);
    Var(p);
    v = cur_index(*cur_word(p));
    if(!d->defined_var[v]) d->defined_var[v] = true;
    else printf("Warning: Variable Redefinition!\n");

    to_next_word(p);
    if(!strsame(cur_word(p), ":="))
        on_error("Expecting ':='.");
    to_next_word(p);
    s = stack_init();
    Polish(p, s, d);
    if(s->size != 1)
        printf("Warning: Wrong notation. Something has not been calculated.\nPlease use Reverse Polish notation.\n");
    stack_pop(s,&d->var[v][0]);
    d->defined_var[v] = true;
    stack_free(s);
}
void Varnum(program *p){
    if(is_number(cur_word(p))) return;
    Var(p);
}
void Var(program *p){
    if(is_capital_letter(cur_word(p))) return;
        printf("Warning: Expecting a number or a single capital letter.\n");
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
            on_error("Error: Wrong notation. Lack of number.\nPlease use Reverse Polish notation.\n");
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
data* init_data(){
    int x, y;
    data *d = ncalloc(1, sizeof(data));
    for(x = 0; x < 26; x++){
        for(y = 0; y < 2; y++){
            d->var[x][y] = -1;
        }
        d->defined_var[x] = false;
    }
    return d;
}
