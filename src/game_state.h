#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern long frames;
extern long score;
extern bool debug;

#ifndef GAME_STATE
#define GAME_STATE
typedef enum {
    MAIN_MENU = 0,
    CREDITS,
    PAUSED,
    PLAYING,
    GAME_OVER,
} GAME_STATE;
#endif

extern int game_state;

void must_init(bool test, char* description);
void init_game();