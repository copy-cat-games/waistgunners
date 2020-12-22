import Foundation

class Entity {
    // base class for basically everything
    var position : Vector
    var size : Vector
    var health : Int

    init(pos : Vector) {
        self.position = pos;
        self.size     = Vector(x : 0, y : 0)
        self.health   = 1
    }

    func update() {
        // whatever it needs to do
        // override in child classes
    }

    func draw() {
        // override in child classes
    }

    func collision(with other : Entity) -> Bool {
        if (self.position.x > other.position.x + other.size.x ||
            self.position.y > other.position.y + other.size.y ||
            other.position.x > self.position.x + self.size.x ||
            other.position.y > self.position.y + self.size.y) {
            return false
        }
        return true
    }
}