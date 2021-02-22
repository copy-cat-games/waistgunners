#include "particle.h"

PARTICLE particles[MAX_PARTICLES];

VECTOR get_particle_position(PARTICLE* particle) {
    VECTOR position = { .x = -3000, .y = -3000 };
    switch (particle->type) {
        case SMOKE_PARTICLE:
            return particle->data.smoke.position;
    }

    return position;
}

void update_particles() {
    for (int c = 0; c < MAX_PARTICLES; c++) {
        PARTICLE* p = &particles[c];
        if (!p->used) continue;
    }
}