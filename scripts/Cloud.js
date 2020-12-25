class Cloud extends Entity {
    constructor() {
        super(new Vector(0, 0));
        this.position.y = -100;
        this.position.x = random(0, BUFFER_WIDTH);

        this.motion = random(0.1, 0.5);
        this.alpha  = random(0.1, 0.2);
        this.active = true;

        this.clusters     = [];
        var cluster_count = random_int(4, 7);
        while(cluster_count > 0) {
            var r  = random_int(20, 70);
            var dx = random_int(-45, 45);
            var dy = random_int(-15, 15);

            this.clusters.push({
                radius: r,
                position: new Vector(dx, dy),
            });
            cluster_count--;
        }
    }

    update() {
        this.position.y += this.motion;
        if (this.position.y > BUFFER_HEIGHT + 100) {
            this.active = false;
        }
        return this;
    }

    draw() {
        context.fillStyle  = "rgba(255, 255, 255, " + this.alpha + ")";
        this.clusters.forEach(cl => {
            var final_position = this.position.plus(cl.position);
            context.beginPath();
            context.arc(final_position.x, final_position.y, cl.radius, 0, Math.PI * 2);
            context.closePath();
            context.fill();
        });
    }
}