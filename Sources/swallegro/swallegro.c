#include <stdio.h>
#include "include/swallegro.h"

/*
    what we do here is all of the leg work
    handling enemies, game content, etc are the jobs of the Swift code
*/

void must_init(bool test, char* description) {
    if (test) return;
    printf("failed to initialize %s.\n", description);
    exit(1);
}

void crash(const int exit_code, const char* msg) {
    printf("something went wrong: %s (exit code %i)\n", msg, exit_code);
    exit(exit_code);
}

ALLEGRO_DISPLAY* display;
ALLEGRO_BITMAP* buffer;
int display_scale;

void set_display_scale(const int scale) {
    display_scale = scale;
    printf("set the display scale to %i\n", scale);
}

void initialize_display() {

}