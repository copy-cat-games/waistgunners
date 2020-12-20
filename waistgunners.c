#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

// did i memorize that? yes. is it useless to memorize this many digits? yes. can these digits be wrong? yes.
// #define PI 3.141592653589793238462643383279502884197169399358209
#define PI 3.141592654

/*
    compile:
    gcc -o waistgunners waistgunners.c -lm -lallegro -lallegro_font -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_image
    x86_64-w64-mingw32-gcc -I/home/sugarleaf/Runtimes/allegro/include -B./libs -o waistgunners waistgunners.c -lm -l allegro -lallegro_font -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_image

*/

/* game helper stuff --------------------------------------------------------- */

long frames, score;

typedef enum GAME_STATE {
    MAIN_MENU = 0,
    CREDITS,
    PAUSED,
    PLAYING,
    GAME_OVER
} GAME_STATE;

int game_state;

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

bool collision(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2) {
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

float get_angle(int ax, int ay, int bx, int by) {
    float opp = by - ay;
    float hyp = hypot(bx - ax, by - ay);

    float angle = asin(opp / hyp);
    if (ax > bx) {
        angle = PI - angle;
    }
    while (angle < 0) {
        angle += PI * 2;
        // this is so that we get an angle between 0 and 2pi
        // we won't have to deal with negative angles, which makes things a lot easier
    }
    return angle;
}

void angle_between(float* angle, float maximum, float minimum) {
    float max = maximum;
    while (max < minimum) {
        max    += 2 * PI;
        *angle += 2 * PI;
    }

    if (*angle < minimum) {
        *angle = minimum;
    }
    if (*angle > max) {
        *angle = max;
    }
    while (*angle > 2 * PI) {
        *angle -= 2 * PI;
    }
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

unsigned int mouse = 0;
int mouse_x, mouse_y;
ALLEGRO_MOUSE_STATE mouse_state;

void init_mouse() {
    mouse   = 0;
    mouse_x = mouse_y = 0;
}

void update_mouse() {
    al_get_mouse_state(&mouse_state);
    mouse_x = mouse_state.x / DISPLAY_SCALE;
    mouse_y = mouse_state.y / DISPLAY_SCALE;
    mouse   = al_mouse_button_down(&mouse_state, 1);
}

/* sprite stuff ------------------------------------------------------------------- */

// this will change as i add more stuff to it. for now...

#define BOMBER_WIDTH   63
#define BOMBER_HEIGHT  58
#define ENGINE_WIDTH   7
#define ENGINE_HEIGHT  14
#define REDICLE_WIDTH  18
#define REDICLE_HEIGHT 18

#define ENEMY_FIGHTER_WIDTH  26
#define ENEMY_FIGHTER_HEIGHT 23

typedef struct SPRITES {
    ALLEGRO_BITMAP* _sprite_sheet;
    
    ALLEGRO_BITMAP* bomber;
    ALLEGRO_BITMAP* engine;
    ALLEGRO_BITMAP* redicle;
    ALLEGRO_BITMAP* redicle2;

    ALLEGRO_BITMAP* engine_dead;

    ALLEGRO_BITMAP* enemy_fighter;
    ALLEGRO_BITMAP* enemy_imposter;
    ALLEGRO_BITMAP* enemy_engine;
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
    
    sprites.bomber   = get_sprite(0, 30, BOMBER_WIDTH, BOMBER_HEIGHT);
    sprites.engine   = get_sprite(64, 30, ENGINE_WIDTH, ENGINE_HEIGHT);
    sprites.redicle  = get_sprite(46, 0, REDICLE_WIDTH, REDICLE_HEIGHT);
    sprites.redicle2 = get_sprite(65, 0, REDICLE_WIDTH, REDICLE_HEIGHT);

    sprites.engine_dead = get_sprite(72, 30, ENGINE_WIDTH, ENGINE_HEIGHT);
    
    sprites.enemy_fighter  = get_sprite(0, 0, ENEMY_FIGHTER_WIDTH, ENEMY_FIGHTER_HEIGHT);
    sprites.enemy_imposter = get_sprite(0, 88, BOMBER_WIDTH, BOMBER_HEIGHT);
    sprites.enemy_engine   = get_sprite(64, 88, ENGINE_WIDTH, ENGINE_HEIGHT);
}

void destroy_sprites() {
    al_destroy_bitmap(sprites.bomber);
    al_destroy_bitmap(sprites.engine);
    al_destroy_bitmap(sprites.redicle);
    al_destroy_bitmap(sprites.redicle2);

    al_destroy_bitmap(sprites.engine_dead);

    al_destroy_bitmap(sprites.enemy_fighter);
    al_destroy_bitmap(sprites.enemy_imposter);
    al_destroy_bitmap(sprites.enemy_engine);
    
    al_destroy_bitmap(sprites._sprite_sheet);
}

/* audio stuff --- i'll figure it out later --------------------------------------- */

/* effects -- i'll figure it out later ---------------------------------------------- */

#define MAX_NUMBER_OF_PARTICLES 65536

const int MAX_LIFETIMES[] = { 75 };

typedef enum PARTICLE_TYPE {
    SMOKE_PARTICLE = 0
} PARTICLE_TYPE;

typedef struct PARTICLE {
    float x, y;
    float dx, dy;
    int lifetime;
    PARTICLE_TYPE type;
    bool used;
    int data; // can be used for different things, for different particles
} PARTICLE;

PARTICLE particles[MAX_NUMBER_OF_PARTICLES];

void add_particle(float x, float y, float dx, float dy, PARTICLE_TYPE type, int data) {
    PARTICLE* p;
    for (int c = 0; c < MAX_NUMBER_OF_PARTICLES; c++) {
        if (particles[c].used) continue;
        p = &particles[c];

        p->x  = x + between(-2, 2);
        p->y  = y + between(-2, 2);
        p->dx = dx;
        p->dy = dy;

        p->type     = type;
        p->used     = true;
        p->data     = data;
        p->lifetime = 0;

        switch (p->type) {
            case SMOKE_PARTICLE:
                p->lifetime = between(0, MAX_LIFETIMES[0] / 2);
        }

        return;
    }
    printf("can't create particle! maximum reached!\n");
}

void update_particles() {
    PARTICLE* p;
    for (int c = 0; c < MAX_NUMBER_OF_PARTICLES; c++) {
        if (!particles[c].used) continue;
        p = &particles[c];
        p->x += p->dx, p->y += p->dy;
        p->lifetime++;
        if (p->lifetime > MAX_LIFETIMES[p->type]) {
            p->used = false;
        }
        switch (p->type) {
            // particle type specific behaviour goes here
            // not every particle type needs to be here
        }
    }
}

void draw_particles() {
    PARTICLE* p;

    // for smoke particles
    float alpha;
    float radius;
    float colour;

    for (int c = 0; c < MAX_NUMBER_OF_PARTICLES; c++) {
        if (!particles[c].used) continue;
        p = &particles[c];
        switch (p->type) {
            case SMOKE_PARTICLE:
                // for now, just draw a circle that gets more transparent as it floats on
                alpha  = 1.0 - (float) p->lifetime / (float) MAX_LIFETIMES[p->type];
                radius = (1.0 - alpha) * 10;
                colour = p->data ? 0.0 : 0.5;
                al_draw_filled_circle(p->x, p->y, radius, al_map_rgba_f(colour, colour, colour, alpha));
                break;
        }
    }
}

/* shots ------------------------------------------------------------------------ */
#define MAX_SHOTS 256
#define SHOT_SPEED 3
#define SHOT_SIZE 2

typedef struct SHOT {
    float x, y;
    float dx, dy;
    bool used;
    bool player;
} SHOT;

SHOT shots[MAX_SHOTS];

void add_shot(float x, float y, float dx, float dy, bool player) {
    SHOT* shot;
    for (int c = 0; c < MAX_SHOTS; c++) {
        if (shots[c].used) continue;
        shot       = &(shots[c]);
        shot->x    = x,  shot->y  = y;
        shot->dx   = dx, shot->dy = dy;
        shot->used = true;

        shot->player = player;
        return;
    }

    printf("maximum number of shots reached! unable to create shot!\n");
}

void update_shots() {
    SHOT* shot;
    for (int c = 0; c < MAX_SHOTS; c++) {
        if (!shots[c].used) continue;
        shot     = &(shots[c]);
        shot->x += shot->dx * SHOT_SPEED;
        shot->y += shot->dy * SHOT_SPEED;

        if (shot->x < 0 || shot->y < 0 || shot->x > BUFFER_WIDTH || shot->y > BUFFER_HEIGHT) {
            shot->used = false;
        }
    }
}

void draw_shots() {
    SHOT* shot;
    for (int c = 0; c < MAX_SHOTS; c++) {
        if (!shots[c].used) continue;
        shot = &(shots[c]);
        al_draw_line(
            shot->x, shot->y,
            shot->x - shot->dx * SHOT_SPEED,
            shot->y - shot->dy * SHOT_SPEED,
            shot->player ? al_map_rgb_f(1, 1, 0.5) : al_map_rgb_f(1, 0.75, 0), 2.0
        );
    }
}

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

ENGINE* select_random_engine() {
    ENGINE* e;
    for (int i = 0; i < 10; i++) {
        e = &engines[between(0, MAX_NUMBER_OF_ENGINES - 1)];
        if (e->used && !e->dead) {
            return e;
        }
    }
    return NULL;
}

bool engines_still_alive() {
    for (int c = 0; c < MAX_NUMBER_OF_ENGINES; c++) {
        ENGINE* e = &engines[c];
        if (e->used && !e->dead) return true;
    }
    return false;
}

void update_engines() {
    for (int c = 0; c < MAX_NUMBER_OF_ENGINES; c++) {
        if (!engines[c].used) continue;
        ENGINE* e = &engines[c];

        // check for collisions
        for (int d = 0; d < MAX_SHOTS; d++) {
            SHOT* s = &shots[d];
            if (!s->used || s->player) continue;
            if (
                collision(
                    e->x, e->y, e->x + ENGINE_WIDTH, e->y + ENGINE_HEIGHT,
                    s->x, s->y, s->x + 1, s->y + 1)
                && !e->dead) {
                e->health--;
                s->used = false;
            }
        }

        if (e->health <= 0) {
            e->dead = true;
        }
        
        if (between(0, ENGINE_MAX_HEALTH) > e->health) {
            add_particle(e->x + ENGINE_WIDTH / 2, e->y + ENGINE_HEIGHT / 2, between_f(-0.1, 0.1), 1, SMOKE_PARTICLE, e->health > 4 ? 0 : 1);
        }
    }
}

void draw_engines() {
    for (int c = 0; c < MAX_NUMBER_OF_ENGINES; c++) {
        if (!engines[c].used) continue;
        al_draw_bitmap(engines[c].dead ? sprites.engine_dead : sprites.engine, engines[c].x, engines[c].y, 0);
    }
}

/* gunners ---------------------------------------------------------------------- */

#define GUNNER_RELOAD 60
#define MAX_NUMBER_OF_GUNNERS 16
#define GUNNER_LENGTH 3
#define GUNNER_INACCURACY PI / 50 // PI / 50, or about 3.6 degrees in either direction

typedef struct GUNNER {
    int x, y;
    int shot_timer;
    bool used;
    bool active;

    float angle;
    float max_angle;
    float min_angle;
} GUNNER;

GUNNER gunners[MAX_NUMBER_OF_GUNNERS];

GUNNER* add_gunner(int x, int y, float min, float max) {
    GUNNER* gunner;
    for (int c = 0; c < MAX_NUMBER_OF_GUNNERS; c++) {
        if (gunners[c].used) continue;
        gunner             = &(gunners[c]);
        gunner->x          = x;
        gunner->y          = y;
        gunner->shot_timer = 0;
        gunner->max_angle  = max;
        gunner->min_angle  = min;
        gunner->angle      = between_f(min, max);

        gunner->used = gunner->active = true;
        return gunner;
    }
    printf("cannot add more gunners! maximum reached!\n");
    return NULL;
}

void update_gunners() {
    GUNNER* gunner;
    float shooting_angle;
    for (int c = 0; c < MAX_NUMBER_OF_GUNNERS; c++) {
        if (!gunners[c].active || !gunners[c].used) continue;
        gunner        = &(gunners[c]);
        gunner->angle = get_angle(gunner->x, gunner->y, mouse_x, mouse_y);
        // angle_between(&(gunner->angle), gunner->max_angle, gunner->min_angle);
        switch (gunner->shot_timer) {
            case 0:
                gunner->shot_timer = GUNNER_RELOAD;
            case 15:
            case 10:
            case 5:
                if (mouse) {
                    // fire a shot!
                    shooting_angle = gunner->angle + between_f(-GUNNER_INACCURACY, GUNNER_INACCURACY);
                    add_shot(gunner->x, gunner->y, cos(shooting_angle), sin(shooting_angle), true);
                }
            default:
                if (gunner->shot_timer) {
                    gunner->shot_timer--;
                }
        }
    }
}

void draw_gunners() {
    GUNNER* gunner;
    for (int c = 0; c < MAX_NUMBER_OF_GUNNERS; c++) {
        if (!gunners[c].active) continue;

        gunner = &(gunners[c]);
        al_draw_line(gunner->x, gunner->y,
            gunner->x + cos(gunner->angle) * GUNNER_LENGTH,
            gunner->y + sin(gunner->angle) * GUNNER_LENGTH,
            al_map_rgb_f(0, 0, 0), 2
        );
    }
}

/* bombers -------------------------------------------------------------- */
#define ENGINES_PER_BOMBER 2
#define GUNNERS_PER_BOMBER 2
#define BOMBER_SPEED       1
#define BOMBER_MAX_X       (BUFFER_WIDTH - BOMBER_WIDTH)
#define BOMBER_MAX_Y       (BUFFER_HEIGHT - BOMBER_HEIGHT);

typedef struct BOMBER {
    int x, y;
    ENGINE* engines[2];
    GUNNER* gunners[2];
    bool down;
    // now, all we're missing is the bomb bay and some bombs...
} BOMBER;

#define MAX_NUMBER_OF_BOMBERS 4

#define ENGINE_1_X 16
#define ENGINE_1_Y 14
#define ENGINE_2_X 40
#define ENGINE_2_Y 14

#define GUNNER_1_X 24
#define GUNNER_1_Y 33
#define GUNNER_2_X 38
#define GUNNER_2_Y 33

BOMBER bombers[MAX_NUMBER_OF_BOMBERS];
BOMBER player;

void init_bombers() {
    for (int c = 0; c < MAX_NUMBER_OF_BOMBERS; c++) {
        BOMBER* b = &(bombers[c]);
        int x = BUFFER_WIDTH / 2, y = BUFFER_HEIGHT / 2;
        switch (c) {
            // gotta fly in formation!
            // these numbers will probably need some tweaking
            case 0:
                break;
            case 1:
                x -= 40;
                y += 40;
                break;
            case 2:
                x -= 80;
                y += 80;
                break;
            case 3:
                x += 40;
                y += 40;
                break;
        }
        b->x = x; b->y = y;
        b->down = false;
        b->engines[0] = add_engine(x + ENGINE_1_X, y + ENGINE_1_Y);
        b->engines[1] = add_engine(x + ENGINE_2_X, y + ENGINE_2_Y);
        b->gunners[0] = add_gunner(x + GUNNER_1_X, y + GUNNER_1_Y, PI / 2, 3 * PI / 2);
        b->gunners[1] = add_gunner(x + GUNNER_2_X, y + GUNNER_2_Y, 3 * PI / 2, PI / 2);
    }
}

#define BOMBER_MARGIN 10 // somehow implement it so that the formation doesn't go too far off the screen

void move_bombers(int number_of_engines, int number_of_gunners, int dx, int dy) {
    if (frames % 2) return;
    // now, the way to do this would be to first calculate the future positions of our bombers
    // if any of the bombers would go too far offscreen, we cancel the movement.
    // otherwise, we move the bombers there
    // obviously, we'd check for downed bombers
    for (int c = 0; c < MAX_NUMBER_OF_BOMBERS; c++) {
        BOMBER* b = &bombers[c];
        if (b->down) continue;
        int future_x = b->x + dx; int future_y = b->y + dy;
        if (
            future_x < -BOMBER_MARGIN ||
            future_y < -BOMBER_MARGIN ||
            future_x + BOMBER_WIDTH > BUFFER_WIDTH + BOMBER_MARGIN ||
            future_y + BOMBER_HEIGHT > BUFFER_HEIGHT + BOMBER_MARGIN
        ) {
            // cancel the movement
            return;
        }
    }

    for (int i = 0; i < MAX_NUMBER_OF_BOMBERS; i++) {
        BOMBER* b = &bombers[i];
        if (b->down) continue;
        b->x += dx; b->y += dy;
        for (int c = 0; c < number_of_engines; c++) {
            ENGINE* e = b->engines[c];
            e->x += dx; e->y += dy;
        }
        for (int c = 0; c < number_of_gunners; c++) {
            GUNNER* g = b->gunners[c];
            g->x += dx; g->y += dy;
        }
    }
}

void update_bombers() {
    if (key[ALLEGRO_KEY_LEFT] || key[ALLEGRO_KEY_A]) {
        move_bombers(2, 2, -BOMBER_SPEED, 0);
    }
    if (key[ALLEGRO_KEY_RIGHT] || key[ALLEGRO_KEY_D]) {
        move_bombers(2, 2, BOMBER_SPEED, 0);
    }
    if (key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_W]) {
        move_bombers(2, 2, 0, -BOMBER_SPEED);
    }
    if (key[ALLEGRO_KEY_DOWN] || key[ALLEGRO_KEY_S]) {
        move_bombers(2, 2, 0, BOMBER_SPEED);
    }
    
    // remember to check for dead engines!
    for (int c = 0; c < MAX_NUMBER_OF_BOMBERS; c++) {
        BOMBER* b = &bombers[c];
        if (b->down && b->y < BUFFER_HEIGHT + 10 && frames % 3 == 0) {
            b->y += BOMBER_SPEED;
            
            b->engines[0]->y += BOMBER_SPEED;
            b->gunners[0]->y += BOMBER_SPEED;
            b->engines[1]->y += BOMBER_SPEED;
            b->gunners[1]->y += BOMBER_SPEED;
        }
        if (b->engines[0]->dead && b->engines[1]->dead && !b->down) {
            b->down = true;

            b->gunners[0]->active = false;
            b->gunners[1]->active = false;
        }
    }
}

void draw_bombers() {
    for (int c = 0; c < MAX_NUMBER_OF_BOMBERS; c++) {
        BOMBER* b = &(bombers[c]);
        if (b->y < BUFFER_HEIGHT) {
            al_draw_bitmap(sprites.bomber, b->x, b->y, 0);
        }
    }
}

/* enemies ---------------------------------------------------------------------- */

#define MAX_NUMBER_OF_ENEMIES     64
#define ENEMY_ALIGNMENT_THRESHOLD 10
#define ENEMY_INACCURACY          PI / 100

const int MAX_ENEMY_HEALTH[] = { 15, 10, 25 };
const int REWARDS[]          = { 100, 250, 500 }; // point awards for destroying enemies
const int ENEMY_SPEEDS[]     = { 2, 5, 1 }; // fighter is slow, jet is fast, destroyer is *very* slow
const int ENEMY_RELOADS[]    = { 25, 15, 20 }; // other two values are placeholders

const int ENEMY_FIGHTER_GUN_LEFT[]  = { 6, 21 };
const int ENEMY_FIGHTER_GUN_RIGHT[] = { 18, 21 };

typedef enum ENEMY_TYPE {
    ENEMY_TYPE_FIGHTER = 0,
    ENEMY_TYPE_JET,
    ENEMY_TYPE_IMPOSTER
    
    // more to come?
} ENEMY_TYPE;

typedef struct ENEMY {
    int x, y;
    int dx, dy;
    ENEMY_TYPE type;
    int health;
    int shot_timer;
    void* target;
    bool used;
    void* data;
    int reload;
    bool has_target;
} ENEMY;

ENEMY enemies[MAX_NUMBER_OF_ENEMIES];

typedef struct ENEMY_IMPOSTER_DATA {
    ENGINE* target_engine;
    int engine_health[2];
} ENEMY_IMPOSTER_DATA;

void add_enemy(int x, ENEMY_TYPE type) {
    // marvel at my terrible memory management!

    bool gun; // for the fighter type
    ENEMY_IMPOSTER_DATA data; // for the imposter type

    for (int c = 0; c < MAX_NUMBER_OF_ENEMIES; c++) {
        if (enemies[c].used) continue;
        ENEMY* e = &enemies[c];
        e->type  = type;
        e->used  = true;

        e->health = MAX_ENEMY_HEALTH[e->type];
        e->target = NULL;
        e->reload = between(0, ENEMY_RELOADS[e->type]);

        e->has_target = false;
        
        // enemy type specific stuff
        switch (e->type) {
            case ENEMY_TYPE_FIGHTER:
                e->data = malloc(sizeof(bool));
                gun     = true;
                e->x    = x;
                e->y    = -50;

                e->target = malloc(sizeof(ENGINE));

                *(bool*)(e->data) = gun;
                break;
            case ENEMY_TYPE_IMPOSTER:
                e->x = BUFFER_WIDTH / 2;
                e->y = BUFFER_HEIGHT + 10;

                e->data   = malloc(sizeof(ENEMY_IMPOSTER_DATA));
                e->target = malloc(sizeof(BOMBER));

                data.target_engine    = NULL;
                data.engine_health[0] = data.engine_health[1] = 10;

                *((ENEMY_IMPOSTER_DATA*) e->data) = data;
                break;
        }
        return;
    }
}

void update_enemies() {
    float angle;
    int gun_x, gun_y;
    for (int c = 0; c < MAX_NUMBER_OF_ENEMIES; c++) {
        if (!enemies[c].used) continue;
        ENEMY* e = &enemies[c];
        switch (e->type) {
            case ENEMY_TYPE_FIGHTER:
                // comes top down. aligns itself with your engines. slow
                /*
                    enemy fighter algorithm:
                    - if it is not 1/3 of the way down the screen, move forward
                    - if the target is NULL, select a new target AT RANDOM
                    - otherwise
                        - if the target's x is greater than fighter's x, set dx to fighter's speed
                        - if the target's x is less than the fighter's x, set dx to fighter's speed * -1
                        - if the target's x is equal to the fighter's x and if reload is zero, shoot
                            - get the angle from ENEMY_FIGHTER_GUNS[data] to the target
                            - add a shot to shots
                */
                e->dy = e->y == (BUFFER_WIDTH / 2) ? 0 : 1;
                if (!e->has_target || e->target == NULL) {
                    // choose a target
                    e->target     = select_random_engine();
                    e->has_target = true;
                } else {
                    // align itself with the target

                    // cast the void* to ENGINE* for the fighter type
                    ENGINE* target = (ENGINE*) e->target;
                    if ((target->x - e->x) > ENEMY_ALIGNMENT_THRESHOLD && e->y > 0) {
                        e->dx = ENEMY_SPEEDS[e->type];
                    }
                    if ((target->x - e->x) < -ENEMY_ALIGNMENT_THRESHOLD && e->y > 0) {
                        e->dx = -ENEMY_SPEEDS[e->type];
                    }
                    if (abs(target->x - e->x) < ENEMY_ALIGNMENT_THRESHOLD && (e->reload <= 0) && e->y + ENEMY_FIGHTER_HEIGHT > BUFFER_HEIGHT / 4) {
                        bool* gun = (bool*) e->data;
                        if (*gun) {
                            gun_x = ENEMY_FIGHTER_GUN_RIGHT[0];
                            gun_y = ENEMY_FIGHTER_GUN_RIGHT[1];
                        } else {
                            gun_x = ENEMY_FIGHTER_GUN_LEFT[0];
                            gun_y = ENEMY_FIGHTER_GUN_LEFT[1];
                        }
                        angle = get_angle(
                            e->x + gun_x, e->y + gun_y,
                            target->x + ENGINE_WIDTH / 2, target->y + ENGINE_HEIGHT / 2
                        ) + between_f(-ENEMY_INACCURACY, ENEMY_INACCURACY);
                        add_shot(e->x + gun_x, e->y + gun_y, cos(angle), sin(angle), false);

                        e->reload = ENEMY_RELOADS[e->type];

                        *gun = 1 - *gun;

                        printf("target engine is at (%i, %i)\n", target->x, target->y);

                        *(bool*)(e->data) = *gun;
                    } else {
                        e->reload--;
                    }
                    if (target->dead) {
                        e->target     = NULL;
                        e->has_target = false;
                    }
                }
                e->x += e->dx; e->dx = 0;
                if (frames % 2) e->y += e->dy;

                // remember to do collision detection!
                for (int d = 0; d < MAX_SHOTS; d++) {
                    SHOT* s = &shots[d];
                    if (s->used && s->player &&
                        s->x >= e->x && s->x <= (e->x + ENEMY_FIGHTER_WIDTH) &&
                        s->y >= e->y && s->y <= (e->y + ENEMY_FIGHTER_HEIGHT)
                    ) {
                        e->health -= 1;
                        s->used    = false;
                        // free(e->target);
                        // free(e->data);
                    }
                }
                if (between(0, MAX_ENEMY_HEALTH[e->type]) > e->health) {
                    add_particle(
                        e->x + ENEMY_FIGHTER_WIDTH / 2, e->y + ENEMY_FIGHTER_WIDTH / 2,
                        between_f(-0.1, 0.1), -1, SMOKE_PARTICLE, e->health > 10 ? 0 : 1
                    );
                }
                if (e->health <= 0) {
                    e->used = false;
                    score  += REWARDS[e->type];
                }
                break;
            case ENEMY_TYPE_JET:
                // comes from the corners? strafes?
                break;
            case ENEMY_TYPE_IMPOSTER:
                // flies into the formation, shoots from its two gunners, like the bomber
                // targets individual bombers. shoots their engines.
                // like the bomber, you must shoot the engines to down it.
                /*
                    enemy imposter algorithm:
                    - if the imposter is less than 60 pixels from the bottom of the screen, move up
                    - align itself with the middle of the screen
                    - choose a bomber
                    - shoot the bomber's engines with the imposter's gunners
                    - check for collisions with the engines, not the imposter itself
                */
                if (!e->has_target) {
                    // choose a target
                    // similar to select_random_engine()
                    for (int i = 0; i < MAX_NUMBER_OF_BOMBERS * 2; i++) {
                        BOMBER* b = &bombers[between(0, MAX_NUMBER_OF_BOMBERS)];
                        if (!b->down) {
                            *(BOMBER*)(e->target) = *b;
                            e->has_target         = true;
                            break;
                        }
                    }
                } else {
                    BOMBER* target           = (BOMBER*) e->target;
                    ENEMY_IMPOSTER_DATA data = *((ENEMY_IMPOSTER_DATA*) e->data);
                    // finish

                    // choose an engine that is not dead
                    if (data.target_engine == NULL || data.target_engine->dead) {
                        data.target_engine = target->engines[1]->dead ? target->engines[0] : target->engines[1];
                    }

                    // set the angle to target engine


                    // check if target is dead
                    if (target->down) {
                        e->has_target = false;
                    }
                    *(ENEMY_IMPOSTER_DATA*)(e->data) = data;
                }
                // movement
                if (e->y > BUFFER_HEIGHT - 60 && frames % 3) {
                    e->y--;
                }

                // check for collision
                // of course, like the bombers, we check the engines
                break;
        }
    }
}

void draw_enemies() {
    for (int c = 0; c < MAX_NUMBER_OF_ENEMIES; c++) {
        if (!enemies[c].used) continue;
        ENEMY* e = &enemies[c];
        switch (enemies[c].type) {
            case ENEMY_TYPE_FIGHTER:
                al_draw_bitmap(sprites.enemy_fighter, e->x, e->y, 0);
                break;
            case ENEMY_TYPE_JET:
            case ENEMY_TYPE_IMPOSTER:
                /*
                    draw the engines and gunners, too!
                */
                al_draw_bitmap(sprites.enemy_imposter, e->x, e->y, 0);
                ENEMY_IMPOSTER_DATA* data = (ENEMY_IMPOSTER_DATA*) e->data;
                al_draw_bitmap(
                    data->engine_health[0] ? sprites.enemy_engine : sprites.engine_dead,
                    e->x + ENGINE_1_X, e->y + ENGINE_1_Y, 0
                );
                al_draw_bitmap(
                    data->engine_health[1] ? sprites.enemy_engine : sprites.engine_dead,
                    e->x + ENGINE_2_X, e->y + ENGINE_2_Y, 0
                );

                // get the angles for both gunners
                float gunner_1_angle = data->target_engine == NULL ? 3 * PI / 2 : get_angle(e->x + GUNNER_1_X, e->y + GUNNER_1_Y, data->target_engine->x, data->target_engine->y);
                float gunner_2_angle = data->target_engine == NULL ? 3 * PI / 2 : get_angle(e->x + GUNNER_2_X, e->y + GUNNER_2_Y, data->target_engine->x, data->target_engine->y);
 
                al_draw_line(
                    e->x + GUNNER_1_X, e->y + GUNNER_1_Y,
                    e->x + GUNNER_1_X + cos(gunner_1_angle) * GUNNER_LENGTH, e->y + GUNNER_1_Y + sin(gunner_1_angle),
                    al_map_rgb_f(1, 0, 0), 2
                );
                al_draw_line(
                    e->x + GUNNER_2_X, e->y + GUNNER_2_Y,
                    e->x + GUNNER_2_X + cos(gunner_2_angle) * GUNNER_LENGTH, e->y + GUNNER_2_Y + sin(gunner_2_angle),
                    al_map_rgb_f(1, 0, 0), 2
                );
                break;
        }
    }
}

/* heads up display, probably in the top left corner --------------------- */

long display_score;
ALLEGRO_FONT* font;
bool debug;

ALLEGRO_COLOR PLAYER_DEBUG_COLOUR;
ALLEGRO_COLOR ENEMY_DEBUG_COLOUR;

void init_hud() {
    font = al_create_builtin_font();
    must_init(font, "font");

    display_score = 0;
    PLAYER_DEBUG_COLOUR = al_map_rgb_f(1, 1, 0);
    ENEMY_DEBUG_COLOUR  = al_map_rgb_f(0.6, 0, 0);
}

void update_hud() {
    if (frames % 5) return;
    for (long i = 5; i > 0; i--) {
        // not my algorithm
        long difference = 1 << i;
        if (display_score <= score - difference) {
            display_score += difference;
        }
    }
}

void draw_hud() {
    al_draw_bitmap(mouse ? sprites.redicle2 : sprites.redicle,
        mouse_x - (REDICLE_WIDTH / 2), mouse_y - (REDICLE_HEIGHT / 2), 0
    );

    if (debug) {
        ENEMY_IMPOSTER_DATA* data;

        // all of the debug drawing code
        al_draw_text(font, al_map_rgb(255, 0, 0), 2, 12, 0, "DEBUG MODE");

        // show the engines' hitboxes and health
        for (int i = 0; i < MAX_NUMBER_OF_ENGINES; i++) {
            ENGINE* e = &engines[i];
            if (!e->used) continue;
            al_draw_rectangle(e->x, e->y, e->x + ENGINE_WIDTH, e->y + ENGINE_HEIGHT, PLAYER_DEBUG_COLOUR, 2);
            al_draw_textf(font, PLAYER_DEBUG_COLOUR, e->x + ENGINE_WIDTH / 2, e->y - 10, ALLEGRO_ALIGN_CENTER, "%i", e->health);
        }
        
        // show the enemies' hitboxes and health
        for (int j = 0; j < MAX_NUMBER_OF_ENEMIES; j++) {
            ENEMY* e = &enemies[j];
            if (!e->used) continue;
            switch (e->type) {
                case ENEMY_TYPE_FIGHTER:
                    al_draw_rectangle(e->x, e->y, e->x + ENEMY_FIGHTER_WIDTH, e->y + ENEMY_FIGHTER_HEIGHT, ENEMY_DEBUG_COLOUR, 2);
                    al_draw_textf(font, ENEMY_DEBUG_COLOUR, e->x + ENEMY_FIGHTER_WIDTH / 2, e->y + ENEMY_FIGHTER_HEIGHT + 2, ALLEGRO_ALIGN_CENTER, "%i", e->health);
                    break;
                case ENEMY_TYPE_IMPOSTER:
                    // we'll want to also show the target engine

                    data = (ENEMY_IMPOSTER_DATA*) e->data;
                    if (data->target_engine != NULL) {
                        ENGINE* engine = data->target_engine;
                        al_draw_rectangle(engine->x, engine->y, engine->x + ENGINE_WIDTH, engine->y + ENGINE_HEIGHT, al_map_rgb_f(1, 0, 0), 2);
                    }
                    break;
            }
        }

        // draw the enemies' firing zone
        al_draw_line(0, BUFFER_HEIGHT / 4, BUFFER_WIDTH, BUFFER_HEIGHT / 4, al_map_rgb_f(1, 1, 1), 1);
    }

    if (game_state == PLAYING || game_state == PAUSED) {
        // show the player's score
        al_draw_textf(font, al_map_rgb(255, 255, 255), 2, 2, 0, "SCORE %06ld", display_score);
    }

    if (game_state == PAUSED) {
        // draw the words "PAUSED"
        al_draw_text(font, PLAYER_DEBUG_COLOUR, BUFFER_WIDTH / 2, BUFFER_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "PAUSED");
        al_draw_text(font, PLAYER_DEBUG_COLOUR, BUFFER_WIDTH / 2, BUFFER_HEIGHT / 2 + 12, ALLEGRO_ALIGN_CENTER, "press P to unpause");
    }

    if (game_state == MAIN_MENU) {
        // draw the splash sprite and the welcome text
        al_draw_filled_rounded_rectangle(25, 25, BUFFER_WIDTH - 25, BUFFER_HEIGHT / 4, 5, 5, al_map_rgb(20, 140, 20));
        al_draw_text(font, PLAYER_DEBUG_COLOUR, BUFFER_WIDTH / 2, 37, ALLEGRO_ALIGN_CENTER, "WAISTGUNNERS");

        if (frames % 75 > 37) {
            al_draw_text(font, PLAYER_DEBUG_COLOUR, BUFFER_WIDTH / 2, BUFFER_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "press ENTER to start");
        }
    }

    if (game_state == GAME_OVER) {
        if (frames % 75 > 37) {
            // blink!
            al_draw_text(font, al_map_rgb(255, 0, 0), BUFFER_WIDTH / 2, BUFFER_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER");
        }
        al_draw_textf(font, PLAYER_DEBUG_COLOUR, BUFFER_WIDTH / 2, BUFFER_HEIGHT / 2 + 12, ALLEGRO_ALIGN_CENTER, "SCORE: %06ld", score);
        // uncomment the line below when you've implemented restarting
        al_draw_text(font, al_map_rgb(255, 0, 0), BUFFER_WIDTH / 2, BUFFER_HEIGHT / 2 + 24, ALLEGRO_ALIGN_CENTER, "press ENTER to play again");
    }
    // remember to also display "G A M E   O V E R" when all four bombers have been shot down
    // if i know what i'm doing (and i don't), i might add high score functionality! no promises, though
}

/* main game loop -------------------------------------------------------- */

void reset() {
    // reset the game so that the player can play again!
    for (int a = 0; a < MAX_NUMBER_OF_ENGINES; a++) {
        engines[a].used = false;
    }
    for (int b = 0; b < MAX_NUMBER_OF_PARTICLES; b++) {
        particles[b].used = false;
    }
    for (int c = 0; c < MAX_SHOTS; c++) {
        shots[c].used = false;
    }
    for (int d = 0; d < MAX_NUMBER_OF_ENEMIES; d++) {
        enemies[d].used = false;
    }
    for (int e = 0; e < MAX_NUMBER_OF_GUNNERS; e++) {
        gunners[e].used = gunners[e].active = false;
    }
    display_score = score = frames = 0;
    init_engines();
    init_bombers();
}

int main() {
    // seed the rng
    time_t t;
    srand((unsigned) time(&t));
    // thanks, tutorialspoint!
    // https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm

    debug = true;

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
    init_engines();
    init_bombers();
    init_hud();

    al_hide_mouse_cursor(display);
    
    frames = 0;
    score  = 0;

    game_state = MAIN_MENU;
    
    bool done   = false;
    bool redraw = false;
    
    ALLEGRO_EVENT event;
    al_start_timer(timer);
    
    add_enemy(BUFFER_WIDTH / 2, ENEMY_TYPE_IMPOSTER); // remove when imposter is perfected
    
    while (1) {
        al_wait_for_event(queue, &event);
        
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                update_mouse();
                if (game_state == PLAYING || game_state == GAME_OVER) {
                    update_engines();
                    update_bombers();
                    update_shots();
                    update_gunners();
                    update_enemies();
                    update_particles();
                }
                
                if (frames % 150 == 1 && game_state == PLAYING) {
                    add_enemy(between(0, BUFFER_WIDTH), ENEMY_TYPE_FIGHTER);
                }

                if (!engines_still_alive() && game_state == PLAYING) {
                    game_state = GAME_OVER;
                }
                
                redraw = true;
                frames++;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
            // disable this block for releases
                if (event.keyboard.keycode == ALLEGRO_KEY_R) {
                    debug = !debug;
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_T && debug) {
                    add_enemy(BUFFER_WIDTH / 2, ENEMY_TYPE_FIGHTER);
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_P || event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    // let's do this Yanderedev style! IF STATEMENTS GALORE
                    if (game_state == PLAYING) {
                        game_state = PAUSED;
                    } else if (game_state == PAUSED) {
                        game_state = PLAYING;
                    }
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_Q && game_state != PLAYING) {
                    done = true;
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
                    // depends on the game state
                    switch (game_state) {
                        case MAIN_MENU:
                            game_state = PLAYING;
                            break;
                        case GAME_OVER:
                            reset();
                            game_state = MAIN_MENU;
                            break;
                    }
                }
                break;
        }
        if (done) break;
        
        update_keyboard(&event);
        
        if (redraw && al_is_event_queue_empty(queue)) {
            display_pre_draw();
            al_clear_to_color(al_map_rgb(143, 188, 143)); // darkseagreen
            
            draw_bombers();
            draw_engines();
            draw_gunners();
            draw_enemies();
            draw_shots();
            draw_particles();
            update_hud();
            draw_hud();
            
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
