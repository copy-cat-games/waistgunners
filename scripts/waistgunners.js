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

function update(lapse) {
    ticks++;

    update_bombers();
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

    bombers.forEach(b => {
        b.draw();
    });
    draw_hud();
}

initialize_bombers();