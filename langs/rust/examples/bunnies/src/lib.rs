#![no_std]
#![no_main]

use wasm_experiment::*;

const MAX_BUNNIES: usize = 400;

// This program maps a coordinate space (16384, 12288) to the actual NDS screen
// resolution (256, 192) in order to avoid (expensive) float operations.
const MAPPED_SCREEN_DIMENSIONS: (i16, i16) = (SCREEN_WIDTH as i16 * 64, SCREEN_HEIGHT as i16 * 64);
const MAPPED_SCREEN_CENTER: (i16, i16) = (MAPPED_SCREEN_DIMENSIONS.0/2, MAPPED_SCREEN_DIMENSIONS.1/2);

// Data that must persist between frames
struct GameState {
    num_bunnies: usize,
    bunnies: [(i16, i16, i16 ,i16); MAX_BUNNIES]
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
                ((api::rand() % 65535) - 32768) as i16 % 128, // At the aforementioned coord space,
                ((api::rand() % 65535) - 32768) as i16 % 128  // 128 maps to the number 2
            );
            self.num_bunnies += 1;
        }

        api::sample_bg();

        for i in 0..self.num_bunnies {
            self.bunnies[i].0 += self.bunnies[i].2;
            self.bunnies[i].1 += self.bunnies[i].3;
            if self.bunnies[i].0 < 0 || self.bunnies[i].0 > MAPPED_SCREEN_DIMENSIONS.0 {
                self.bunnies[i].2 *= -1;
                self.bunnies[i].0 += self.bunnies[i].2;
            }
            if self.bunnies[i].1 < 0 || self.bunnies[i].1 > MAPPED_SCREEN_DIMENSIONS.1 {
                self.bunnies[i].3 *= -1;
                self.bunnies[i].1 += self.bunnies[i].3;
            }
            api::sprite((self.bunnies[i].0 >> 6) as u8, (self.bunnies[i].1 >> 6) as u8, 3);
        }
    }
}

wasm_game!(GameState);

