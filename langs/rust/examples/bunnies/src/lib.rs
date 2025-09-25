#![no_std]
#![no_main]

use wasm_experiment::*;

const MAX_BUNNIES: usize = 1000;

// This program maps a coordinate space (16384, 12288) to the actual Fantasy Console screen
// resolution in order to avoid (expensive) float operations.
const MAPPED_SCREEN_DIMENSIONS: (i16, i16) = (SCREEN_WIDTH as i16 * 64, SCREEN_HEIGHT as i16 * 64);
const MAPPED_SCREEN_CENTER: (i16, i16) = (MAPPED_SCREEN_DIMENSIONS.0/2, MAPPED_SCREEN_DIMENSIONS.1/2);

// Data that must persist between frames
struct GameState {
    num_bunnies: usize,
    bunnies: [(i16, i16, i16 ,i16); MAX_BUNNIES] // x, y, dx, dy
}

impl game_state::Game for GameState {
    fn setup() -> Self {
        Self { num_bunnies: 0, bunnies: [(0, 0, 0, 0); MAX_BUNNIES] }
    }

    fn update(&mut self) {
        if api::btn(Button::A) && self.num_bunnies < MAX_BUNNIES {
            self.bunnies[self.num_bunnies] = (
                MAPPED_SCREEN_CENTER.0,
                MAPPED_SCREEN_CENTER.1,
                ((api::rand() % 65535) - 32768) as i16 % 192, // At the aforementioned coord space,
                ((api::rand() % 65535) - 32768) as i16 % 192  // 128 maps to the number 2.0
            );
            self.num_bunnies += 1;
        }

        api::clear_screen(1);

        for i in 0..self.num_bunnies {
            self.bunnies[i].0 += self.bunnies[i].2;
            self.bunnies[i].1 += self.bunnies[i].3;
            if self.bunnies[i].0 < 0 || self.bunnies[i].0 > (MAPPED_SCREEN_DIMENSIONS.0 - 8 * 64) {
                self.bunnies[i].2 *= -1;
                self.bunnies[i].0 += self.bunnies[i].2;
            }
            if self.bunnies[i].1 < 0 || self.bunnies[i].1 > (MAPPED_SCREEN_DIMENSIONS.1 - 8 * 64) {
                self.bunnies[i].3 *= -1;
                self.bunnies[i].1 += self.bunnies[i].3;
            }
            api::sprite((self.bunnies[i].0 >> 6) as i32, (self.bunnies[i].1 >> 6) as i32, 3);
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

