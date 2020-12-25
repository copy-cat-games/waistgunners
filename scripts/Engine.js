const MAXIMUM_ENGINE_HEALTH = 12;

const ENGINE_SIZE = new Vector(7, 18);

class Engine extends Entity {
    constructor(position) {
        super(position);
        this.health   = MAXIMUM_ENGINE_HEALTH;
        this.dead     = false;
        this.size     = ENGINE_SIZE;
    }

    update() {
        bullets.forEach(b => {
            if (this.collision(b) && b.owner != "player") {
                if (this.health) this.health--;
                b.hit();
            }
        });

        if (random_int(0, MAXIMUM_ENGINE_HEALTH) > this.health) {
            particles.push(new Smoke_particle(this.get_center(), new Vector(0, 1), this.health < 4));
        }
    }

    draw() {
        var sprite;

        switch (this.health) {
            case 12:
            case 11:
            case 10:
            case 9:
            case 8:
                sprite = sprites.player_engine;
                break;
            case 7:
            case 6:
            case 5:
            case 4:
            case 3:
            case 2:
            case 1:
                sprite = sprites.player_engine_damaged;
                break;
            default:
                sprite = sprites.player_engine_dead;
        }

        context.drawImage(sprite, this.position.x, this.position.y);
    }

    draw_debug(hide_health) {
        context.strokeStyle = PLAYER_DEBUG_COLOUR;
        context.strokeRect(this.position.x, this.position.y, this.size.x, this.size.y);
        if (hide_health) return;
        context.save();
        context.fillStyle = PLAYER_DEBUG_COLOUR;
        context.textAlign    = "center";
        context.textBaseline = "bottom";
        context.fillText(this.health, this.position.x + this.size.x / 2, this.position.y);
        context.restore();
    }
}