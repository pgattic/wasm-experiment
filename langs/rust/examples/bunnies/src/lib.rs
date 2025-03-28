#![no_std]
#![no_main]

use wasm_experiment::{self as env, Button};

const MAX_BUNNIES: usize = 400;

// This program maps a coordinate space (16384, 12288) to the actual NDS screen
// resolution (256, 192) in order to avoid (expensive) float operations.
const MAPPED_SCREEN_DIMENSIONS: (i16, i16) = (16384, 12288);
const MAPPED_SCREEN_CENTER: (i16, i16) = (MAPPED_SCREEN_DIMENSIONS.0/2, MAPPED_SCREEN_DIMENSIONS.1/2);

// User-code entrypoint
#[unsafe(no_mangle)]
pub extern "C" fn start() {
    let mut num_bunnies = 0;
    let mut bunnies: [(i16, i16, i16 ,i16); MAX_BUNNIES] = [(0, 0, 0, 0); MAX_BUNNIES];

    loop {
        if env::btn(Button::A) && num_bunnies < MAX_BUNNIES {
            bunnies[num_bunnies] = (
                MAPPED_SCREEN_CENTER.0,
                MAPPED_SCREEN_CENTER.1,
                ((env::rand() % 65535) - 32768) as i16 % 128, // At the aforementioned coord space,
                ((env::rand() % 65535) - 32768) as i16 % 128  // 128 maps to the number 2
            );
            num_bunnies += 1;
        }

        env::sample_bg();

        for i in 0..num_bunnies {
            bunnies[i].0 += bunnies[i].2;
            bunnies[i].1 += bunnies[i].3;
            if bunnies[i].0 < 0 || bunnies[i].0 > MAPPED_SCREEN_DIMENSIONS.0 {
                bunnies[i].2 *= -1;
                bunnies[i].0 += bunnies[i].2;
            }
            if bunnies[i].1 < 0 || bunnies[i].1 > MAPPED_SCREEN_DIMENSIONS.1 {
                bunnies[i].3 *= -1;
                bunnies[i].1 += bunnies[i].3;
            }
            env::sprite((bunnies[i].0 >> 6) as u8, (bunnies[i].1 >> 6) as u8, 3);
        }

        env::sync_frame();
    }
}

