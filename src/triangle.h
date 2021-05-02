#include "vector.h"
#include <math.h>

#ifndef _TRIANGLE
#define _TRIANGLE

typedef struct TRIANGLE {
    VECTOR vertices[3];
} TRIANGLE;

TRIANGLE create_triangle(VECTOR points[]);
float calculate_perimeter(TRIANGLE triangle);
float calculate_area(TRIANGLE triangle);
bool triangle_collision(TRIANGLE triangle, VECTOR point);

#endif