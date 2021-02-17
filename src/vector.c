#include "vector.h"

VECTOR add(VECTOR a, VECTOR b) {
    VECTOR result = { .x = a.x + b.x, .y = a.y + b.y };
    return result;
}

VECTOR subtract(VECTOR a, VECTOR b) {
    VECTOR result = { .x = a.x - b.x, .y = a.y - b.y };
    return result;
}

VECTOR multiply(VECTOR a, float factor) {
    VECTOR result = { .x = a.x * factor, .y = a.y * factor };
    return result;
}

float get_length(VECTOR a) {
    return hypot(a.x, a.y);
}

bool collision(VECTOR pos1, VECTOR size1, VECTOR pos2, VECTOR size2) {
    if (pos1.x > pos2.x + size2.x) return false;
    if (pos1.y > pos2.y + size2.y) return false;
    if (pos2.x > pos1.x + size1.x) return false;
    if (pos2.y > pos1.y + size1.y) return false;
    return true;
}