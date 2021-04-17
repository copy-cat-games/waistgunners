/*
    exactly what the file name is
    handles graphics -- both display and sprites
    also handles drawing, because C doesn't like it when i draw from another file
*/

#include "graphics.h"

ALLEGRO_DISPLAY* display;
int landscape_horizontal_scroll = 0;

typedef enum BUFFERS {
    LANDSCAPE_BUFFER = 0,
    CLOUD_BUFFER,
    MAIN_BUFFER,
    NIGHT_BUFFER,
    BULLET_BUFFER,
    HUD_BUFFER,
    BUFFERS_N
} BUFFERS;

ALLEGRO_BITMAP* draw_buffers[BUFFERS_N];

#define TINY_FONT_SIZE  6
#define SMALL_FONT_SIZE 9
#define LARGE_FONT_SIZE 24

ALLEGRO_FONT* tiny_font;
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

    for (int c = 0; c < BUFFERS_N; c++) {
        draw_buffers[c] = al_create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
    }

    must_init(al_init_primitives_addon(), "primitives addon");

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    must_init(al_init_font_addon(), "font addon");
    must_init(al_init_ttf_addon(), "ttf addon");

    tiny_font  = al_load_ttf_font("PressStart2P-Regular.ttf", TINY_FONT_SIZE, ALLEGRO_TTF_MONOCHROME);
    must_init(tiny_font, "font");
    small_font = al_load_ttf_font("PressStart2P-Regular.ttf", SMALL_FONT_SIZE, ALLEGRO_TTF_MONOCHROME);
    must_init(small_font, "font");
    large_font = al_load_ttf_font("PressStart2P-Regular.ttf", LARGE_FONT_SIZE, ALLEGRO_TTF_MONOCHROME);
    must_init(large_font, "font");

    al_register_event_source(queue, al_get_display_event_source(display));

    score_colour  = al_map_rgb_f(1, 1, 0.75);
    debug_colour  = al_map_rgb(232, 234, 246);
    gunner_colour = al_map_rgb_f(1, 0, 0.9);
}

void destroy_display() {
    for (int c = 0; c < BUFFERS_N; c++) {
        al_destroy_bitmap(draw_buffers[c]);
    }
    al_destroy_display(display);
}

void display_pre_draw() {
    al_set_target_backbuffer(display);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    for (int c = 0; c < BUFFERS_N; c++) {
        // clear the buffers
        al_set_target_bitmap(draw_buffers[c]);
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    }
    al_set_target_bitmap(draw_buffers[MAIN_BUFFER]);
}

void display_post_draw(){
    al_set_target_backbuffer(display);
    for (int c = 0; c < BUFFERS_N; c++) {
        al_draw_scaled_bitmap(draw_buffers[c],
            0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,
            0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT,
        0);
    }
    al_flip_display();
}

const VECTOR JET_SIZE = { .x = 30, .y = 35 };

const VECTOR RETICLE_SIZE = { .x = 17, .y = 17 };
const VECTOR CLIP_SIZE    = { .x = 4,  .y = 10 };

const VECTOR LANDSCAPE_SIZE = { .x = 384, .y = 384 };

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

    // // once this is finalized, we'll put it in the main spritesheet
    // sprites.icon = al_load_bitmap("icon.png");
    // must_init(sprites.icon, "icon");

    // // once this is finalized, we'll put it in the main spritesheet
    // sprites.banner = al_load_bitmap("banner.png");
    // must_init(sprites.banner, "banner");

    sprites.landscape = get_sprite(0, 79, LANDSCAPE_SIZE);

    VECTOR small_cirrus_size         = { .x = 32, .y = 19 };
    sprites.small_cirrus             = get_sprite(227, 0, small_cirrus_size);
    VECTOR medium_stratoculumus_size = { .x = 62, .y = 41 };
    sprites.medium_stratoculumus     = get_sprite(260, 0, medium_stratoculumus_size);
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

    al_destroy_bitmap(sprites.icon);

    al_destroy_bitmap(sprites.banner);

    al_destroy_bitmap(sprites.landscape);

    al_destroy_bitmap(sprites.small_cirrus);
    al_destroy_bitmap(sprites.medium_stratoculumus);

    al_destroy_bitmap(sprites.spritesheet);
}

float landscape_scroll = 0;

void draw_landscape() {
    al_set_target_bitmap(draw_buffers[LANDSCAPE_BUFFER]);

    for (float c = landscape_scroll; c < BUFFER_HEIGHT; c += LANDSCAPE_SIZE.y) {
        al_draw_bitmap(sprites.landscape, -landscape_horizontal_scroll, (int) c, 0);
    }

    al_set_target_bitmap(draw_buffers[MAIN_BUFFER]);
    if (paused) return;
    landscape_scroll += 0.25;
    if (landscape_scroll >= 0) landscape_scroll -= LANDSCAPE_SIZE.y;
}

void draw_clouds() {
    al_set_target_bitmap(draw_buffers[CLOUD_BUFFER]);

    for (int c = 0; c < MAX_CLOUDS; c++) {
        CLOUD* cloud = &clouds[c];
        if (!cloud->used) continue;
        ALLEGRO_BITMAP* sprite;
        switch (cloud->type) {
            case SMALL_CIRRUS:
                sprite = sprites.small_cirrus;
                break;
            case MEDIUM_STRATOCULUMUS:
                sprite = sprites.medium_stratoculumus;
                break;
        }

        al_draw_bitmap(sprite, cloud->position.x, cloud->position.y, 0);
    }
    al_set_target_bitmap(draw_buffers[MAIN_BUFFER]);
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
            /* // could also do this
            switch (e->health) {
                case 0:
                    sprite = sprites.bomber_engine_dead;
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    sprite = sprites.bomber_engine_damaged;
                    break;
                default:
                    sprite = sprites.bomber_engine;
            } */
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
    al_set_target_bitmap(draw_buffers[BULLET_BUFFER]);
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
    al_set_target_bitmap(draw_buffers[MAIN_BUFFER]);
}

void draw_smoke_particle(SMOKE_DATA* smoke) {
    float alpha          = 1.0 - (float) smoke->lifetime / (float) MAX_SMOKE_LIFETIME;
    ALLEGRO_COLOR colour = smoke->thick ? al_map_rgba_f(0, 0, 0, alpha) : al_map_rgba_f(0.75, 0.75, 0.75, alpha);
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
    al_set_target_bitmap(draw_buffers[HUD_BUFFER]);
    al_draw_bitmap(mouse ? sprites.reticle_firing : sprites.reticle_aiming,
        mouse_x - (RETICLE_SIZE.x + 1) / 2, mouse_y - (RETICLE_SIZE.y + 1) / 2, 0
    );

    al_draw_textf(small_font, al_map_rgb_f(1, 1, 0.5), 5, 5, 0, "%06ld", get_display_score());

    // draw the gunners' reloading
    // we first have to select a gunner that's from a bomber that's not down yet
    #define MAX_CLIPS_DRAWN 10
    if (gunner_shots <= MAX_CLIPS_DRAWN) {
        for (int c = 0; c < gunner_shots; c++) {
            al_draw_bitmap(sprites.bullet_clip, c * CLIP_SIZE.x + 5, 20, 0);
        }
    } else {
        // draw a single bullet, and a number to indicate how many
        al_draw_bitmap(sprites.bullet_clip, 5, 20, 0);
        al_draw_textf(small_font, al_map_rgb_f(0.5, 0.5, 0), 10 + CLIP_SIZE.x, 21, 0, "x%i", gunner_shots);
    }
    if (gunner_reload) {
        float reload_bar_max_width = (float) CLIP_SIZE.x * fmin(GUNNER_MAX_SHOTS, MAX_CLIPS_DRAWN);
        float proportion           = ((float) gunner_reload / (float) GUNNER_RELOAD);

        VECTOR start       = { .x = 5, .y = CLIP_SIZE.y + 23 };
        VECTOR destination = {
            .x = start.x + proportion * reload_bar_max_width,
            .y = start.y + 5
        };

        al_draw_filled_rectangle(start.x, start.y, destination.x, destination.y, al_map_rgb_f(0.5, 0.5, 0));
    }
    al_set_target_bitmap(draw_buffers[MAIN_BUFFER]);
}

void draw_debug() {
    if (!debug) return;
    al_set_target_bitmap(draw_buffers[HUD_BUFFER]);

    // draw the engines' health and hitboxes
    for (int c = 0; c < MAX_BOMBERS; c++) {
        BOMBER* bomber = &bombers[c];
        if (bomber->position.y > BUFFER_HEIGHT) continue;
        for (int d = 0; d < ENGINES_PER_BOMBER; d++) {
            ENGINE* engine     = bomber->engines[d];
            VECTOR destination = add(engine->position, ENGINE_SIZE);
            al_draw_rectangle(engine->position.x, engine->position.y, destination.x, destination.y, debug_colour, 2);
            al_draw_textf(tiny_font, debug_colour, engine->position.x - 3, engine->position.y - (TINY_FONT_SIZE + 1), 0, "%i", engine->health);
        }
    }

    al_set_target_bitmap(draw_buffers[MAIN_BUFFER]);
}

void draw_night_overlay() {
    al_set_target_bitmap(draw_buffers[NIGHT_BUFFER]);
    ALLEGRO_COLOR night_colour = al_map_rgba(1, 1, 50, 200);
    al_draw_filled_rectangle(0, 0, BUFFER_WIDTH, BUFFER_HEIGHT, night_colour);
    al_set_target_bitmap(draw_buffers[MAIN_BUFFER]);
}

void draw() {
    display_pre_draw();
    draw_landscape();
    draw_clouds();
    draw_enemies();
    draw_bombers();
    if (!night) draw_bullets();
    draw_particles();
    if (night) {
        draw_night_overlay();
        draw_bullets();
    }
    draw_hud();
    draw_debug();
    display_post_draw();
}