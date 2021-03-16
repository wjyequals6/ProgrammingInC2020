#include "interp.h"

int main(int argc, char *argv[]){
    SDL_Simplewin sw;

    if((argc != 2) || (argv[1][0] == '\0')){
        on_error("Please input a program to execute.");
    }
    Neill_SDL_Init(&sw);
    Neill_SDL_SetDrawColour(&sw, 255, 255, 255);
    Prog(inport_file(argv[1]), &sw);
    Neill_SDL_UpdateScreen(&sw);
    Neill_SDL_Events(&sw);
    SDL_Delay(FINAL_DELAY_TIME);
    SDL_Quit();
    atexit(SDL_Quit);
    return 0;
}
