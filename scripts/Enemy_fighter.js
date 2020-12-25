const ENEMY_FIGHTER_SIZE      = new Vector(36, 35);
const ENEMY_FIGHTER_GUNS      = [new Vector(10, 30), new Vector(25, 30)];
const ENEMY_FIGHTER_HEALTH    = 15;
const ENEMY_FIGHTER_RELOAD    = 30;
const ENEMY_FIGHTER_THRESHOLD = 3;

class Enemy_fighter extends Entity {
    constructor() {
        super(new Vector(random_int(0, BUFFER_WIDTH), -40));
        this.target = null;
        this.size   = ENEMY_FIGHTER_SIZE;
        this.gun    = 0;
        this.health = ENEMY_FIGHTER_HEALTH;
        this.active = true;
        this.reload = 0;
    }

    update() {
        bullets.forEach(b => {
            if (this.collision(b) && b.owner != "enemy") {
                b.hit();
                this.health--;
                if (this.health <= 0) this.active = false;
            }
        });

        // pick a target, shoot the target!

        return this;
    }

    draw() {
        context.drawImage(sprites.enemy_fighter, this.position.x, this.position.y);
    }

    draw_debug() {
        context.fillStyle   = ENEMY_DEBUG_COLOUR;
        context.strokeStyle = ENEMY_DEBUG_COLOUR;
    }
}