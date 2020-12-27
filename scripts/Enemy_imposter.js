const ENEMY_IMPOSTER_ENGINE_HEALTH = 10;
const ENEMY_IMPOSTER_ENGINE_SIZE   = new Vector(7, 19);

class Enemy_imposter_engine extends Engine {
    // why would i subclass Engine, when I'm overriding everything?
    constructor(position) {
        super(position);
        this.size   = ENEMY_IMPOSTER_ENGINE_SIZE;
        this.health = ENEMY_IMPOSTER_ENGINE_HEALTH;
    }

    update() {
        bullets.forEach(b => {
            if (this.collision(b) && b.owner != "enemy") {
                if (this.health) this.health--;
                b.hit();
            }
        });

        if (!this.dead && this.health <= 0) {
            score += 200;
        }

        this.dead = this.health <= 0;

        if (random_int(0, ENEMY_IMPOSTER_ENGINE_HEALTH) > this.health) {
            particles.push(new Smoke_particle(this.get_center(), new Vector(0, 1), this.health < 2));
        }
    }

    draw() {
        var sprite;
        if (this.health > 8) {
            sprite = sprites.enemy_imposter_engine;
        }
        if (this.health > 0) {
            sprite = sprites.enemy_imposter_engine_damaged;
        }
        if (this.dead) {
            sprite = sprites.enemy_imposter_engine_dead;
        }

        context.drawImage(sprite, this.position.x, this.position.y);
    }
}

const ENEMY_IMPOSTER_GUNNER_RELOAD     = 90;
const ENEMY_IMPOSTER_GUNNER_INACCURACY = Math.PI / 150;

class Enemy_imposter_gunner extends Gunner {
    constructor(position, target) {
        super(position);
        this.reload = 0;
        this.target = target;
    }

    update() {
        this.reload--;
        if (this.reload <= 0) {
            this.reload = ENEMY_IMPOSTER_GUNNER_RELOAD;
        }
        if (this.reload > 53 && this.reload % 6 == 0 && this.target != null) {
            var angle  = this.target.get_center().minus(this.position).get_angle() + random(-ENEMY_IMPOSTER_GUNNER_INACCURACY, ENEMY_IMPOSTER_GUNNER_INACCURACY);
            var motion = new Vector(Math.cos(angle), Math.sin(angle));
            bullets.push(new Bullet(this.position.minus(BULLET_SIZE.times(0.5)), motion, "enemy"));
        }
    }

    set_target(target) {
        this.target = target;
    }

    draw() {
        // what the fuck? why are there two ternary statements here?
        var reset_target = this.target == null;
        this.target = reset_target ? {position: this.position.plus(new Vector(0, -1))} : this.target;
        super.draw();
        this.target = reset_target ? null : this.target;
    }
}

const ENEMY_IMPOSTER_SIZE    = new Vector(65, 59);
const ENEMY_IMPOSTER_ENGINES = [
    new Vector(16, 12),
    new Vector(42, 12),
];
const ENEMY_IMPOSTER_GUNNERS = [
    new Vector(24, 36),
    new Vector(40, 36),
];

var spawn_imposter = true;

class Enemy_imposter extends Bomber {
    // bombers! there is an imposter among us...
    constructor() {
        super(new Vector(BUFFER_WIDTH / 2, BUFFER_HEIGHT + 75));
        this.size = ENEMY_IMPOSTER_SIZE;

        this.engines = [
            new Enemy_imposter_engine(this.position.plus(ENEMY_IMPOSTER_ENGINES[0])),
            new Enemy_imposter_engine(this.position.plus(ENEMY_IMPOSTER_ENGINES[1]))
        ];

        this.gunners = [
            new Enemy_imposter_gunner(this.position.plus(ENEMY_IMPOSTER_GUNNERS[0])),
            new Enemy_imposter_gunner(this.position.plus(ENEMY_IMPOSTER_GUNNERS[1]))
        ];

        this.target_bomber = null;
        this.target_engine = null;

        this.active = true;

        spawn_imposter = false;
    }

    update() {
        super.update();

        // pick a target if there isn't any or if the current one is down
        if (this.target_bomber == null || this.target_bomber.down) {
            var index = random_int(0, 4);
            if (bombers[index].down) {
                this.target_bomber = null;
            } else {
                this.target_bomber = bombers[index];
            }
        }

        if (this.target_bomber != null && (this.target_engine == null || this.target_engine.dead)) {
            // pick a new engine target
            var index                   = random_int(0, 2);
            var potential_target_engine = this.target_bomber.engines[index];
            if (potential_target_engine.dead) {
                this.target_engine = null;
            } else {
                this.target_engine = potential_target_engine;
            }
            this.gunners[0].set_target(this.target_engine);
            this.gunners[1].set_target(this.target_engine);
        }

        if (this.down && (this.position.y > BUFFER_HEIGHT + this.size.y)) {
            this.active    = false;
            spawn_imposter = true;
        }

        if (!this.down && this.position.y > BUFFER_HEIGHT * 3/ 4) {
            this.move(new Vector(0, -1));
        }

        return this;
    }

    draw() {
        context.drawImage(sprites.enemy_imposter, this.position.x, this.position.y);
        this.engines[0].draw();
        this.engines[1].draw();
        this.gunners[0].draw();
        this.gunners[1].draw();
    }
}