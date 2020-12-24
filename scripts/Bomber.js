const BOMBER_SIZE  = new Vector(63, 58);
const BOMBER_SPEED = 0.5;

const BOMBER_ENGINES = [
    new Vector(15, 10),
    new Vector(41, 10),
];

const BOMBER_GUNNERS = [
    new Vector(23, 35),
    new Vector(39, 35),
];

class Bomber extends Entity {
    constructor(position) {
        super(position);
        this.size = BOMBER_SIZE;
        this.down = false;

        this.engines = [
            new Engine(this.position.plus(BOMBER_ENGINES[0])),
            new Engine(this.position.plus(BOMBER_ENGINES[1])),
        ];
        this.gunners = [
            new Gunner(this.position.plus(BOMBER_GUNNERS[0])),
            new Gunner(this.position.plus(BOMBER_GUNNERS[1])),
        ];
    }

    update(lapse) {
        this.engines[0].update();
        this.engines[1].update();
        if (!this.down) {
            this.gunners[0].update();
            this.gunners[1].update();
        }
    }

    move(offset) {
        this.position = this.position.plus(offset);
        this.engines[0].position = this.engines[0].position.plus(offset);
        this.engines[1].position = this.engines[1].position.plus(offset);
        this.gunners[0].position = this.gunners[0].position.plus(offset);
        this.gunners[1].position = this.gunners[1].position.plus(offset);
    }

    draw() {
        // also draw the engines and the gunners
        context.drawImage(sprites.player_bomber, this.position.x, this.position.y);
        this.engines[0].draw();
        this.engines[1].draw();
        this.gunners[0].draw();
        this.gunners[1].draw();
    }

    draw_debug(hide_health) {
        this.engines[0].draw_debug(hide_health);
        this.engines[1].draw_debug(hide_health);
    }
}

function initialize_bombers() {
    for (var c = 0; c < 4; c++) {
        var base_position = new Vector(82, 172);
        switch (c) {
            // case 0 uses the base position, at the head of the formation
            case 1:
                base_position.x += 40; base_position.y += 40;
                break;
            case 2:
                base_position.x -= 40; base_position.y += 40;
                break;
            case 3:
                base_position.x -= 80; base_position.y += 80;
                break;
        }
        bombers.push(new Bomber(base_position));
    }
}

function update_bombers(lapse) {
    var motion = new Vector(0, 0);
    if (keys[KEY_A] || keys[KEY_LEFT]) {
        motion.x -= 0.5;
    }
    if (keys[KEY_D] || keys[KEY_RIGHT]) {
        motion.x += 0.5;
    }
    bombers.forEach(b => {
        if (b.down) return;
        if (b.position.x + motion.x + BOMBER_SIZE.x > BUFFER_WIDTH + 10 || b.position.x + motion.x < -10) {
            motion.x = 0;
        }
    });

    if (keys[KEY_W] || keys[KEY_UP]) {
        motion.y -= 0.5;
    }
    if (keys[KEY_S] || keys[KEY_DOWN]) {
        motion.y += 0.5;
    }
    bombers.forEach(b => {
        if (b.down) return;
        if (b.position.y + motion.y + BOMBER_SIZE.y > BUFFER_HEIGHT + 10 || b.position.y + motion.y < -10) {
            motion.y = 0;
        }
    });

    bombers.forEach(b => {
        b.update();
        b.move(motion);
    });
}