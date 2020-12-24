const RETICLE_SIZE = new Vector(16, 16);

var display_score = 0;

function update_hud() {
    if (ticks % 3 == 0) {
        for (var x = 13; x > 1; x--) {
            var increase = Math.pow(2, x);
            if (score - display_score >= increase) {
                display_score += increase / 4;
            }
            if (score - display_score < 4 && score > display_score) {
                display_score++;
            }
        }
    }
}

function draw_hud() {
    context.fillStyle    = "white";
    context.font         = "sans-serif";
    context.textAlign    = "left";
    context.textBaseline = "top";

    context.fillText("SCORE: " + left_pad(display_score), 2, 2);

    var center        = new Vector(BUFFER_WIDTH / 2, BUFFER_HEIGHT / 2);
    context.textAlign = "center";

    switch (game_state) {
        case "main menu":
            context.fillStyle = PLAYER_DEBUG_COLOUR;
            context.fillText("WAISTGUNNERS", center.x, center.y);
            if (ticks % 150 > 50) context.fillText("ENTER to start", center.x, center.y + 20);
            break;
        case "tutorial":
            context.fillStyle = PLAYER_DEBUG_COLOUR;
            switch (tutorial_stage) {
                case 0:
                    context.fillText("use MOUSE to", center.x, center.y - 5);
                    context.fillText("aim and fire", center.x, center.y + 5);
                    break;
                case 1:
                    context.fillText("WASD or arrows", center.x, center.y - 5);
                    context.fillText("move around", center.x, center.y + 5);
                    break;
                case 2:
                    context.fillText("protect your", center.x, center.y - 17);
                    context.fillText("bombers' engines!", center.x, center.y - 5);
                    context.fillText("each has 12 health", center.x, center.y + 5);
                    context.fillText("GOOD LUCK!", center.x, center.y + 17);
                    bombers.forEach(b => {
                        b.draw_debug(true);
                    });
            }
            break;
        case "paused":
            context.fillStyle = "red";
            context.fillText("PAUSED", center.x, center.y);
            context.fillText("press ESC to unpause", center.x, center.y + 10);
            break;
        case "game over":
            context.fillStyle = "red";
            if (ticks % 150 < 110) context.fillText("GAME OVER", center.x, center.y);
            context.fillStyle = PLAYER_DEBUG_COLOUR;
            context.fillText("score: " + score, center.x, center.y + 12);
            context.fillText("ENTER to play again", center.x, center.y + 24);
            break;
    }

    // draw the reticle
    var reticle_position = mouse.position.minus(RETICLE_SIZE.times(0.5));
    context.drawImage(mouse.down ? sprites.reticle_firing : sprites.reticle_aiming, reticle_position.x, reticle_position.y);

    if (debug) {
        context.fillStyle = "red";
        context.textAlign = "left";
        context.fillText("DEBUG MODE", 2, 12);
        bombers.forEach(b => {
            b.draw_debug();
        });
    }
}

function left_pad(value) {
    value = value.toString();
    while (value.length < 6) {
        value = "0" + value;
    }
    return value;
}