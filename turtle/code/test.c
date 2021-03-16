#include "parser.h"
#include "test_interp.h"
#define FINAL_DELAY_TIME 1000

#define EX1 "../data/simp_c.ttl"
#define EX2 "../data/simp.ttl"
#define EX3 "../data/spiral1_c.ttl"
#define EX4 "../data/spiral2_c.ttl"
#define EX5 "../data/3loop.ttl"
#define EX6 "../data/fromVarToVar.ttl"
#define EX7 "../data/redefinedVariable.ttl"
#define EX8 "../data/wrongnotation1.ttl" /* This can pass parse but not interp */


void test_stringlist();
void test_general();
void test_function();/* for function in parse.c and interp.c */
bool test_parser(char argv[]);
bool test_interp(char argv[], SDL_Simplewin sw);

int main(void){
    SDL_Simplewin sw;
    test_general();
    test_stringlist();
    test_function();
    assert(test_parser(EX1));
    assert(test_parser(EX2));
    assert(test_parser(EX3));
    assert(test_parser(EX4));
    assert(test_parser(EX5));
    assert(test_parser(EX6));
    assert(test_parser(EX7));
    assert(test_parser(EX8));

    Neill_SDL_Init(&sw);
    test_interp(EX1, sw);
    test_interp(EX2, sw);
    test_interp(EX3, sw);
    test_interp(EX4, sw);
    test_interp(EX5, sw);
    test_interp(EX6, sw);
    test_interp(EX7, sw);
    SDL_Quit();
    return 0;
}
bool test_interp(char argv[], SDL_Simplewin sw){
    printf("[START]Interp : %s\n", argv);
    Neill_SDL_SetDrawColour(&sw, 255, 255, 255);
    iProg(inport_file(argv), &sw);
    Neill_SDL_UpdateScreen(&sw);
    Neill_SDL_Events(&sw);
    SDL_Delay(FINAL_DELAY_TIME);
    SDL_SetRenderDrawColor(sw.renderer, 0, 0, 0, 255);
    SDL_RenderClear(sw.renderer);
    return false;
}

bool test_parser(char argv[]){
    printf("[START]Parser : %s\n", argv);
    if(Prog(inport_file(argv))){
        printf("[COMPLETE]Parser : %s\n", argv);
        return true;
    }
    return false;
}

void test_general(){
    assert(is_capital_letter("C"));
    assert(is_capital_letter("A"));
    assert(is_capital_letter("z"));
    assert(!is_capital_letter("Ca"));
    assert(!is_capital_letter("D2"));
    assert(!is_capital_letter("2"));
    assert(is_number("297"));
    assert(is_number("27"));
    assert(is_number("0"));
    assert(is_number("0.15"));
    assert(is_number(".43"));/* Note in readme!! */
    assert(is_number("43."));/* Note in readme!! */
    assert(!is_number("4af"));
    assert(!is_number("fsd"));
    assert(!is_number("..43"));
    assert(get_number("26") - 26 < 1e-5);
    assert(get_number("0.15") - 0.15 < 1e-5);
    assert(get_number("A") - 0 < 1e-5 );
    assert(get_number("Z") - 25 < 1e-5);
    assert(get_number(".43") - 0.43 < 1e-5);
    assert(get_number("a") - 0 < 1e-5);
    assert(get_number("sf") - -1 < 1e-5);
    printf("[OK]Test : general.c\n");
}
void test_stringlist(){
    list* l = init_list();
    str* s = init_str("string");
    int i = 0;
    assert(!strcmp(s->word,"string"));
    assert(s->next == NULL);
    free(s);
    assert(l->size == 0);
    assert(l->current == NULL);
    assert(l->start == NULL);

    for(i = 0;i < 99; i++){
        if(l->current){
            assert(l->current->next == NULL);
        }
        assert(add(l, "test"));
        assert(size(l) == i + 1);
    }
    assert(size(l) == 99);
    l->current = l->start;
    for(i = 0; i < 99; i++){
        assert(!strcmp(cur_word(l), "test"));
        assert(to_next_word(l));
    }
    /*assert(print_list(l));*/
    assert(cur_index('A') == 0);
    assert(cur_index('z') == 25);
    assert(free_list(l));
    printf("[OK]Test : stringlist.c\n");
}
void test_function(){
    int x, y;
    data* d1 = init_data();
    dat* d2 = ini_data();

    for(x = 0; x < 26; x++){
        for(y = 0; y < 2; y++){
            assert(d1->var[x][y] + 1 < 1e-5);
        }
        assert(d1->defined_var[x] == false);
    }
    assert(data_num("test", d2) - 0 < 1e-5);
    assert(data_num("a", d2) - d2->var[0][0] < 1e-5);
    printf("[OK]Test : Other function\n");
}
