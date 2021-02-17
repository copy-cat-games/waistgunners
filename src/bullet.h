#include "vector.h"

#ifndef _BULLET
#define _BULLET

typedef enum ALLIANCE {
    PLAYER_BULLET = 0,
    ENEMY_BULLET,
} ALLIANCE;

typedef struct BULLET {
    VECTOR position;
    ALLIANCE alliance;
} BULLET;

#define MAX_BULLETS 256

extern BULLET bullets[MAX_BULLETS];

void destroy_bullet(BULLET* bullet);
void add_bullet(VECTOR position, ALLIANCE alliance);

#endif