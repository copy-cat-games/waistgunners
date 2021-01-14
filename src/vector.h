#include <stdbool.h>
#include <math.h>

#ifndef VECTOR_H
#define VECTOR_H
typedef struct VECTOR {
    float x; float y;
} VECTOR;
#endif

VECTOR add(VECTOR a, VECTOR b);
VECTOR subtract(VECTOR a, VECTOR b);
VECTOR multiply(VECTOR a, float factor);
float get_length(VECTOR a);
float angle_between(VECTOR a, VECTOR b);
bool collision(VECTOR pos1, VECTOR size1, VECTOR pos2, VECTOR size2);