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
        switch (p->type) {
            case SMOKE_PARTICLE:
                ;
                SMOKE_DATA* smoke = &(p->data.smoke);
                update_smoke(smoke);
                if (smoke->lifetime > MAX_SMOKE_LIFETIME) {
                    p->used = false;
                }
                break;
        }
    }
}

void add_particle(PARTICLE_DATA data, PARTICLE_TYPE type) {
    for (int c = 0; c < MAX_PARTICLES; c++) {
        PARTICLE* p = &particles[c];
        if (p->used) continue;
        p->data = data;
        p->type = type;
        p->used = true;
        return;
    }
}

void add_smoke(VECTOR position, VECTOR motion, bool thick) {
    VECTOR random_motion = { .x = between(-0.1, 0.1), .y = between(-0.1, 0.1) };
    VECTOR smoke_motion  = add(motion, random_motion);

    SMOKE_DATA smoke_data = {
        .position = position,
        .motion   = smoke_motion,
        .lifetime = rand() % (MAX_SMOKE_LIFETIME / 2),
        .size     = 0,
        .thick    = thick
    };

    PARTICLE_DATA data = { .smoke = smoke_data };

    add_particle(data, SMOKE_PARTICLE);
}