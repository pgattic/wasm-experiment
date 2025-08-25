#![no_std]
#![no_main]

use wasm_experiment::*;

// Data that must persist between frames
struct GameState {
    player_x: i32,
    player_y: i32,
}

const SPEED: i32 = 4;

impl game_state::Game for GameState {
    fn setup() -> Self {
        Self { player_x: 15, player_y: 15, }
    }

    fn update(&mut self) {
        if api::btn(Button::Left) { self.player_x -= SPEED; }
        if api::btn(Button::Right) { self.player_x += SPEED; }
        if api::btn(Button::Up) { self.player_y -= SPEED; }
        if api::btn(Button::Down) { self.player_y += SPEED; }

        api::clear_screen(1);

        api::rect_fill(self.player_x, self.player_y, 8, 8, 3);
    }
}

wasm_game!(GameState);

