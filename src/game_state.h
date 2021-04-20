/*
    as well, provide some utility functions
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _GAME_STATE
#define _GAME_STATE

extern long frames;
extern long score;
extern bool debug;
extern bool night;
extern bool paused;

typedef enum {
    MAIN_MENU = 0,
    CREDITS,
    PAUSED,
    PLAYING,
    GAME_OVER,
} GAME_STATE;

extern int game_state;

void must_init(bool test, char* description);
void init_game();
float between(float lower, float higher);

// menu button functions, defined in main.c

void start_game();
void game_over();
void restart_game();

void main_menu();
void show_credits();
void quit();

#endif