// handle stuff, keep the game together

// animation and timing

var last_time = null;
function animate(time) {
    var lapse = (last_time == null) ? 0 : (time - last_time);

    if (game_state != "paused") {
        update();
    }

    draw();

    requestAnimationFrame(animate);
}

var cloud_countdown = 0;

function reset() {
    bombers = [];
    initialize_bombers();
    enemies   = [];
    particles = [];
    bullets   = [];
    score     = 0;
    ticks     = 0;

    display_score = 0;
}

function update() {
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

    if (game_state == "playing") {
        // spawn in enemies!
        if (ticks % 75 == 0) {
            enemies.push(new Enemy_fighter());
        }
        if (score > 2000 && random_int(0, 50) > 48 && spawn_imposter) {
            enemies.push(new Enemy_imposter());
        }
    }

    update_hud();

    if (bombers.every(b => {
        return b.down;
    })) {
        game_state = "game over";
    }
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