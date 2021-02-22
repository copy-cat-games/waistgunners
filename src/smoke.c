#include "smoke.h"

void update_smoke(SMOKE_DATA* smoke) {
    smoke->lifetime++;
    smoke->position = add(smoke->position, smoke->motion);
    smoke->size     = (int) (((float) smoke->lifetime) / ((float) MAX_SMOKE_LIFETIME) * MAX_SMOKE_RADIUS * 100);
}