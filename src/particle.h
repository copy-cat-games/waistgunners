#include "game_state.h"
#include "vector.h"

// serves as a wrapper for all particles
// add their header files here. make sure they do not depend on particle.h!
#include "smoke.h"
#include "explosion.h"

#ifndef _PARTICLE
#define _PARTICLE

typedef enum PARTICLE_TYPE {
    SMOKE_PARTICLE = 0,
    EXPLOSION,
} PARTICLE_TYPE;

typedef union PARTICLE_DATA {
    SMOKE_DATA smoke;
    EXPLOSION_DATA explosion;
} PARTICLE_DATA;

typedef struct PARTICLE {
    PARTICLE_DATA data;
    bool used;
    PARTICLE_TYPE type;
} PARTICLE;

#define MAX_PARTICLES 1024
extern PARTICLE particles[MAX_PARTICLES];

#endif

void update_particles();
VECTOR get_particle_position(PARTICLE* particle);
void add_smoke(VECTOR position, VECTOR motion, bool thick);
void add_explosion(VECTOR position, float max_radius, ALLEGRO_COLOR colour);