#include "vector.h"
#include "game_state.h"

#ifndef _CLOUD
#define _CLOUD

typedef enum CLOUD_TYPES {
    SMALL_CIRRUS = 0,
    MEDIUM_STRATOCULUMUS,
    // more to be added

    CLOUD_TYPES_N,
} CLOUD_TYPES;

extern VECTOR CLOUD_SIZES[CLOUD_TYPES_N];

typedef struct CLOUD {
    VECTOR position;
    int type; // this determines which sprite and what size the cloud will be
    float speed;
    bool used;
} CLOUD;

#define MAX_CLOUDS 32

extern CLOUD clouds[MAX_CLOUDS];

void update_clouds();

#endif