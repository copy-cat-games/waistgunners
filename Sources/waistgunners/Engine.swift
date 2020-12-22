import Foundation

let maximum_engine_health = 12

class Engine : Entity {
    var dead : Bool = false

    override init(pos : Vector) {
        super.init(pos : pos)
        self.health = maximum_engine_health
    }

    override func update() {
        // we need to check for collisions with bullets
    }
}