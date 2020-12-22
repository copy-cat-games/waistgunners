import swallegro
import Foundation

/*
    only need to run `swift build` to build this game
*/

// code in here will be run without being in a main() function
// reduces indentation levels anyway. no more Yanderedev style

var redraw  : Bool = false
var running : Bool = true

var frame : Int = 0

let background_colour = Colour(r: 0, g: 0, b: 0, a: 1)
let text_colour       = Colour(r: 255, g: 255, b: 255, a: 1)

initialize_allegro()
initialize_keyboard()
initialize_display(200, 300, 2)
initialize_font()
initialize_timer_and_event(50)
register_display()
register_keyboard()

start_timer()

repeat {
    let event_type = Int(wait_for_event())
    switch event_type {
        case ALLEGRO_EVENT_TIMER:
            redraw = true
        case ALLEGRO_EVENT_KEY_DOWN:
            fallthrough
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false
        default:
            // do nothing
            break
    }

    if event_queue_is_empty() {
        // redraw the screen
        start_drawing(background_colour)
        draw_text(2, 2, "Hello Allegro from Swift!", text_colour, 0)
        finish_drawing()
    }
} while running



// remember to destroy everything once we're done!
destroy_timer_and_event();
destroy_font()
destroy_display()