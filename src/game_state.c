#include "game_state.h"

void must_init(bool test, char* description) {
    if (test) return;
    printf("can't initalize %s!\n", description);
    exit(1);
}