const RETICLE_SIZE = new Vector(16, 16);

function draw_hud() {
    // draw the reticle
    var reticle_position = mouse.position.minus(RETICLE_SIZE.times(0.5));
    context.drawImage(mouse.down ? sprites.reticle_firing : sprites.reticle_aiming, reticle_position.x, reticle_position.y);

    if (debug) {

    }
}