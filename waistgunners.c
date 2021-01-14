/*
    to compile:
    gcc -o waistgunners waistgunners.c -lm -lallegro -lallegro_acodec -lallegro_audio -lallegro_font -lallegro_image -lallegro_primitives
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

/* global variables, helper stuff */

#define PI 3.141592654

long frames, score;

typedef enum GAME_STATE {
    MAIN_MENU = 0,
    TUTORIAL_1,
    TUTORIAL_2,
    TUTORIAL_3,
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

typedef struct VECTOR {
    float x;
    float y;
} VECTOR;

float get_length(VECTOR* v) {
    return hypot(v->x, v->y);
}

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

VECTOR get_center(VECTOR position, VECTOR size) {
    return add(position, multiply(size, 0.5));
}

float angle_between(VECTOR a, VECTOR b) {
    float opp = b.y - a.y;
    float hyp = hypot(b.x - a.x, b.y - a.y);

    float angle = asin(opp / hyp);

    if (a.x > b.x) {
        angle = PI - angle;
    }
}

bool collision(VECTOR position, VECTOR size, VECTOR point) {
    return (
        point.x > position.x &&
        point.x < position.x + size.x &&
        point.y > position.y &&
        point.y < position.y + size.y
    );
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
VECTOR mouse_position;
ALLEGRO_MOUSE_STATE mouse_state;

void init_mouse() {
    mouse = 0;

    mouse_position.x = 0;
    mouse_position.y = 0;
}

void update_mouse() {
    al_get_mouse_state(&mouse_state);
    mouse_position.x = mouse_state.x / DISPLAY_SCALE;
    mouse_position.y = mouse_state.y / DISPLAY_SCALE;

    mouse = al_mouse_button_down(&mouse_state, 1);
}

/* sprite stuff --------------------------------------------------------------------------- */

const VECTOR BOMBER_SIZE = { .x = 64, .y = 59 };
const VECTOR ENGINE_SIZE = { .x = 7,  .y = 18 };

const VECTOR BULLET_SIZE = { .x = 4,  .y = 4 };

const VECTOR FIGHTER_SIZE         = { .x = 33, .y = 33 };
const VECTOR IMPOSTER_SIZE        = { .x = 66, .y = 60 };
const VECTOR IMPOSTER_ENGINE_SIZE = { .x = 8,  .y = 20 };
const VECTOR JET_SIZE             = { .x = 30, .y = 35 };

const VECTOR RETICLE_SIZE = { .x = 17, .y = 17 };

typedef struct SPRITES {
    ALLEGRO_BITMAP* spritesheet;

    ALLEGRO_BITMAP* bomber;
    ALLEGRO_BITMAP* bomber_engine;
    ALLEGRO_BITMAP* bomber_engine_damaged;
    ALLEGRO_BITMAP* bomber_engine_dead;

    ALLEGRO_BITMAP* player_bullet_1;
    ALLEGRO_BITMAP* player_bullet_2;
    ALLEGRO_BITMAP* enemy_bullet_1;
    ALLEGRO_BITMAP* enemy_bullet_2;

    ALLEGRO_BITMAP* fighter;
    ALLEGRO_BITMAP* imposter;
    ALLEGRO_BITMAP* imposter_engine;
    ALLEGRO_BITMAP* imposter_engine_damaged;
    ALLEGRO_BITMAP* imposter_engine_dead;
    ALLEGRO_BITMAP* jet_up;
    ALLEGRO_BITMAP* jet_down;

    ALLEGRO_BITMAP* reticle_aiming;
    ALLEGRO_BITMAP* reticle_firing;
} SPRITES;

SPRITES sprites;

ALLEGRO_BITMAP* get_sprite(int x, int y, VECTOR size) {
    // does all of the fancy casting stuff for us!
    int width  = (int) size.x;
    int height = (int) size.y;

    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites.spritesheet, x, y, width, height);
    must_init(sprite, "subsprite");
    return sprite;
}

void init_sprites() {
    must_init(al_init_image_addon(), "image addon");
    must_init(al_init_primitives_addon(), "primitives addon");
    sprites.spritesheet = al_load_bitmap("spritesheet.png");
    must_init(sprites.spritesheet, "main spritesheet");

    // load all of the sprites
    sprites.bomber                = get_sprite(0, 0, BOMBER_SIZE);
    sprites.bomber_engine         = get_sprite(0, 59, ENGINE_SIZE);
    sprites.bomber_engine_damaged = get_sprite(8, 59, ENGINE_SIZE);
    sprites.bomber_engine_dead    = get_sprite(16, 59, ENGINE_SIZE);

    sprites.player_bullet_1 = get_sprite(48, 75, BULLET_SIZE);
    sprites.player_bullet_2 = get_sprite(52, 75, BULLET_SIZE);
    sprites.enemy_bullet_1  = get_sprite(56, 75, BULLET_SIZE);
    sprites.enemy_bullet_2  = get_sprite(60, 75, BULLET_SIZE);

    sprites.fighter                 = get_sprite(130, 0, FIGHTER_SIZE);
    sprites.imposter                = get_sprite(64, 0, IMPOSTER_SIZE);
    sprites.imposter_engine         = get_sprite(24, 59, IMPOSTER_ENGINE_SIZE);
    sprites.imposter_engine_damaged = get_sprite(32, 59, IMPOSTER_ENGINE_SIZE);
    sprites.imposter_engine_dead    = get_sprite(40, 59, IMPOSTER_ENGINE_SIZE);
    sprites.jet_up                  = get_sprite(197, 0, JET_SIZE);
    sprites.jet_down                = get_sprite(167, 0, JET_SIZE);

    sprites.reticle_aiming = get_sprite(64, 62, RETICLE_SIZE);
    sprites.reticle_firing = get_sprite(81, 62, RETICLE_SIZE);
}

void destroy_sprites() {
    al_destroy_bitmap(sprites.bomber);
    al_destroy_bitmap(sprites.bomber_engine);
    al_destroy_bitmap(sprites.bomber_engine_damaged);
    al_destroy_bitmap(sprites.bomber_engine_dead);

    al_destroy_bitmap(sprites.player_bullet_1);
    al_destroy_bitmap(sprites.player_bullet_2);
    al_destroy_bitmap(sprites.enemy_bullet_1);
    al_destroy_bitmap(sprites.enemy_bullet_2);

    al_destroy_bitmap(sprites.fighter);
    al_destroy_bitmap(sprites.imposter);
    al_destroy_bitmap(sprites.imposter_engine);
    al_destroy_bitmap(sprites.imposter_engine_damaged);
    al_destroy_bitmap(sprites.imposter_engine_dead);
    al_destroy_bitmap(sprites.jet_up);
    al_destroy_bitmap(sprites.jet_down);

    al_destroy_bitmap(sprites.reticle_aiming);
    al_destroy_bitmap(sprites.reticle_firing);

    al_destroy_bitmap(sprites.spritesheet);
}

/* audio would go here... once I have that... --------------------------------------------------- */

/* smoke ---------------------------------------------------------------------------------------- */
#define SMOKE_LIFETIME 75

typedef struct SMOKE {
    // unlike most other things in the game, `position` here is the center of the particle, not the top left corner
    VECTOR position;
    VECTOR motion;
    int lifetime;
    bool dark;
} SMOKE;

void initialize_smoke(VECTOR position, VECTOR motion, SMOKE* s) {
    // extra processing
    s->position   = position;
    s->lifetime   = between(0, 37);
    VECTOR wobble = { .x = between_f(-0.1, 0.1), .y = between_f(-0.1, 0.1) };
    s->motion     = add(motion, wobble);
}

bool update_smoke(SMOKE* s) {
    s->position = add(s->position, s->motion);
    s->lifetime++;
    return s->lifetime > SMOKE_LIFETIME;
}

void draw_smoke(SMOKE* s) {
    float alpha  = 1.0 - ((float) s->lifetime) / ((float) SMOKE_LIFETIME);
    float radius = (1.0 - alpha) * 10;
    float colour = s->dark ? 0.0 : 0.5;

    al_draw_filled_circle(s->position.x, s->position.y, radius, al_map_rgb_f(colour, colour, colour));
}
/* particle union ------------------------------------------------------------------------------- */
#define MAX_PARTICLES 65536

typedef enum PARTICLE_TYPE {
    SMOKE_PARTICLE = 0
} PARTICLE_TYPE;

typedef union PARTICLE_DATA_UNION {
    SMOKE* smoke_data;
} PARTICLE_DATA_UNION;

typedef struct PARTICLE {
    bool used;
    PARTICLE_DATA_UNION data;
    PARTICLE_TYPE type;
} PARTICLE;

PARTICLE particles[MAX_PARTICLES];

void add_particle(VECTOR position, VECTOR motion, PARTICLE_DATA_UNION data, PARTICLE_TYPE type) {
    PARTICLE* p;
    for (int c = 0; c < MAX_PARTICLES; c++) {
        p = &particles[c];
        if (p->used) continue;
        p->used     = true;
        p->data     = data;
        p->type     = type;

        switch (type) {
            // particle specific initialization
            // every particle type MUST be here
            case SMOKE_PARTICLE:
                initialize_smoke(position, motion, data.smoke_data);
                break;
        }
        return;
    }

    puts("particle limit reached! cannot create a new particle!");
}

void update_particles() {
    PARTICLE* p;
    for (int c = 0; c < MAX_PARTICLES; c++) {
        p = &particles[c];
        if (!p->used) continue;
        switch (p->type) {
            // particle type specific behaviour
            // every particle type should be here
            case SMOKE_PARTICLE:
                p->used = update_smoke(p->data.smoke_data);
                if (!p->used) {
                    // hopefully it doesn't kill itself on this
                    free(p->data.smoke_data);
                }
                break;
        }
    }
}

void draw_particles() {
    PARTICLE* p;
    for (int c = 0; c < MAX_PARTICLES; c++) {
        p = &particles[c];
        if (!p->used) continue;
        switch (p->type) {
            // particle specific drawing
            // if a particle type is not here,
            // it will not be drawn!
            case SMOKE_PARTICLE:
                draw_smoke(p->data.smoke_data);
                break;
        }
    }
}

// wrapper for creating smoke particles using add_particle()
void add_smoke_particle(VECTOR position, VECTOR motion, bool dark) {
    PARTICLE_TYPE type       = SMOKE_PARTICLE;
    SMOKE* s                 = malloc(sizeof(SMOKE));
    s->dark                  = dark;
    PARTICLE_DATA_UNION data = { .smoke_data = s };

    add_particle(position, motion, data, type);
}

/* clouds --------------------------------------------------------------------------------------- */

/* bullet, player or enemy ---------------------------------------------------------------------- */

#define BULLET_SPEED 3
#define MAX_BULLETS  512 // bullet hell!

typedef enum BULLET_OWNER {
    PLAYER = 0,
    INCOMING, // "ENEMY" is reserved for the struct
} BULLET_OWNER;

typedef struct BULLET {
    // here, `position` is the center of the bullet
    // to get the top left corner, subtract 2 from both the x and y
    VECTOR position;
    VECTOR motion;
    bool used;
    BULLET_OWNER owner;
    int lifetime;
} BULLET;

BULLET bullets[MAX_BULLETS];

void add_bullet(VECTOR position, VECTOR motion, BULLET_OWNER owner) {
    BULLET* b;
    for (int c = 0; c < MAX_BULLETS; c++) {
        b = &bullets[c];
        if (b->used) continue;
        b->position = position;
        b->motion   = motion;
        b->owner    = owner;
        // that's it?
        return;
    }
}

void update_bullets() {
    BULLET* b;
    for (int c = 0; c < MAX_BULLETS; c++) {
        b = &bullets[c];
        if (!b->used) continue;
        b->position = add(b->position, multiply(b->motion, BULLET_SPEED));
        b->lifetime++;
    }
}

void draw_bullets() {
    BULLET* b;
    for (int c = 0; c < MAX_BULLETS; c++) {
        b = &bullets[c];
        if (!b->used) continue;
        ALLEGRO_BITMAP* sprite;
        VECTOR draw_position = add(b->position, multiply(BULLET_SIZE, -0.5));
        switch (b->owner) {
            case PLAYER:
                sprite = b->lifetime % 70 > 35 ? sprites.player_bullet_1 : sprites.player_bullet_2;
                break;
            case INCOMING:
                sprite = b->lifetime % 70 > 35 ? sprites.enemy_bullet_1 : sprites.enemy_bullet_2;
                break;
        }
        al_draw_bitmap(sprite, draw_position.x, draw_position.y, 0);
    }
}

/* player's engine ------------------------------------------------------------------------------- */

#define MAX_BOMBER_ENGINE_HEALTH 12

typedef struct BOMBER_ENGINE {
    VECTOR position;
    int health;
    bool dead;
} BOMBER_ENGINE;

BOMBER_ENGINE* create_bomber_engine(VECTOR position) {
    BOMBER_ENGINE* engine = malloc(sizeof(BOMBER_ENGINE));
    engine->position      = position;
    engine->health        = MAX_BOMBER_ENGINE_HEALTH;
    engine->dead          = false;

    return engine;
}

void update_bomber_engine(BOMBER_ENGINE* engine) {
    BULLET* b;
    for (int c = 0; c < MAX_BULLETS; c++) {
        // check for bullets colliding with the engine
        if (collision(engine->position, ENGINE_SIZE, b->position)) {
            if (engine->health) engine->health--;
            b->used = true;
        }
        if (between(0, MAX_BOMBER_ENGINE_HEALTH - 2) > engine->health) {
            VECTOR motion = { .x = 0, .y = 1 };
            add_smoke_particle(get_center(engine->position, ENGINE_SIZE), motion, engine->health < 4);
        }
    }
}

void draw_bomber_engine(BOMBER_ENGINE* engine) {
    ALLEGRO_BITMAP* sprite;
    if (engine->health > 8) {
        sprite = sprites.bomber_engine;
    } else if (engine->health > 0) {
        sprite = sprites.bomber_engine_damaged;
    } else {
        sprite = sprites.bomber_engine_dead;
    }
    al_draw_bitmap(sprite, engine->position.x, engine->position.y, 0);
}

/* player's gunner ------------------------------------------------------------------------------- */

#define GUNNER_LENGTH     3
#define GUNNER_INACCURACY PI / 50 // this actually helps the player, believe it or not
#define GUNNER_RELOAD     50

// a magenta-ish colour
// its value is set in `main()`
ALLEGRO_COLOR GUNNER_COLOUR;

typedef struct BOMBER_GUNNER {
    VECTOR position;
    int reload;
    bool active;
    float angle;
} BOMBER_GUNNER;

void update_bomber_gunner(BOMBER_GUNNER* gunner) {
    gunner->angle = angle_between(gunner->position, mouse_position);
}

void draw_bomber_gunner(BOMBER_GUNNER* gunner) {
    VECTOR gunner_dir  = { .x = cos(gunner->angle), .y = sin(gunner->angle) };
    VECTOR destination = add(gunner->position, multiply(gunner_dir, GUNNER_LENGTH));
    al_draw_line(gunner->position.x, gunner->position.y, destination.x, destination.y, GUNNER_COLOUR, 2);
}

/* player's bombers ------------------------------------------------------------------------------ */

VECTOR formation_position = { .x = BUFFER_WIDTH / 2 - 20, .y = BUFFER_HEIGHT / 2 };

const VECTOR BOMBER_ENGINES[2] = {
    { .x = 15, .y = 10 }, { .x = 41, .y = 10 }
};

const VECTOR BOMBER_GUNNERS[2] = {
    { .x = 23, .y = 35 }, { .x = 39, .y = 35 }
};

typedef struct BOMBER {
    VECTOR position;
    BOMBER_ENGINE* engines[2]; // array of BOMBER_ENGINE pointers, be careful
    BOMBER_GUNNER* gunners[2]; // array of BOMBER_GUNNER pointers, be careful
    bool down;
} BOMBER;

/* enemy fighter --------------------------------------------------------------------------------- */

const VECTOR FIGHTER_GUNS[2] = {
    { .x = 10, .y = 30 }, {.x = 25, .y = 30 }
};

/* enemy imposter gunner ------------------------------------------------------------------------- */

/* enemy imposter engine ------------------------------------------------------------------------- */

/* enemy imposter -------------------------------------------------------------------------------- */

const VECTOR IMPOSTER_ENGINES[2] = {
    { .x = 16, .y = 12 },
    { .x = 42, .y = 12 }
};

const VECTOR IMPOSTER_GUNNERS[2] = {
    { .x = 24, .y = 36 },
    { .x = 40, .y = 36 }
};

bool can_spawn_imposter = true;

/* enemy jet ------------------------------------------------------------------------------------- */

/* enemy, union ---------------------------------------------------------------------------------- */

#define MAX_ENEMIES 128 // uh oh

typedef enum ENEMY_TYPE {
    FIGHTER = 0,
    JET,
    IMPOSTER,
} ENEMY_TYPE;

typedef union ENEMY_DATA {

} ENEMY_DATA;

typedef struct ENEMY {
    bool used;
    ENEMY_TYPE type;
    ENEMY_DATA data;
} ENEMY;

ENEMY enemies[MAX_ENEMIES];

/* hud and debug mode ------------------------------------------------------------------------------------ */

bool debug;

long display_score = 0;

void update_hud() {

}

void draw_hud() {

}

/* tying everything together --------------------------------------------------------------------- */

bool done, redraw;

void reset() {

}

// these used to be in main()
// but i don't want four levels of indentation

void handle_special_keys() {
    // for pause, credits, etc.
}

void update_game() {
    // for updating the game for ALLEGRO_EVENT_TIMER
}

void draw() {
    // for drawing the entire game
}

int main() {
    // seed the rng
    time_t t;
    srand((unsigned) time(&t));
    // thanks, tutorialspoint!
    // https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm

    debug = false;

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 50.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "event queue");

    init_display();
    // image and primitives addons are initialized in init_sprites()
    init_sprites();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    init_keyboard();
    init_mouse();

    // some additional constants that we need to set
    GUNNER_COLOUR = al_map_rgb(176, 20, 85);

    // also initialize the hud
    // probably easier to just call reset()
    reset();

    done = false, redraw = false;

    ALLEGRO_EVENT event;
    al_start_timer(timer);

    while (1) {
        al_wait_for_event(queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                printf("key pressed: %i\n", event.keyboard.keycode);
                done = true;
                break;
        }

        if (done) break;
    }

    destroy_sprites();
    destroy_display();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}