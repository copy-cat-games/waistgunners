import Foundation
import swallegro

class Gunner : Entity {
    private var target : Vector

    override init(pos : Vector) {
        self.target = Vector(x : 0, y : 0)
        super.init(pos : pos)
    }

    override func update() {
        // remember to shoot!
    }
}