/**
 * base class for everything in the game.
 *
 * `update()` and `draw()` should be overriden.
 *
 * overriding `draw_debug()` is optional.
 */
class Entity {
    constructor(position) {
        this.position = position;
        this.size     = new Vector(0, 0);
    }

    update(lapse) {
        // override this
    }

    draw() {
        // override this
    }

    /**
     * detects rectangular collision with another entity
     * @param {Entity} other the other entity with which to detect collision
     * @returns {boolean} returns `true` if the other entity overlaps with this one.
     */
    collision(other) {
        if (other == this) return false;
        if (this.position.x > other.position.x + other.size.x) return false;
        if (this.position.y > other.position.y + other.size.y) return false;
        if (other.position.x > this.position.x + this.size.x) return false;
        if (other.position.y > this.position.y + this.size.y) return false;
        return true;
    }

    draw_debug() {

    }
}