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

var mouse : Vector = Vector(x : 0, y : 0)

let buffer_width  = 200
let buffer_height = 300
let display_scale = 2

var keys : [Bool] = [Bool](repeating : false, count : ALLEGRO_KEY_MAX)

let background_colour = Colour(r: 143, g: 188, b: 143, a: 1)

initialize_allegro()
initialize_keyboard()
initialize_display(200, 300, 2)
initialize_font()
initialize_timer_and_event(50)
register_display()
register_keyboard()

func reset() {
    // resets the game
}

func update() {
    // updates everything in the game
    for b in bombers {
        b.update()
    }
}

func draw() {
    for b in bombers {
        b.draw()
    }
}

func draw_bitmap(_ identifier : SPRITES, position : Vector, flags : Int) {
    draw_sprite(Int32(identifier.rawValue), position.x, position.y, Int32(flags))
}

func load_bitmap(_ identifier : SPRITES, position : Vector, size : Vector) {
    load_sprite(Int32(identifier.rawValue), Int32(position.x), Int32(position.y), Int32(size.x), Int32(size.y))
}

// load our sprites
load_spritesheet("spritesheet.png")
load_sprite(Int32(SPRITE_BOMBER.rawValue), 0, 0, Int32(BOMBER_SIZE.x), Int32(BOMBER_SIZE.y))
load_bitmap(SPRITE_ENGINE, position : Vector( x : 0, y : 59), size : ENGINE_SIZE);

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