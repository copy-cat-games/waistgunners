import Foundation
import swallegro

let BOMBER_SIZE     = Vector(x : 63, y : 57)
let BOMBER_ENGINES  = [Vector(x : 15, y : 11), Vector(x : 42, y : 11)]

var bombers : [Bomber] = []

class Bomber : Entity {
    var engines : [Engine]

    override init(pos : Vector) {
        self.engines = [
            // set up the engines
        ]
        super.init(pos : pos)
        self.size = BOMBER_SIZE
    }

    override func update() {
        var motion : Vector = Vector(x : 0, y : 0)
        if keys[ALLEGRO_KEY_A] || keys[ALLEGRO_KEY_LEFT] {
            motion = Vector(x : -1, y : 0) + motion
        }
        if keys[ALLEGRO_KEY_S] || keys[ALLEGRO_KEY_DOWN] {
            motion = Vector(x : 0, y : 1) + motion
        }
        if keys[ALLEGRO_KEY_D] || keys[ALLEGRO_KEY_RIGHT] {
            motion = Vector(x : 1, y : 0) + motion
        }
        if keys[ALLEGRO_KEY_W] || keys[ALLEGRO_KEY_UP] {
            motion = Vector(x : 0, y : -1) + motion
        }
        if (ticks % 2 == 0) {
            self.position = self.position + motion
        }
    }

    override func draw() {
        draw_bitmap(SPRITE_BOMBER, self.position.x, self.position.y, 0)
        // also draw the engines as well
    }
}

func initialize_bombers() {
    var x : Float = 0
    var y : Float = 0

    for i in 0..<4 {
        switch i {
            case 0:
                x = Float(buffer_width / 2)
                y = Float(buffer_height / 2)
            case 1:
                x = Float(buffer_width / 2 + 40)
                y = Float(buffer_height / 2 + 40)
            case 2:
                x = Float(buffer_width / 2 - 40)
                y = Float(buffer_height / 2 + 40)
            case 3:
                x = Float(buffer_width / 2 - 80)
                y = Float(buffer_height / 2 + 80)
            default:
                // do nothing
                break
        }
        bombers.append(Bomber(pos : Vector(x : x, y : y)))
    }
}