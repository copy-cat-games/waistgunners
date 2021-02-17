/*
    exactly what the file name is
    handles graphics -- both display and sprites
    also handles drawing, because C doesn't like it when i draw from another file
*/

#include "graphics.h"

ALLEGRO_DISPLAY* display;
ALLEGRO_BITMAP* buffer;

void init_display() {
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    must_init(display, "display");

    buffer = al_create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    must_init(display, "display");

    must_init(al_init_primitives_addon(), "primitives addon");

    al_register_event_source(queue, al_get_display_event_source(display));
}

void destroy_display() {
    al_destroy_bitmap(buffer);
    al_destroy_display(display);
}

void display_pre_draw() {
    al_set_target_bitmap(buffer);
    al_clear_to_color(al_map_rgb(143, 188, 143)); // darkseagreen
}

void display_post_draw(){
    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(buffer,
        0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,
        0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT,
        0);
    al_flip_display();
}

// const VECTOR BOMBER_SIZE = { .x = 64, .y = 59 };
const VECTOR ENGINE_SIZE = { .x = 7,  .y = 18 };

const VECTOR BULLET_SIZE = { .x = 4,  .y = 4 };

const VECTOR FIGHTER_SIZE         = { .x = 33, .y = 33 };
const VECTOR IMPOSTER_SIZE        = { .x = 66, .y = 60 };
const VECTOR IMPOSTER_ENGINE_SIZE = { .x = 8,  .y = 20 };
const VECTOR JET_SIZE             = { .x = 30, .y = 35 };

const VECTOR RETICLE_SIZE = { .x = 17, .y = 17 };

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

void draw_bombers() {
    for (int c = 0; c < MAX_BOMBERS; c++) {
        BOMBER* b = &bombers[c];
        if (b->position.y > BUFFER_HEIGHT) continue;

        al_draw_bitmap(sprites.bomber, b->position.x, b->position.y, 0);
        for (int d = 0; d < ENGINES_PER_BOMBER; d++) {
            ENGINE* e = b->engines[d];
            ALLEGRO_BITMAP* sprite;
            //mountain of if statements, Yanderedev style!
            if (e->health <= 0) {
                sprite = sprites.bomber_engine_dead;
            } else if (e->health < 8) {
                sprite = sprites.bomber_engine_damaged;
            } else {
                sprite = sprites.bomber_engine;
            }
            al_draw_bitmap(sprite, e->position.x, e->position.y, 0);
        }
    }
}

void draw_hud() {
    al_draw_bitmap(mouse ? sprites.reticle_firing : sprites.reticle_aiming,
        mouse_x - (RETICLE_SIZE.x + 1) / 2, mouse_y - (RETICLE_SIZE.y + 1) / 2, 0
    );
}

void draw_debug() {

}

void draw() {
    display_pre_draw();
    draw_bombers();
    draw_hud();
    display_post_draw();
}