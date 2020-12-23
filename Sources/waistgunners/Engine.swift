import Foundation
import swallegro

let maximum_engine_health = 12

let ENGINE_SIZE = Vector(x : 8, y : 19)

class Engine : Entity {
    var dead : Bool = false

    override init(pos : Vector) {
        super.init(pos : pos)
        self.health = maximum_engine_health
        self.size   = ENGINE_SIZE
    }

    override func update() {
        // we need to check for collisions with bullets
    }

    override func draw() {
        draw_bitmap(SPRITE_ENGINE, position : self.position, flags : 0);
    }
}