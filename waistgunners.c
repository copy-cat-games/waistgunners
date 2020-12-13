#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

/*
    compile:
    gcc -o waistgunners waistgunners.c -lm -lallegro -lallegro_font -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_image
*/

/* game helper stuff --------------------------------------------------------- */

long frames, score;

void must_init(bool test, char* description) {
    if (test) return;
    printf("can't initialize %s!\n", description);
    exit(1);
}

int between(int lower, int higher) {
    return lower + (rand() % (higher - lower));
}

float between_f(float lower, float higher) {
    return lower + ((float) rand() / (float) RAND_MAX) * (higher - lower);
}

bool collision(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
    if (
        ax1 > bx2 ||
        ax2 < bx1 ||
        ay1 > by2 ||
        ay2 < by1
    ) {
        return false;
    }
    return true;
}

/* display stuff ------------------------------------------------------------------*/

#define BUFFER_WIDTH   200
#define BUFFER_HEIGHT  300
#define DISPLAY_SCALE  2
#define DISPLAY_WIDTH  (BUFFER_WIDTH * DISPLAY_SCALE)
#define DISPLAY_HEIGHT (BUFFER_HEIGHT * DISPLAY_SCALE)

ALLEGRO_DISPLAY* display;
ALLEGRO_BITMAP*  buffer;

void init_display() {
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    
    display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    must_init(display, "display");
    
    buffer = al_create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    must_init(buffer, "buffer");
}

void destroy_display() {
    al_destroy_bitmap(buffer);
    al_destroy_display(display);
}

void display_pre_draw() {
    al_set_target_bitmap(buffer);
}

void display_post_draw() {
    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(buffer,
        0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,
        0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT,
    0);
    al_flip_display();
}

/* keyboard stuff ----------------------------------------------------------------- */

#define KEY_SEEN     1
#define KEY_RELEASED 2

unsigned char key[ALLEGRO_KEY_MAX];

void init_keyboard() {
    memset(key, 0, sizeof(key));
}

void update_keyboard(ALLEGRO_EVENT* event) {
    switch (event->type) {
        case ALLEGRO_EVENT_TIMER:
            for (int c = 0; c < ALLEGRO_KEY_MAX; c++) {
                key[c] &= KEY_SEEN;
            }
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}

/* mouse stuff -------------------------------------------------------------------- */
// quite similar to the keyboard stuff, actually. might as well treat the mouse as a key instead

#define MOUSE_SEEN 1
#define MOUSE_RELEASED 2

unsigned int mouse = 0;
int mouse_x, mouse_y;
ALLEGRO_MOUSE_STATE mouse_state;

void init_mouse() {
    mouse_x = mouse_y = 0;
}

void update_mouse(ALLEGRO_EVENT* event) {
    al_get_mouse_state(&mouse_state);
    mouse_x = mouse_state.x / DISPLAY_SCALE;
    mouse_y = mouse_state.y / DISPLAY_SCALE;

    switch (event->type) {
        case ALLEGRO_EVENT_TIMER:
            mouse &= MOUSE_SEEN;
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            mouse = MOUSE_SEEN | MOUSE_RELEASED;
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            mouse &= MOUSE_RELEASED;
            break;
    }
}

/* sprite stuff ------------------------------------------------------------------- */

// this will change as i add more stuff to it. for now...

#define BOMBER_WIDTH   40
#define BOMBER_HEIGHT  29
#define ENGINE_WIDTH   4
#define ENGINE_HEIGHT  11
#define REDICLE_WIDTH  18
#define REDICLE_HEIGHT 18

typedef struct SPRITES {
    ALLEGRO_BITMAP* _sprite_sheet;
    
    ALLEGRO_BITMAP* bomber;
    ALLEGRO_BITMAP* engine;
    ALLEGRO_BITMAP* redicle;
} SPRITES;

SPRITES sprites;

ALLEGRO_BITMAP* get_sprite(int x, int y, int width, int height) {
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sprite_sheet, x, y, width, height);
    must_init(sprite, "subsprite");
    return sprite;
}

void init_sprites() {
    sprites._sprite_sheet = al_load_bitmap("spritesheet.png");
    must_init(sprites._sprite_sheet, "main spritesheet");
    
    sprites.bomber  = get_sprite(0, 0, BOMBER_WIDTH, BOMBER_HEIGHT);
    sprites.engine  = get_sprite(41, 0, ENGINE_WIDTH, ENGINE_HEIGHT);
    sprites.redicle = get_sprite(46, 0, REDICLE_WIDTH, REDICLE_HEIGHT);
}

void destroy_sprites() {
    al_destroy_bitmap(sprites.bomber);
    al_destroy_bitmap(sprites.engine);
    al_destroy_bitmap(sprites.redicle);
    
    al_destroy_bitmap(sprites._sprite_sheet);
}

/* audio stuff --- i'll figure it out later --------------------------------------- */

/* effects -- i'll figure it out later ---------------------------------------------- */

/* engines -- critical to your squad! ------------------------------------------------- */

#define ENGINE_MAX_HEALTH 12

typedef struct ENGINE {
    int health;
    int x, y;
    bool dead;
    bool used;
} ENGINE;

#define MAX_NUMBER_OF_ENGINES 32
// though... i'd like to see someone build an airplane with 32 powerful engines.
// boeing, airbus, northrop-grumman, lockheed-martin? can you guys build me an airplane with 32 jet engines? please?
ENGINE engines[MAX_NUMBER_OF_ENGINES];

void init_engines() {
    for (int c = 0; c < MAX_NUMBER_OF_ENGINES; c++) {
        engines[c].used = false;
    }
}

ENGINE* add_engine(int x, int y) {
    for (int c = 0; c < MAX_NUMBER_OF_ENGINES; c++) {
        if (engines[c].used) continue;
        ENGINE* e = &engines[c];
        e->x      = x;
        e->y      = y;
        e->health = ENGINE_MAX_HEALTH;
        e->dead   = false;
        e->used   = true;
        return e;
    }
    printf("unable to create an engine!\n");
    return NULL;
}

void update_engines() {
    for (int c = 0; c < MAX_NUMBER_OF_ENGINES; c++) {
        if (!engines[c].used) continue;
        ENGINE* e = &engines[c];
        if (e->health <= 0) {
            e->dead = true;
        }
        
        if ((frames / 5) % ENGINE_MAX_HEALTH < (ENGINE_MAX_HEALTH - e->health)) {
            // draw a smoke particle
        }
    }
}

void draw_engines() {
    for (int c = 0; c < MAX_NUMBER_OF_ENGINES; c++) {
        if (!engines[c].used) continue;
        al_draw_bitmap(sprites.engine, engines[c].x, engines[c].y, 0);
    }
}

/* shots ------------------------------------------------------------------------ */
#define MAX_SHOTS 256
#define SHOT_SPEED 3

typedef struct SHOT {
    int x, y;
    int dx, dy;
} SHOT;

void init_gunners() {

}

void update_shots() {

}

void draw_shots() {

}

/* gunners ---------------------------------------------------------------------- */

#define GUNNER_RELOAD 8
#define MAX_NUMBER_OF_GUNNERS 16

typedef struct GUNNER {
    int x, y;
    int shot_timer;
} GUNNER;

GUNNER gunners[MAX_NUMBER_OF_GUNNERS];

void update_gunners() {

}

void draw_gunners() {
    al_draw_bitmap(sprites.redicle, mouse_x - (REDICLE_WIDTH / 2), mouse_y - (REDICLE_HEIGHT / 2), 0);
}

/* bombers -------------------------------------------------------------- */
#define ENGINES_PER_BOMBER 2
#define BOMBER_SPEED       1
#define BOMBER_MAX_X       (BUFFER_WIDTH - BOMBER_WIDTH)
#define BOMBER_MAX_Y       (BUFFER_HEIGHT - BOMBER_HEIGHT);

typedef struct BOMBER {
    int x, y;
    ENGINE* engines[2];
    GUNNER* gunners[2];
    bool down;
} BOMBER;

#define MAX_NUMBER_OF_BOMBERS 4;
#define BOMBER_ENGINES { { 9, 3 }, { 23, 3 } };
// BOMBERS bombers[MAX_NUMBER_OF_BOMBERS];
BOMBER player;

void init_bombers() {
    // for now, just one
    player.x = (BUFFER_WIDTH - BOMBER_WIDTH) / 2;
    player.y = (BUFFER_HEIGHT - BOMBER_HEIGHT) / 2;
    
    player.engines[0] = add_engine(player.x + 9, player.y + 3);
    player.engines[1] = add_engine(player.x + 27, player.y + 3);
}

void move_bomber(BOMBER* b, int number_of_engines, int dx, int dy) {
    b->x += dx; b->y += dy;
    
    for (int c = 0; c < number_of_engines; c++) {
        ENGINE* e = b->engines[c];
        e->x += dx; e->y += dy;
    }
}

void update_bombers() {
    if (key[ALLEGRO_KEY_LEFT] || key[ALLEGRO_KEY_A]) {
        move_bomber(&player, 2, -BOMBER_SPEED, 0);
    }
    if (key[ALLEGRO_KEY_RIGHT] || key[ALLEGRO_KEY_D]) {
        move_bomber(&player, 2, BOMBER_SPEED, 0);
    }
    if (key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_W]) {
        move_bomber(&player, 2, 0, -BOMBER_SPEED);
    }
    if (key[ALLEGRO_KEY_DOWN] || key[ALLEGRO_KEY_S]) {
        move_bomber(&player, 2, 0, BOMBER_SPEED);
    }
    
    // remember to check for dead engines!
}

void draw_bombers() {
    al_draw_bitmap(sprites.bomber, player.x, player.y, 0);
}

/* main game loop -------------------------------------------------------- */

void reset() {
    // reset the game so that the player can play again!
}

int main() {
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");
    
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 50.0);
    must_init(timer, "timer");
    
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "event queue");
    
    init_display();
    must_init(al_init_image_addon(), "image addon");
    init_sprites();
    
    must_init(al_init_primitives_addon(), "primitives");
    // remember to install audio and other fun things
    
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    
    init_keyboard();
    init_mouse();
    init_gunners();
    init_engines();
    init_bombers();

    al_hide_mouse_cursor(display);
    
    frames = 0;
    score  = 0;
    
    bool done   = false;
    bool redraw = false;
    
    ALLEGRO_EVENT event;
    al_start_timer(timer);
    
    while (1) {
        al_wait_for_event(queue, &event);
        
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                update_engines();
                update_bombers();
                
                if (key[ALLEGRO_KEY_ESCAPE]) {
                    done = true;
                }
                
                redraw = true;
                frames++;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }
        if (done) break;
        
        update_keyboard(&event);
        update_mouse(&event);
        
        if (redraw && al_is_event_queue_empty(queue)) {
            display_pre_draw();
            al_clear_to_color(al_map_rgb(255, 255, 255));
            
            draw_bombers();
            draw_engines();
            draw_gunners();
            
            display_post_draw();
            redraw = false;
        }
    }
    
    destroy_sprites();
    destroy_display();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    
    return 0;
}
