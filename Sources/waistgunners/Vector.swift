import Foundation

struct Vector {
    var x : Float
    var y : Float

    func to_string() -> String {
        return "(\(self.x), \(self.y))"
    }

    func apply(function : (_ input : Vector) -> Vector) -> Vector {
        return function(self)
    }
}

func +(lhs : Vector, rhs : Vector) -> Vector {
    return Vector(x : lhs.x + rhs.x, y : lhs.y + rhs.y)
}

func -(lhs : Vector, rhs : Vector) -> Vector {
    return Vector(x : lhs.x - rhs.x, y : lhs.y - rhs.y)
}

prefix func -(lhs : Vector) -> Vector {
    return Vector(x : lhs.x * -1, y : lhs.y * -1)
}

func *(lhs : Vector, rhs : Float) -> Vector {
    return Vector(x : lhs.x * rhs, y : lhs.y * rhs)
}

func get_length(of v : Vector) -> Float {
    return hypot(v.x, v.y)
}

func get_angle(of v : Vector) -> Float {
    // gets the angle of the Vector
    let soh   = v.y / get_length(of : v)
    var angle = asin(soh)

    if (v.x < 0) {
        angle = Float.pi - angle;
    }

    while angle < 0 {
        angle += 2 * Float.pi
    }

    return angle
}