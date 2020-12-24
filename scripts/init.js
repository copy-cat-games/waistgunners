var canvas    = document.querySelector("canvas");
canvas.width  = 400;
canvas.height = 600;
var context   = canvas.getContext("2d");

context.imageSmoothingEnabled = false;
context.scale(2, 2);
context.textBaseline = "top";
context.textAlign    = "left";
context.font         = "10px 'Press Start 2P'";

// global variables

/**
    the game state.

    possible values: `main menu`, `tutorial`, `credits`, `paused`, `playing`, `game over`
*/
var game_state     = "main menu";
/**
 * the tutorial at the start of the game
 *
 * from 0 to 2
 */
var tutorial_stage = 0;
/*
    tutorial stages:
    0: show the player mouse to aim and fire
    1: show the player how to move the bombers
    2: tell the player to defend their bombers' engines
*/

var ticks = 0;
var debug = false;
var score = 0;

var bullets = [];
var enemies = [];
var bombers = [];

const BUFFER_WIDTH  = 200;
const BUFFER_HEIGHT = 300;

const PLAYER_DEBUG_COLOUR = "rgb(255, 255, 128)";
const ENEMY_DEBUG_COLOUR  = "rgb(240, 40, 40)";

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
const KEY_R     = 82;
const KEY_P     = 80;
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
        case KEY_R:
            debug = !debug;
            break;
        case KEY_ESC:
        case KEY_P:
            if (game_state == "playing") {
                game_state = "paused";
            } else if (game_state == "paused") {
                game_state = "playing";
            }
            break;
        case KEY_ENTER:
            switch (game_state) {
                case "game over":
                case "credits":
                    game_state = "main menu";
                    break;
                case "main menu":
                    game_state = "tutorial";
                    break;
                case "tutorial":
                    if (tutorial_stage == 2) {
                        game_state     = "playing";
                        tutorial_stage = 0;
                    } else {
                        tutorial_stage++;
                    }
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