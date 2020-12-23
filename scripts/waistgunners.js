// handle stuff, keep the game together

// const remote = require("electron");
// var window   = remote.getCurrentWindow();

var canvas    = document.querySelector("canvas");
canvas.width  = 400;
canvas.height = 600;
var context   = canvas.getContext("2d");

context.imageSmoothingEnabled = false;
context.scale(2, 2);

// global variables

/**
    the game state.

    possible values: `main menu`, `credits`, `paused`, `playing`, `game over`
*/
var game_state = "main menu";
var ticks      = 0;
var debug      = false;

// keyboard and mouse
mouse = {
    position: new Vector(0, 0),
    down: false,
};

// helpful constants
const KEY_W     = 87;
const KEY_A     = 65;
const KEY_S     = 83;
const KEY_D     = 68;
const KEY_Q     = 81;
const KEY_R     = 82;
const KEY_ESC   = 27;
const KEY_ENTER = 13;
const KEY_SPACE = 32;
const KEY_LEFT  = 37;
const KEY_UP    = 38;
const KEY_RIGHT = 39;
const KEY_DOWN  = 40;

var keys = [];

addEventListener("keydown", (event) => {
    switch (event.keyCode) {
        case KEY_Q:
            if (game_state != "playing") {
                // window.close();
            }
            break;
        case KEY_R:
            debug = !debug;
            break;
        case KEY_ESC:
            if (game_state == "playing") {
                game_state = "paused";
            } else if (game_state == "paused") {
                game_state = "playing";
            }
            break;
        case KEY_ENTER:
            if (game_state == "main menu") {
                game_state = "playing";
            }
            if (game_state == "game over" || game_state == "credits") {
                game_state = "main menu";
            }
            break;
        default:
            keys[event.keyCode] = true;
    }
});

addEventListener("keyup", (event) => {
    keys[event.keyCode] = false;
});

addEventListener("mousedown", (event) => {
    mouse.down = true;
});

addEventListener("mouseup", (event) => {
    mouse.down = false;
});

addEventListener("mousemove", (event) => {
    mouse.position = (new Vector(event.clientX, event.clientY)).times(0.5);
});

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

    draw_hud();
}
