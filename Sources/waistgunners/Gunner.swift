import Foundation
import swallegro

let GUNNER_LENGTH = 3
let GUNNER_RELOAD = 60

class Gunner : Entity {
    var target : Vector
    var reload : Int = 0

    override init(pos : Vector) {
        self.target = Vector(x : 0, y : 0)
        super.init(pos : pos)
    }

    override func update() {
        // remember to shoot!

        // first update the gunners' position
        self.target = mouse.position

        let angle = get_angle(of : self.target - self.position)

        if mouse.down {
            if self.reload % 5 == 0 && self.reload > 20 {
                bullets.append(Bullet(pos : self.position - Vector(x : 2, y : 2), motion : Vector(x : cos(angle), y : sin(angle)), player : true))
            }
        }
        self.reload -= 1
        if self.reload <= 0 {
            self.reload = GUNNER_RELOAD
        }
    }

    override func draw() {
        let angle = get_angle(of : self.target - self.position)
        draw_line(self.position.x, self.position.y, cos(angle), sin(angle), Colour(r : 240, g : 40, b : 200, a : 1))
    }
}