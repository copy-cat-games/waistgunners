const BULLET_SPEED = 3;
const BULLET_SIZE  = new Vector(3, 3);

class Bullet extends Entity {
    constructor(position, motion, owner) {
        super(position);
        this.owner    = owner;
        this.active   = true;
        this.motion   = motion;
        this.lifetime = 0;
    }

    update() {
        this.lifetime++;
        this.position = this.position.plus(this.motion.times(BULLET_SPEED));
        this.active   = (
            this.position.x > 0 && this.position.x < BUFFER_WIDTH &&
            this.position.y > 0 && this.position.y < BUFFER_HEIGHT
        ) && this.active;
        return this;
    }

    hit() {
        this.active = false;
    }

    draw() {
        var sprite;
        if (this.owner == "player") {
            if (this.lifetime % 35 > 17) {
                sprite = sprites.player_bullet_1;
            } else {
                sprite = sprites.player_bullet_2;
            }
        } else if (this.owner == "enemy") {
            if (this.lifetime % 20 > 10) {
                sprite = sprites.enemy_bullet_1;
            } else {
                sprite = sprites.enemy_bullet_2;
            }
        }
        context.drawImage(sprite, this.position.x, this.position.y);
    }
}