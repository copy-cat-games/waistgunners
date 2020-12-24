const MAXIMUM_ENGINE_HEALTH = 12;

class Engine extends Entity {
    constructor(position) {
        super(position);
        this.health   = MAXIMUM_ENGINE_HEALTH;
        this.dead     = false;
    }

    update() {

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
}