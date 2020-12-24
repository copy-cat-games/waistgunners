const RETICLE_SIZE = new Vector(16, 16);

var display_score = 0;

function update_hud() {
    if (ticks % 5 == 0) {
        for (var x = 11; x > 0; x--) {
            var increase = Math.pow(2, x);
            if (score - display_score >= increase) {
                display_score += increase / 2;
            }
            if (score - display_score == 1) {
                display_score++;
            }
        }
    }
}

function draw_hud() {
    context.fillStyle = "white";
    context.font      = "sans-serif";

    context.fillText("SCORE: " + left_pad(display_score), 2, 10);

    // draw the reticle
    var reticle_position = mouse.position.minus(RETICLE_SIZE.times(0.5));
    context.drawImage(mouse.down ? sprites.reticle_firing : sprites.reticle_aiming, reticle_position.x, reticle_position.y);

    if (debug) {

    }
}

function left_pad(value) {
    value = value.toString();
    while (value.length < 6) {
        value = "0" + value;
    }
    return value;
}