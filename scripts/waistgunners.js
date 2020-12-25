// handle stuff, keep the game together

// animation and timing

var last_time = null;
function animate(time) {
    var lapse = (last_time == null) ? 0 : (time - last_time);

    if (game_state != "paused") {
        update(lapse);
    }

    draw();

    requestAnimationFrame(animate);
}

var cloud_countdown = 0;

function update(lapse) {
    ticks++;
    cloud_countdown--;

    bullets = bullets.filter(b => {
        return b.update().active;
    });

    update_bombers();

    enemies = enemies.filter(enemy => {
        return enemy.update().active;
    });

    particles = particles.filter(particle => {
        return particle.update().active;
    });

    clouds = clouds.filter(cloud => {
        return cloud.update().active;
    });

    if (cloud_countdown <= 0) {
        // add a cloud
        clouds.push(new Cloud());

        cloud_countdown = random_int(0, 300);
    }

    update_hud();
}

function draw() {
    context.clearRect(0, 0, canvas.width, canvas.height);
    /*
        draw order:
        - the ground (if we have a background)
        - low clouds
        - player and enemies
        - shots
        - particles
        - hud elements (reticle, highscore, debug mode)
    */

    clouds.forEach(c => {
        c.draw();
    });

    bombers.forEach(b => {
        b.draw();
    });

    enemies.forEach(e => {
        e.draw();
    });

    bullets.forEach(b => {
        b.draw();
    });

    particles.forEach(p => {
        p.draw();
    });

    draw_hud();
}

initialize_bombers();