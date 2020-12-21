#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <math.h>

/*
    general stuff
    - [x] crashing (you never know!)
*/

void crash(const int exit_code, const char* msg);

/*
    display and graphics stuff
    - [ ] initializing the display and buffer
    - [x] setting the display and buffer
    - [ ] cleaning up the display and buffer
    - [ ] drawing various things to the display
        - [ ] rectangles (filled and unfilled)
        - [ ] circles
        - [ ] lines
*/
void set_display_scale(int scale);