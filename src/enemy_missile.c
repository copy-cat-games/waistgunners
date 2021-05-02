#include "enemy_missile.h"

void update_enemy_missile(ENEMY_MISSILE_DATA* missile) {
    missile->position = add(missile->position, missile->motion);
}