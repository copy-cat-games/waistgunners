const GUNNER_LENGTH = 2;
const GUNNER_RELOAD = 70;
const GUNNER_COLOUR = "rgb(176, 20, 85)";

const GUNNER_INACCURACY = Math.PI / 50;

class Gunner extends Entity {
    constructor(position) {
        super(position);
        this.reload = 0;
        this.target = mouse;
    }

    update() {
        this.reload--;
        if (this.reload <= 0) {
            this.reload = GUNNER_RELOAD;
        }
        switch (this.reload) {
            case 0:
            case 5:
            case 10:
            case 15:
            case 20:
            case 25:
            case 30:
                if (mouse.down || keys[KEY_SPACE]) {
                    var angle  = this.target.position.minus(this.position).get_angle() + random(-GUNNER_INACCURACY, GUNNER_INACCURACY); // chaining galore!
                    var motion = new Vector(Math.cos(angle), Math.sin(angle));
                    bullets.push(new Bullet(this.position.minus(BULLET_SIZE.times(0.5)), motion, "player"));
                }
                // no need for break statement
        }
    }

    draw() {
        var angle = this.target.position.minus(this.position).get_angle();

        context.strokeStyle = GUNNER_COLOUR;
        context.strokeWidth = 2;

        context.beginPath();
        context.moveTo(this.position.x, this.position.y);
        context.lineTo(this.position.x + Math.cos(angle) * GUNNER_LENGTH, this.position.y + Math.sin(angle) * GUNNER_LENGTH);
        context.closePath();
        context.stroke();
    }
}