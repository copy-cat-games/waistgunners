#include "vector.h"
#include "engine.h"

#ifndef _BOMBER
#define _BOMBER

typedef struct BOMBER {
    VECTOR position; // top left corner
} BOMBER;

#define MAX_BOMBERS 6
extern BOMBER bombers[MAX_BOMBERS];

void update_bombers();
void reset_bombers();

#endif