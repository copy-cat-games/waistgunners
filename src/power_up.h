#include "game_state.h"
#include "vector.h"
#include "sounds.h"

#ifndef _POWER_UP
#define _POWER_UP

typedef enum POWER_UP_TYPE {
    NONE = -1,
    BIGGER_CLIP_SIZE = 0, // common
    FASTER_RELOAD, // common
    FASTER_BULLETS, // common
    DESTROY_ENEMIES, // rare
    REPAIR_ENGINE, // common
    TEMPORARY_INVINCIBILITY, // common
    POWER_UP_N,
} POWER_UP_TYPE;

extern POWER_UP_TYPE common_power_ups[];
extern POWER_UP_TYPE rare_power_ups[];

/*
    planned:
    - infinite shooting (rare)
    - increase immunity chance (rare)
    - invulnerability/invincibility (common)
    - permanent increase in clip size (permanent)
    - more engine health (permanent)
    - passive healing (permanent)
    - life steal (permanent)
    - change clip size increase to additive

    group powerups by rarity
    - common (purple)
    - rare (blue)
    - permanent upgrade (green)
    - missing (grey)
 */

typedef struct POWER_UP {
    VECTOR position;
    POWER_UP_TYPE type;
    int lifetime;
    bool activated;
} POWER_UP;

extern char* names[];
extern VECTOR POWER_UP_SIZE;

#define MAX_STORED_POWER_UPS 3

extern POWER_UP stored_power_ups[MAX_STORED_POWER_UPS];
extern int MAX_POWER_UP_LIFETIMES[POWER_UP_N];

void update_power_ups();
void activate_power_up(int index);
int power_up_activated(POWER_UP_TYPE type);

POWER_UP create_power_up(VECTOR position, POWER_UP_TYPE type);
void add_power_up(POWER_UP_TYPE type);
void add_common_power_up();
void add_rare_power_up();

void reset_stored_power_ups();

#endif