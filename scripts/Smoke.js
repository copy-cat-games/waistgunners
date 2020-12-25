const SMOKE_PARTICLE_LIFETIME = 75;
const SMOKE_PARTICLE_RADIUS   = 10;

class Smoke_particle extends Entity {
    constructor(position, motion, dark) {
        super(position);
        this.lifetime = random_int(0, 37);
        this.motion   = motion.plus(new Vector(random(-0.1, 0.1), random(-0.1, 0.1)));
        this.active   = true;
        this.dark     = dark;
    }

    update() {
        this.lifetime++;

        this.position = this.position.plus(this.motion);

        if (this.lifetime >= SMOKE_PARTICLE_LIFETIME) {
            this.active = false;
        }

        return this;
    }

    draw() {
        var alpha      = 1 - (this.lifetime / SMOKE_PARTICLE_LIFETIME);
        var brightness = this.dark ? 10 : 200;
        var colour     = "rgba(" + brightness + ", " + brightness + ", " + brightness + ", " + alpha + ")";
        var radius     = this.lifetime * SMOKE_PARTICLE_RADIUS / SMOKE_PARTICLE_LIFETIME;

        context.fillStyle = colour;
        context.beginPath();
        context.arc(this.get_center().x, this.get_center().y, radius, 0, Math.PI * 2);
        context.closePath();
        context.fill();
    }
}