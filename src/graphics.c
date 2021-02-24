/*
    exactly what the file name is
    handles graphics -- both display and sprites
    also handles drawing, because C doesn't like it when i draw from another file
*/

#include "graphics.h"

ALLEGRO_DISPLAY* display;
ALLEGRO_BITMAP* buffer;

#define SMALL_FONT_SIZE 9
#define LARGE_FONT_SIZE 24

ALLEGRO_FONT* small_font;
ALLEGRO_FONT* large_font;

ALLEGRO_COLOR score_colour;
ALLEGRO_COLOR debug_colour;
ALLEGRO_COLOR gunner_colour;

ALLEGRO_PATH* path;

void init_display() {
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    // set the path
    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    must_init(al_change_directory(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP)), "path");
    al_destroy_path(path);
    path = 0;

    display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    must_init(display, "display");

    buffer = al_create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    must_init(display, "display");

    must_init(al_init_primitives_addon(), "primitives addon");

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    must_init(al_init_font_addon(), "font addon");
    must_init(al_init_ttf_addon(), "ttf addon");

    small_font = al_load_ttf_font("PressStart2P-Regular.ttf", SMALL_FONT_SIZE, ALLEGRO_TTF_MONOCHROME);
    must_init(small_font, "font");
    large_font = al_load_ttf_font("PressStart2P-Regular.ttf", LARGE_FONT_SIZE, ALLEGRO_TTF_MONOCHROME);
    must_init(large_font, "font");

    al_register_event_source(queue, al_get_display_event_source(display));

    score_colour  = al_map_rgb_f(1, 1, 0.75);
    debug_colour  = al_map_rgb_f(0.75, 0, 0);
    gunner_colour = al_map_rgb_f(1, 0, 0.9);
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

const VECTOR JET_SIZE             = { .x = 30, .y = 35 };

const VECTOR RETICLE_SIZE = { .x = 17, .y = 17 };
const VECTOR CLIP_SIZE    = { .x = 4,  .y = 10 };

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
    // again, first set the path
    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_change_directory(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
    al_destroy_path(path);
    path = 0;

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
    sprites.bullet_clip    = get_sprite(98, 69, CLIP_SIZE);
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
    al_destroy_bitmap(sprites.bullet_clip);

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
        for (int d = 0; d < GUNNERS_PER_BOMBER; d++) {
            GUNNER* g = b->gunners[d];
            VECTOR scaled = scale(subtract(get_mouse_position(), g->position), 3);
            VECTOR end    = add(g->position, scaled);
            al_draw_line(g->position.x, g->position.y, end.x, end.y, gunner_colour, 2);
        }
    }
}

void draw_enemy_fighter(ENEMY_FIGHTER_DATA* fighter) {
    if (fighter->dead) {
        VECTOR fighter_center = multiply(FIGHTER_SIZE, 0.5);
        VECTOR draw_center    = add(fighter->position, fighter_center);
        float scale           = 1 - (fabs(fighter->angle) / ENEMY_FIGHTER_MAX_ANGLE) * 50;

        al_draw_scaled_rotated_bitmap(sprites.fighter,
            fighter_center.x, fighter_center.y,
            draw_center.x, draw_center.y,
            scale, scale,
        fighter->angle, 0);
    } else {
        al_draw_bitmap(sprites.fighter, fighter->position.x, fighter->position.y, 0);
    }
}

void draw_enemy_imposter(ENEMY_IMPOSTER_DATA* imposter) {
    al_draw_bitmap(sprites.imposter, imposter->position.x, imposter->position.y, 0);

    for (int c = 0; c < ENGINES_PER_IMPOSTER; c++) {
        ENEMY_IMPOSTER_ENGINE* engine = imposter->engines[c];
        ALLEGRO_BITMAP* sprite;
        if (engine->health <= 0) {
            sprite = sprites.imposter_engine_dead;
        } else if (engine->health < 8) {
            sprite = sprites.imposter_engine_damaged;
        } else {
            sprite = sprites.imposter_engine;
        }
        al_draw_bitmap(sprite, engine->position.x, engine->position.y, 0);
    }

    for (int c = 0; c < GUNNERS_PER_IMPOSTER; c++) {
        ENEMY_IMPOSTER_GUNNER* gunner = imposter->gunners[c];
        VECTOR end                    = gunner->position;
        if (gunner->target == NULL) {
            end.y -= 3;
        } else {
            VECTOR scaled = scale(subtract(gunner->target->position, gunner->position), 3);
            end           = add(gunner->position, scaled);
        }
        al_draw_line(gunner->position.x, gunner->position.y, end.x, end.y, gunner_colour, 2);
    }
}

void draw_enemies() {
    for (int c = 0; c < MAX_ENEMIES; c++) {
        ENEMY* e = &enemies[c];
        if (!e->used) continue;
        switch (e->type) {
            case ENEMY_FIGHTER:
                draw_enemy_fighter(&(e->data.fighter));
                break;
            case ENEMY_IMPOSTER:
                draw_enemy_imposter(&(e->data.imposter));
                break;
            // more enemy types to come! promise!
        }
    }
}

void draw_bullets() {
    for (int c = 0; c < MAX_BULLETS; c++) {
        BULLET* b = &bullets[c];
        if (!b->used) continue;
        ALLEGRO_BITMAP* sprite;
        switch (b->alliance) {
            case PLAYER_BULLET:
                sprite = sprites.player_bullet_1;
                break;
            case ENEMY_BULLET:
                sprite = sprites.enemy_bullet_1;
                break;
        }
        al_draw_bitmap(sprite, b->position.x, b->position.y, 0);
    }
}

void draw_smoke_particle(SMOKE_DATA* smoke) {
    float alpha          = 1.0 - (float) smoke->lifetime / (float) MAX_SMOKE_LIFETIME;
    ALLEGRO_COLOR colour = smoke->thick ? al_map_rgba_f(0, 0, 0, alpha) : al_map_rgba_f(0.5, 0.5, 0.5, alpha);
    float radius         = (1.0 - alpha) * MAX_SMOKE_RADIUS;

    if (!smoke->thick) alpha = 1.0 - alpha;

    al_draw_filled_circle(smoke->position.x, smoke->position.y, radius, colour);
}

void draw_particles() {
    for (int c = 0; c < MAX_PARTICLES; c++) {
        PARTICLE* particle = &particles[c];
        if (!particle->used) continue;
        VECTOR position = get_particle_position(particle);
        switch (particle->type) {
            case SMOKE_PARTICLE:
                draw_smoke_particle(&(particle->data.smoke));
                break;
        }
    }
}

void draw_hud() {
    al_draw_bitmap(mouse ? sprites.reticle_firing : sprites.reticle_aiming,
        mouse_x - (RETICLE_SIZE.x + 1) / 2, mouse_y - (RETICLE_SIZE.y + 1) / 2, 0
    );

    al_draw_textf(small_font, al_map_rgb_f(1, 1, 0.5), 5, 5, 0, "%06ld", get_display_score());

    // draw the gunners' reloading
    // we first have to select a gunner that's from a bomber that's not down yet
    #define MAX_CLIPS_DRAWN 10
    GUNNER* gunner = select_gunner();
    if (gunner != NULL) {
        if (gunner->shots <= MAX_CLIPS_DRAWN) {
            for (int c = 0; c < gunner->shots; c++) {
                al_draw_bitmap(sprites.bullet_clip, c * CLIP_SIZE.x + 5, 20, 0);
            }
        } else {
            // draw a single bullet, and a number to indicate how many
            char indicator[7];
            sprintf(indicator, "x%i", gunner->shots);
            al_draw_bitmap(sprites.bullet_clip, 5, 20, 0);
            al_draw_text(small_font, al_map_rgb_f(0.5, 0.5, 0), 10 + CLIP_SIZE.x, 21, 0, indicator);
        }
        if (gunner->reload) {
            float reload_bar_max_width = (float) CLIP_SIZE.x * fmin(GUNNER_MAX_SHOTS, MAX_CLIPS_DRAWN);
            float proportion           = ((float) gunner->reload / (float) GUNNER_RELOAD);

            VECTOR start       = { .x = 5, .y = CLIP_SIZE.y + 23 };
            VECTOR destination = {
                .x = start.x + proportion * reload_bar_max_width,
                .y = start.y + 5
            };

            al_draw_filled_rectangle(start.x, start.y, destination.x, destination.y, al_map_rgb_f(0.5, 0.5, 0));
        }
    }
}

void draw_debug() {

}

void draw() {
    display_pre_draw();
    draw_enemies();
    draw_bombers();
    draw_bullets();
    draw_particles();
    draw_hud();
    display_post_draw();
}