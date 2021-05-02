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
    // as usual, when times get tough, we steal code from someone else!

    return fabs(
        (triangle.vertices[1].x - triangle.vertices[0].x) * (triangle.vertices[2].y - triangle.vertices[0].y) -
        (triangle.vertices[2].x - triangle.vertices[0].x) * (triangle.vertices[1].y - triangle.vertices[0].y)
    );
}

bool triangle_collision(TRIANGLE triangle, VECTOR point) {
    // thanks to http://jeffreythompson.org/collision-detection/tri-point.php
    // what we do is create three triangles, each containing the point we are testing
    // if the three triangles' area add up to the big triangle's area, then we have collision

    float total_area = 0;
    for (int c = 0; c < 3; c++) {
        VECTOR points[] = {
            triangle.vertices[c], triangle.vertices[(c + 1) % 3], point
        };
        total_area += calculate_area(create_triangle(points));
    }

    return total_area == calculate_area(triangle);
}