import Foundation
import swallegro

let BOMBER_SIZE    = Vector(x : 63, y : 58)
let BOMBER_ENGINES = [Vector(x : 15, y : 11), Vector(x : 42, y : 11)]
let BOMBER_GUNNERS = [Vector(x : 23, y : 35), Vector(x : 39, y : 35)]

var bombers : [Bomber] = []

class Bomber : Entity {
    var engines : [Engine]
    var gunners : [Gunner]
    var down : Bool = false

    override init(pos : Vector) {
        self.engines = [
            // set up the engines
            Engine(pos : pos + BOMBER_ENGINES[0]),
            Engine(pos : pos + BOMBER_ENGINES[1])
        ]
        self.gunners = [
            // set up the gunners
            Gunner(pos : pos + BOMBER_GUNNERS[0]),
            Gunner(pos : pos + BOMBER_GUNNERS[1])
        ]
        super.init(pos : pos)
        self.size = BOMBER_SIZE
    }

    override func update() {
        // we'll also update the engines and the gunners
        // update the engines regardless
        // there are only two, no loop needed
        self.engines[0].update()
        self.engines[1].update()

        if !self.down {
            self.gunners[0].update()
            self.gunners[1].update()
        }
    }

    func move(by motion : Vector) {
        if (ticks % 2 == 0) {
            self.position = self.position + motion
            // move the engines and gunners, too
            for e in self.engines {
                e.position = e.position + motion
            }
            for g in self.gunners {
                g.position = g.position + motion
            }
        }
    }

    override func draw() {
        draw_bitmap(SPRITE_BOMBER, position : self.position, flags : 0)
        // also draw the engines as well
        for e in self.engines {
            e.draw()
        }
        for g in self.gunners {
            g.draw()
        }
    }
}

func initialize_bombers() {
    var x : Float = 0
    var y : Float = 0

    for i in 0..<4 {
        switch i {
            case 0:
                x = Float(buffer_width / 2 - 10)
                y = Float(buffer_height / 2)
            case 1:
                x = Float(buffer_width / 2 + 30)
                y = Float(buffer_height / 2 + 40)
            case 2:
                x = Float(buffer_width / 2 - 50)
                y = Float(buffer_height / 2 + 40)
            case 3:
                x = Float(buffer_width / 2 - 90)
                y = Float(buffer_height / 2 + 80)
            default:
                // do nothing
                break
        }
        bombers.append(Bomber(pos : Vector(x : x, y : y)))
    }
}

func update_bombers() {
    for b in bombers {
        b.update()
    }

    var motion = Vector(x : 0, y : 0)
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
    for b in bombers {
        if b.down {
            // if this bomber is down, it's gonna fall out of formation anyway
            continue
        }

        let new_position = b.position + motion
        // reset motion.x if the bombers will move out of bounds
        if (
            Int(new_position.x) < -12 ||
            Int((new_position + BOMBER_SIZE).x) - 12 > buffer_width
        ) {
            motion = Vector(x : 0, y : motion.y)
        }
        // reset motion.y if the bombers will move out of bounds
        if (
            Int(new_position.y) < 0 ||
            Int((new_position + BOMBER_SIZE).y) - 15 > buffer_height
        ) {
            motion = Vector(x : motion.x, y : 0)
        }
    }
    for b in bombers {
        b.move(by : motion)
    }
}