import swallegro
import Foundation

/*
    only need to run `swift build` to build this game
*/

// code in here will be run without being in a main() function
// reduces indentation levels anyway. no more Yanderedev style

var redraw  : Bool  = false
var running : Bool  = true
var ticks   : Int64 = 0
var score   : Int   = 0

var mouse = (position : Vector(x : 0, y : 0), down : false)

let buffer_width  = 200
let buffer_height = 300
let display_scale = 2

var keys : [Bool] = [Bool](repeating : false, count : ALLEGRO_KEY_MAX)

let background_colour = Colour(r: 143, g: 188, b: 143, a: 1)

// var enemies : [Enemy]  = []
var bullets : [Bullet] = []

initialize_allegro()
initialize_keyboard()
initialize_mouse()
initialize_display(200, 300, 2)
initialize_font()
initialize_timer_and_event(50)
register_display()
register_keyboard()

set_thickness(2) // set thiccness?

func reset() {
    // resets the game
}

func update() {
    // updates everything in the game

    // update the mouse
    update_mouse()
    mouse.position = Vector(x : Float(get_mouse_x()), y : Float(get_mouse_y()))
    mouse.down     = mouse_is_down()

    bullets = bullets.filter { bullet in
        return bullet.active
    }

    for b in bullets {
        b.update()
    }

    // update the bombers. they will each update their engines and gunners
    update_bombers()
}

func draw() {
    /*
        draw order:
        - background (the ground, if we have one)
        - lower clouds (coming soon!)
        - bombers and enemies
        - bullets
        - particles
        - the reticle and other hud elements
        - debug mode elements, if debug mode is activated
    */
    for b in bombers {
        b.draw()
    }

    for b in bullets {
        b.draw()
    }

    let reticle_position = Vector(x : Float(Int(mouse.position.x)), y : Float(Int(mouse.position.y)))

    if mouse.down {
        draw_bitmap(SPRITE_RETICLE_FIRING, position : reticle_position - (RETICLE_SIZE * 0.5), flags : 0)
    } else {
        draw_bitmap(SPRITE_RETICLE_AIMING, position : reticle_position - (RETICLE_SIZE * 0.5), flags : 0)
    }
}

// sprite loading ---------------------------------------------------------------------------------------------

func draw_bitmap(_ identifier : SPRITES, position : Vector, flags : Int) {
    draw_sprite(Int32(identifier.rawValue), position.x, position.y, Int32(flags))
}

func load_bitmap(_ identifier : SPRITES, position : Vector, size : Vector) {
    load_sprite(Int32(identifier.rawValue), Int32(position.x), Int32(position.y), Int32(size.x), Int32(size.y))
}

// load our sprites
load_spritesheet("spritesheet.png")

/*
    these constants, find a better place for them
    i'm putting them here just for now, so we can load the sprites
*/

let RETICLE_SIZE = Vector(x : 17, y : 17)

let ENEMY_FIGHTER_SIZE  = Vector(x : 37, y : 36)
let ENEMY_JET_SIZE      = Vector(x : 30, y : 35)
let ENEMY_IMPOSTER_SIZE = Vector(x : 66, y : 60)

let ENEMY_IMPOSTER_ENGINE_SIZE = Vector(x : 8, y : 20)

load_bitmap(SPRITE_BOMBER, position : Vector( x : 0, y : 0), size : BOMBER_SIZE)
load_bitmap(SPRITE_ENGINE, position : Vector( x : 0, y : 59), size : ENGINE_SIZE)
load_bitmap(SPRITE_ENGINE_DAMAGED, position : Vector(x : 8, y : 59), size : ENGINE_SIZE)
load_bitmap(SPRITE_ENGINE_DEAD, position : Vector(x : 16, y : 59), size : ENGINE_SIZE)

load_bitmap(SPRITE_RETICLE_AIMING, position : Vector(x : 64, y : 62), size : RETICLE_SIZE)
load_bitmap(SPRITE_RETICLE_FIRING, position : Vector(x : 81, y : 62), size : RETICLE_SIZE)

load_bitmap(SPRITE_PLAYER_BULLET_1, position : Vector( x : 48, y : 75), size : BULLET_SIZE)
load_bitmap(SPRITE_PLAYER_BULLET_2, position : Vector( x : 52, y : 75), size : BULLET_SIZE)
load_bitmap(SPRITE_ENEMY_BULLET_1, position : Vector(x : 56, y : 75), size : BULLET_SIZE)
load_bitmap(SPRITE_ENEMY_BULLET_2, position : Vector(x : 60, y : 75), size : BULLET_SIZE)

load_bitmap(SPRITE_ENEMY_FIGHTER, position : Vector(x : 130, y : 0), size : ENEMY_FIGHTER_SIZE)
load_bitmap(SPRITE_ENEMY_JET_DOWN, position : Vector(x : 167, y : 0), size : ENEMY_JET_SIZE)
load_bitmap(SPRITE_ENEMY_JET_UP, position : Vector(x : 197, y : 0), size : ENEMY_JET_SIZE)
load_bitmap(SPRITE_ENEMY_IMPOSTER, position : Vector(x : 64, y : 0), size : ENEMY_IMPOSTER_SIZE)

load_bitmap(SPRITE_ENEMY_IMPOSTER_ENGINE, position : Vector(x : 24, y : 59), size : ENEMY_IMPOSTER_ENGINE_SIZE)
load_bitmap(SPRITE_ENEMY_IMPOSTER_ENGINE_DAMAGED, position : Vector(x : 32, y : 59), size : ENEMY_IMPOSTER_ENGINE_SIZE)
load_bitmap(SPRITE_ENEMY_IMPOSTER_ENGINE_DEAD, position : Vector(x : 40, y : 59), size : ENEMY_IMPOSTER_ENGINE_SIZE)

initialize_bombers()

start_timer()

repeat {
    let event_type = Int(wait_for_event())
    switch event_type {
        case ALLEGRO_EVENT_TIMER:
            ticks += 1
            update()
            redraw = true
        case ALLEGRO_EVENT_KEY_DOWN:
            keys[Int(get_keyboard_code())] = true
        case ALLEGRO_EVENT_KEY_UP:
            keys[Int(get_keyboard_code())] = false
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false
        default:
            // do nothing
            break
    }

    if redraw && event_queue_is_empty() {
        // redraw the screen
        start_drawing(background_colour)
        draw()
        finish_drawing()
        redraw = false;
    }
} while running

// remember to destroy everything once we're done!
destroy_timer_and_event()
destroy_font()
destroy_display()
destroy_sprites()