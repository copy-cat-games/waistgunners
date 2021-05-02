#include "triangle.h"

TRIANGLE create_triangle(VECTOR points[]) {
    TRIANGLE t = {
        .vertices = {
            points[0], points[1], points[2]
            // make sure we don't accidentally include too many points!
            // otherwise, we'd have a _square_
        }
    };
    return t;
}

float calculate_perimeter(TRIANGLE triangle) {
    // using hypot
    float perimeter = 0;
    for (int c = 0; c < 3; c++) {
        VECTOR difference = subtract(triangle.vertices[c], triangle.vertices[(c + 1) % 3]);
        perimeter        += get_length(difference);
    }

    return perimeter;
}

float calculate_area(TRIANGLE triangle) {
    // use heron's formula
    float semiperimeter = calculate_perimeter(triangle);
    float product       = semiperimeter;
    for (int c = 0; c < 3; c++) {
        VECTOR difference = subtract(triangle.vertices[c], triangle.vertices[(c + 1) % 3]);
        product          *= semiperimeter - get_length(difference);
    }

    return sqrt(product);
}