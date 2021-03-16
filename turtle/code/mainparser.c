#include "parser.h"

int main(int argc, char *argv[]){

    if((argc != 2) || (argv[1][0] == '\0')){
        on_error("Please input a program to execute.");
    }

    if(Prog(inport_file(argv[1]))){
        printf("\nParsed OK\n");
    }
    return 0;
}
