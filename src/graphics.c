/*
    exactly what the file name is
    handles graphics -- both display and sprites
    also handles drawing, because C doesn't like it when i draw from another file
*/

#include "graphics.h"
#include "credits.h"

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

ALLEGRO_COLOR credits_background_colour;
ALLEGRO_COLOR ui_background_colour;
ALLEGRO_COLOR power_up_bar_colour;

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

    credits_background_colour = al_map_rgb(46, 139, 87); // x11: "seagreen"
    ui_background_colour      = al_map_rgba_f(0, 0, 0, 0.8);
    power_up_bar_colour       = al_map_rgb(190, 190, 190); //al_map_rgb(190, 190, 190); // matches the background colour of the powerup
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
    sprites.missile                 = get_sprite(130, 32, MISSILE_SIZE);
    
    for (int c = 0; c < 4; c++) {
        sprites.missile_fire[c] = get_sprite(130 + c * 10, 53, MISSILE_FIRE_SIZE);
    }

    sprites.reticle_aiming = get_sprite(64, 62, RETICLE_SIZE);
    sprites.reticle_firing = get_sprite(81, 62, RETICLE_SIZE);
    sprites.bullet_clip    = get_sprite(98, 69, CLIP_SIZE);

    sprites.landscape = get_sprite(0, 79, LANDSCAPE_SIZE);

    sprites.cloud_small = get_sprite(0, 465, CLOUD_SIZES[SMALL]);
    sprites.cloud_wide  = get_sprite(61, 465, CLOUD_SIZES[WIDE]);
    sprites.cloud_wispy = get_sprite(239, 465, CLOUD_SIZES[WISPY]);
    sprites.cloud_light = get_sprite(0, 554, CLOUD_SIZES[LIGHT]);

    sprites.power_up_bigger_clip_size = get_sprite(102, 62, POWER_UP_SIZE);
    sprites.power_up_faster_reload    = get_sprite(136, 62, POWER_UP_SIZE);
    sprites.power_up_faster_bullets   = get_sprite(119, 62, POWER_UP_SIZE);
    sprites.power_up_destroy_enemies  = get_sprite(153, 62, POWER_UP_SIZE);
    sprites.power_up_repair_engine    = get_sprite(170, 62, POWER_UP_SIZE);
    sprites.power_up_missing          = get_sprite(187, 62, POWER_UP_SIZE);
    sprites.power_up_invincibility    = get_sprite(204, 62, POWER_UP_SIZE);
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
    al_destroy_bitmap(sprites.missile);
    
    for (int c = 0; c < 4; c++) {
        al_destroy_bitmap(sprites.missile_fire[c]);
    }

    al_destroy_bitmap(sprites.reticle_aiming);
    al_destroy_bitmap(sprites.reticle_firing);
    al_destroy_bitmap(sprites.bullet_clip);

    al_destroy_bitmap(sprites.icon);

    al_destroy_bitmap(sprites.banner);

    al_destroy_bitmap(sprites.landscape);

    al_destroy_bitmap(sprites.cloud_small);
    al_destroy_bitmap(sprites.cloud_wide);
    al_destroy_bitmap(sprites.cloud_wispy);
    al_destroy_bitmap(sprites.cloud_light);

    al_destroy_bitmap(sprites.power_up_bigger_clip_size);
    al_destroy_bitmap(sprites.power_up_faster_reload);
    al_destroy_bitmap(sprites.power_up_faster_bullets);
    al_destroy_bitmap(sprites.power_up_destroy_enemies);
    al_destroy_bitmap(sprites.power_up_repair_engine);
    al_destroy_bitmap(sprites.power_up_missing);
    al_destroy_bitmap(sprites.power_up_invincibility);

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
            case SMALL:
                sprite = sprites.cloud_small;
                break;
            case WIDE:
                sprite = sprites.cloud_wide;
                break;
            case WISPY:
                sprite = sprites.cloud_wispy;
                break;
            case LIGHT:
                sprite = sprites.cloud_light;
                break;
            default:
                sprite = sprites.cloud_small;
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

void draw_enemy_jet(ENEMY_JET_DATA* jet) {
    ALLEGRO_BITMAP* sprite = jet->direction == DOWN ? sprites.jet_down : sprites.jet_up;
    al_draw_bitmap(sprite, jet->position.x, jet->position.y, 0);
}

void draw_enemy_missile(ENEMY_MISSILE_DATA* missile) {
    al_draw_bitmap(sprites.missile, missile->position.x, missile->position.y, 0);
    int counter = frames % 32;
    ALLEGRO_BITMAP* fire_sprite;
    VECTOR draw_fire_position = add(missile->position, MISSILE_FIRE_OFFSET);
    if (counter < 12 ) {
        fire_sprite = sprites.missile_fire[0];
    } else if (counter < 20) {
        fire_sprite = sprites.missile_fire[1];
    } else if (counter < 24) {
        fire_sprite = sprites.missile_fire[2];
    } else {
        fire_sprite = sprites.missile_fire[3];
    }
    al_draw_bitmap(fire_sprite, draw_fire_position.x, draw_fire_position.y, 0);
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
            case ENEMY_JET:
                draw_enemy_jet(&(e->data.jet));
                break;
            case ENEMY_MISSILE:
                draw_enemy_missile(&(e->data.missile));
                break;
            // more enemy types to come! promise!
        }
    }
}

void draw_floating_power_ups() {
    // not sure if this is needed
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

    //  if (!smoke->thick) alpha = 1.0 - alpha;

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

void draw_button(BUTTON button) {
    if (!button.text) return;

    ALLEGRO_COLOR button_colour;

    if (is_hovering(button)) {
        // i'll change the colour later
        button_colour = BUTTON_HOVER_COLOUR;
    } else {
        button_colour = BUTTON_COLOUR;
    }

    VECTOR button_position = get_button_position(button);

    al_draw_filled_rounded_rectangle(
        button_position.x, button_position.y,
        button_position.x + BUTTON_SIZE.x, button_position.y + BUTTON_SIZE.y,
        BUTTON_CORNER_RADIUS, BUTTON_CORNER_RADIUS, button_colour
    );

    VECTOR text_position = add(button_position, BUTTON_TEXT_OFFSET);

    al_draw_textf(small_font, BUTTON_TEXT_COLOUR, text_position.x, text_position.y, 0, button.text);
}

void draw_hud() {
    al_set_target_bitmap(draw_buffers[HUD_BUFFER]);

    if (game_state == CREDITS) {
        int credits_background_end = 60 + 20 * CREDITS_N;
        al_draw_filled_rounded_rectangle(10, 50, 190, credits_background_end, BUTTON_CORNER_RADIUS, BUTTON_CORNER_RADIUS, credits_background_colour);

        for (int c = 0; c < CREDITS_N; c++) {
            int y = 55 + c * (int) floor(SMALL_FONT_SIZE * 1.5);
            al_draw_text(small_font, score_colour,
                c % 2 ? 25 : 15, 55 + c * 20,
                0, credits[c]
            );
        }
    }

    if (game_state == GAME_OVER) {
        al_draw_filled_rounded_rectangle(50, 50, 150, 200, 3, 3, credits_background_colour);
        if (frames % 100 > 50) {
            al_draw_text(large_font, score_colour, 52, 55, 0, "GAME");
            al_draw_text(large_font, score_colour, 52, 100, 0, "OVER");
        }

        al_draw_textf(small_font, score_colour, 70, 150, 0, "%06ld", score);
    }

    // if you're wondering where the code to draw the main menu is,
    // there isn't any. yet. i still need a banner for the game

    // draws buttons
    // no matter what the game state is
    for (int c = 0; c < MAX_BUTTONS; c++) {
        draw_button(menu_buttons[c]);
    }

    al_draw_bitmap(mouse ? sprites.reticle_firing : sprites.reticle_aiming,
        mouse_x - (RETICLE_SIZE.x + 1) / 2, mouse_y - (RETICLE_SIZE.y + 1) / 2, 0
    );

    // draw the gunners' reloading
    // we first have to select a gunner that's from a bomber that's not down yet
    #define MAX_CLIPS_DRAWN 10
    if (game_state == PLAYING) {
        // so that the score counter and reloading bar are easier to see
        al_draw_filled_rounded_rectangle(0, 0, 63, 40, 3, 3, ui_background_colour);

        al_draw_textf(small_font, score_colour, 5, 5, 0, "%06ld", get_display_score());

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

        // a box to contain the powerup elements
        al_draw_filled_rounded_rectangle(0, 41, 63, 93, 3, 3, ui_background_colour);
        for (int c = 0; c < MAX_STORED_POWER_UPS; c++) {
            POWER_UP p = stored_power_ups[c];
            if (!p.lifetime || p.type == NONE) continue;
            int draw_y = 42 + c * (POWER_UP_SIZE.y + 1);
            ALLEGRO_BITMAP* sprite;
            switch (p.type) {
                case BIGGER_CLIP_SIZE:
                    sprite = sprites.power_up_bigger_clip_size;
                    break;
                case FASTER_RELOAD:
                    sprite = sprites.power_up_faster_reload;
                    break;
                case FASTER_BULLETS:
                    sprite = sprites.power_up_faster_bullets;
                    break;
                case DESTROY_ENEMIES:
                    sprite = sprites.power_up_destroy_enemies;
                    break;
                case REPAIR_ENGINE:
                    sprite = sprites.power_up_repair_engine;
                    break;
                case TEMPORARY_INVINCIBILITY:
                    sprite = sprites.power_up_invincibility;
                    break;
                default:
                    sprite = sprites.power_up_missing;
                // more powerups to be added...
            }
            al_draw_bitmap(sprite, 1, draw_y, 0);
            
            // also draw a bar showing how much time is left
            float proportion = (float) p.lifetime / ((float) MAX_POWER_UP_LIFETIMES[p.type]);

            VECTOR start = { .x = 18, .y = draw_y + 5 };
            VECTOR end   = { .x = 18 + proportion * 44, .y = draw_y + 11 };
            al_draw_filled_rectangle(start.x, start.y, end.x, end.y, power_up_bar_colour);
        }

        if (paused && game_state == PLAYING) {
            // draw a small box that tells the player that the game is paused
            // it's obvious, but you never know!
            al_draw_filled_rounded_rectangle(70, 100, 130, 120, 3, 3, ui_background_colour);
            al_draw_text(small_font, score_colour, 73, 105, 0, "paused");
        }
    }

    al_set_target_bitmap(draw_buffers[MAIN_BUFFER]);
}

// various enemy debug drawing functions
// draw_debug() itself is at the very bottom

void draw_enemy_fighter_debug(ENEMY_FIGHTER_DATA fighter_data) {
    // draw the enemy fighter's hitbox
    VECTOR start = add(fighter_data.position, FIGHTER_COLLISION_POSITION);
    VECTOR end   = add(start, FIGHTER_COLLISION_SIZE);
    al_draw_rectangle(start.x, start.y, end.x, end.y, debug_colour, 2);

    al_draw_textf(tiny_font, debug_colour, start.x - 3, start.y - (TINY_FONT_SIZE + 1), 0, "%i", fighter_data.health);
}

void draw_enemy_imposter_debug(ENEMY_IMPOSTER_DATA imposter_data) {
    // draw the imposter's engines' health and hitboxes
    for (int c = 0; c < ENGINES_PER_IMPOSTER; c++) {
        ENEMY_IMPOSTER_ENGINE* engine = imposter_data.engines[c];
        VECTOR start = engine->position;
        VECTOR end   = add(engine->position, IMPOSTER_ENGINE_SIZE);
        al_draw_rectangle(start.x, start.y, end.x, end.y, debug_colour, 2);
        al_draw_textf(tiny_font, debug_colour, start.x - 3, start.y - (TINY_FONT_SIZE + 1), 0, "%i", engine->health);
    }
}

void draw_enemy_jet_debug(ENEMY_JET_DATA jet_data) {
    // draw the triangle
    TRIANGLE jet_triangle = get_triangle(&jet_data);
    al_draw_triangle(
        jet_triangle.vertices[0].x, jet_triangle.vertices[0].y,
        jet_triangle.vertices[1].x, jet_triangle.vertices[1].y,
        jet_triangle.vertices[2].x, jet_triangle.vertices[2].y,
        debug_colour, 2
    );

    al_draw_textf(tiny_font, debug_colour, jet_data.position.x - 3, jet_data.position.y - TINY_FONT_SIZE - 1, 0, "%i", jet_data.health);
}

void draw_enemy_missile_debug(ENEMY_MISSILE_DATA missile_data) {
    VECTOR end = add(missile_data.position, MISSILE_SIZE);
    al_draw_rectangle(
        missile_data.position.x, missile_data.position.y,
        end.x, end.y, debug_colour, 2
    );
    al_draw_textf(tiny_font, debug_colour, missile_data.position.x - 3, missile_data.position.y - (TINY_FONT_SIZE + 1), 0, "%i", missile_data.health);
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

    // draw a box in the top right corner
    // this will hold the countdown to imposter spawn, as well as other things
    al_draw_filled_rectangle(BUFFER_WIDTH - 100, 0, BUFFER_WIDTH, 60, ui_background_colour);
    al_draw_textf(tiny_font, debug_colour, BUFFER_WIDTH - 93, 3, 0, "%i", imposter_countdown);

    // draw enemy data, whatever it might be
    for (int c = 0; c < MAX_ENEMIES; c++) {
        ENEMY* enemy = &enemies[c];
        if (!enemy->used) continue;
        switch (enemy->type) {
            case ENEMY_FIGHTER:
                ;
                ENEMY_FIGHTER_DATA fighter_data = enemy->data.fighter;
                draw_enemy_fighter_debug(fighter_data);
                break;
            case ENEMY_IMPOSTER:
                ;
                ENEMY_IMPOSTER_DATA imposter_data = enemy->data.imposter;
                draw_enemy_imposter_debug(imposter_data);
                break;
            case ENEMY_JET:
                ;
                ENEMY_JET_DATA jet_data = enemy->data.jet;
                draw_enemy_jet_debug(jet_data);
                break;
            case ENEMY_MISSILE:
                ;
                ENEMY_MISSILE_DATA missile_data = enemy->data.missile;
                draw_enemy_missile_debug(missile_data);
                break;
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
        if (!debug) draw_night_overlay();
        draw_bullets();
    }
    draw_hud();
    draw_debug();
    display_post_draw();
}