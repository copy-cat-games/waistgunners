function create_sprite(path) {
    var img = document.createElement("img");
    img.src = path;
    return img;
}

var sprites = {
    player_bomber: create_sprite("sprites/player_bomber.png"),
    player_engine: create_sprite("sprites/engine.png"),
    player_engine_damaged: create_sprite("sprites/engine_damaged.png"),
    player_engine_dead: create_sprite("sprites/engine_dead.png"),
    player_bullet_1: create_sprite("sprites/player_bullet_1.png"),
    player_bullet_2: create_sprite("sprites/player_bullet_2.png"),

    enemy_fighter: create_sprite("sprites/enemy_fighter.png"),
    enemy_jet_down: create_sprite("sprites/enemy_jet_down.png"),
    enemy_jet_up: create_sprite("sprites/enemy_jet_up.png"),
    enemy_imposter: create_sprite("sprites/enemy_imposter.png"),
    enemy_imposter_engine: create_sprite("sprites/imposter_engine.png"),
    enemy_imposter_engine_damaged: create_sprite("sprites/imposter_engine_damaged.png"),
    enemy_imposetr_engine_dead: create_sprite("sprites/imposter_engine_dead.png"),
    enemy_bullet_1: create_sprite("sprites/enemy_bullet_1.png"),
    enemy_bullet_2: create_sprite("sprites/enemy_bullet_2.png"),

    reticle_aiming: create_sprite("sprites/reticle_aiming.png"),
    reticle_firing: create_sprite("sprites/reticle_firing.png"),
};