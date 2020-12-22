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

func draw_bitmap(_ identifier : SPRITES, _ x : Float, _ y : Float, _ flags : Int) {
    draw_sprite(Int32(identifier.rawValue), x, y, Int32(flags))
}

// load our sprites
load_spritesheet("spritesheet.png")
load_sprite(Int32(SPRITE_BOMBER.rawValue), 0, 0, Int32(BOMBER_SIZE.x), Int32(BOMBER_SIZE.y))

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