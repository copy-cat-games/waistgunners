#include "vector.h"

#ifndef _BULLET
#define _BULLET

typedef enum ALLIANCE {
    PLAYER_BULLET = 0,
    ENEMY_BULLET,
} ALLIANCE;

typedef struct BULLET {
    VECTOR position;
    VECTOR motion;
    ALLIANCE alliance;
    bool used;
} BULLET;

#define MAX_BULLETS 256
#define BULLET_SPEED 3.5

extern BULLET bullets[MAX_BULLETS];
extern VECTOR BULLET_SIZE;

void initialize_bullets();
void destroy_bullet(BULLET* bullet);
void add_bullet(VECTOR position, VECTOR motion, ALLIANCE alliance);
void update_bullets();

#endif