class Vector {
    constructor(x, y) {
        this.x = x; this.y = y;

        this.length = Math.hypot(x, y);
    }

    plus(other) {
        return new Vector(this.x + other.x, this.y + other.y);
    }

    minus(other) {
        return new Vector(this.x - other.x, this.y - other.y);
    }

    times(factor) {
        return new Vector(this.x * factor, this.y * factor);
    }

    apply(func) {
        return new Vector(func(this.x), func(this.y));
    }

    get_angle() {
        var opp = this.y;
        var hyp = this.length;

        var angle = Math.asin(opp / hyp);
        if (this.x < 0) {
            angle = Math.PI - angle;
        }
        return angle;
    }

    to_string() {
        return "(" + this.x + ", " + this.y + ")";
    }
}