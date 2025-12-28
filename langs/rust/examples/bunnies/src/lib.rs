#![no_std]
#![no_main]

use wasm_experiment::{*, game_state::Game};

// This program maps a coordinate space 64 times as wide and tall to the Fantasy Console screen
// resolution, in order to avoid (expensive) float operations.
// That is what all the `<< 6` and `>> 6` are for

const MAX_BUNNIES: usize = 4000;

#[derive(Clone, Copy, Default)]
struct Bunny { x: i32, y: i32, dx: i32, dy: i32 }

// Data that must persist between frames
struct GameState {
    num_bunnies: usize,
    bunnies: [Bunny; MAX_BUNNIES]
}

impl game_state::Game for GameState {
    fn setup() -> Self {
        Self { num_bunnies: 0, bunnies: [Bunny::default(); MAX_BUNNIES] }
    }

    fn update(&mut self) {
        if api::btn(Button::A) && self.num_bunnies < MAX_BUNNIES {
            let rand = api::rand();
            self.bunnies[self.num_bunnies] = Bunny {
                x: (SCREEN_WIDTH/2 - 4) << 6,
                y: (SCREEN_HEIGHT/2 - 4) << 6,
                dx: (((rand & 0xFFFF) % 385) - 192) as i32, // -192..192 maps to -3.0..3.0
                dy: (((rand >> 16) % 385) - 192) as i32
            };
            self.num_bunnies += 1;
        }

        api::clear_screen(1);

        for bunny in &mut self.bunnies[..self.num_bunnies].iter_mut() {
            bunny.x += bunny.dx;
            bunny.y += bunny.dy;
            if bunny.x < 0 || bunny.x > (SCREEN_WIDTH - 8) << 6 {
                bunny.dx = -bunny.dx;
                bunny.x += bunny.dx;
            }
            if bunny.y < 0 || bunny.y > (SCREEN_HEIGHT - 8) << 6 {
                bunny.dy = -bunny.dy;
                bunny.y += bunny.dy;
            }
            api::sprite(bunny.x >> 6, bunny.y >> 6, 3);
        }
        let rect_width = match self.num_bunnies {
            0..10 => 1,
            10..100 => 2,
            100..1000 => 3,
            1000.. => 4,
        } * 8 + 1;
        api::rect_fill(0, SCREEN_HEIGHT - 9, rect_width, 9, 0);
        wc_print!(1, SCREEN_HEIGHT - 8, "{}", self.num_bunnies);
    }
}

wasm_game!(GameState);

