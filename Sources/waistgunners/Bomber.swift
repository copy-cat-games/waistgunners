import Foundation

let BOMBER_SIZE     = Vector(x : 62, y : 57)
let BOMBER_ENGINES  = [Vector(x : 15, y : 11), Vector(x : 42, y : 11)]

class Bomber : Entity {
    var engines : [Engine]

    override init(pos : Vector) {
        self.engines = [
            // set up the engines
        ]
        super.init(pos : pos)
        self.size = BOMBER_SIZE
    }
}