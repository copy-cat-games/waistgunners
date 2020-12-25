const ENEMY_FIGHTER_SIZE       = new Vector(36, 35);
const ENEMY_FIGHTER_GUNS       = [new Vector(10, 30), new Vector(25, 30)];
const ENEMY_FIGHTER_HEALTH     = 15;
const ENEMY_FIGHTER_RELOAD     = 30;
const ENEMY_FIGHTER_THRESHOLD  = 3;
const ENEMY_FIGHTER_SPEED      = 3;
const ENEMY_FIGHTER_INACCURACY = Math.PI / 100;

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
                if (this.health <= 0) {
                    this.active = false;
                    score += 100;
                }
            }
        });

        // pick a target, shoot the target!
        if (this.target == null) {
            for (var c = 0; c < 10; c++) {
                var pick          = random_int(0, 8);
                var bomber_number = Math.floor(pick / 2);
                var engine_number = pick % 2;

                var engine = bombers[bomber_number].engines[engine_number];
                if (!engine.dead) {
                    this.target = engine;
                    break;
                }
            }
        } else {
            // move to the target
            if (Math.abs(this.get_center().x - this.target.get_center().x) <= ENEMY_FIGHTER_THRESHOLD) {
                // shoot!
                if (this.reload <= 0) {
                    this.reload       = ENEMY_FIGHTER_RELOAD;
                    var angle         = this.target.get_center().minus(this.get_center()).get_angle() + random(-ENEMY_FIGHTER_INACCURACY, ENEMY_FIGHTER_INACCURACY);
                    var fire_position = this.position.plus(ENEMY_FIGHTER_GUNS[this.gun]);
                    bullets.push(new Bullet(fire_position, new Vector(Math.cos(angle), Math.sin(angle)), "enemy"));
                    this.gun = 1 - this.gun;
                }
            } else if (this.get_center().x > this.target.get_center().x) {
                if (this.position.y > 0) {
                    this.position.x -= 1;
                }
            } else if (this.get_center().x < this.target.get_center().x) {
                if (this.position.y > 0) {
                    this.position.x += 1;
                }
            }
        }

        if (this.position.y <= BUFFER_WIDTH / 2) {
            this.position.y++;
        }

        if (random_int(0, ENEMY_FIGHTER_HEALTH) > this.health + 1) {
            particles.push(new Smoke_particle(this.get_center(), new Vector(0, -1), this.health > 3));
        }

        this.reload--;

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