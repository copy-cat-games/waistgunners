import Foundation
import swallegro

let BULLET_SIZE          = Vector(x : 4, y : 4)
let BULLET_SPEED : Float = 3

class Bullet : Entity {
    var active : Bool = true
    var motion : Vector
    var player : Bool
    var lifetime : Int = 0

    init(pos : Vector, motion : Vector, player : Bool) {
        self.motion = motion
        self.player = player
        super.init(pos : pos)
        self.size = BULLET_SIZE
    }

    func stop() {
        self.active = false
    }

    override func update() {
        self.position = self.motion * BULLET_SPEED + self.position
        self.lifetime = self.lifetime + 1
        if self.position.x < 0 || self.position.y < 0 ||
            Int(self.position.x) > buffer_width || Int(self.position.y) > buffer_height {
            self.active = false
        }
    }

    override func draw() {
        var sprite : SPRITES
        if self.player {
            if self.lifetime % 50 > 25 {
                sprite = SPRITE_PLAYER_BULLET_1
            } else {
                sprite = SPRITE_PLAYER_BULLET_2
            }
        } else {
            if self.lifetime % 50 > 25 {
                sprite = SPRITE_ENEMY_BULLET_1
            } else {
                sprite = SPRITE_ENEMY_BULLET_2
            }
        }
        draw_bitmap(sprite, position : self.position.apply(function : {(_ input : Vector) in
            return Vector(x : Float(Int(input.x)), y : Float(Int(input.y)))
        }), flags : 0)
    }
}