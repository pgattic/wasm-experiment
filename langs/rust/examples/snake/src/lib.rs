#![no_std]
#![no_main]

use wasm_experiment as env;
mod circ_buf;

use crate::circ_buf::CircBuf;

const SCREEN_DIMENSIONS: (u16, u16) = (256, 192);
const GRID_SIZE: u8 = 8;
const GRID_DIMENSIONS: (u8, u8) = (
    (SCREEN_DIMENSIONS.0 / GRID_SIZE as u16) as u8,
    (SCREEN_DIMENSIONS.1 / GRID_SIZE as u16) as u8
);

#[derive(PartialEq)]
enum Direction { Right, Left, Down, Up, }

struct Player {
    score: u8,
    body: CircBuf<(u8, u8)>,
    direction: Direction,
}

impl Player {
    pub fn new() -> Self {
        let mut body = CircBuf::new((0, 0));
        body.enqueue((0, 0));
        return Self {
            score: 5,
            body,
            direction: Direction::Right
        }
    }
}

fn new_food() -> (u8, u8, u8) {
    let sprite_id = match env::rand() % 3 {
        0 => 1,
        1 => 4,
        2 => 5,
        _ => unreachable!()
    };
    ((env::rand() % GRID_DIMENSIONS.0 as u32) as u8, (env::rand() % GRID_DIMENSIONS.1 as u32) as u8, sprite_id)
}

#[unsafe(no_mangle)]
pub extern "C" fn start() {
    let mut player = Player::new();
    let mut foods = [new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food()];
    let mut paused = false;
    let mut frame_count: u8 = 0;

    env::println_dbg(b"Hello from Rust!\0");

    loop {
        if env::btn_p(0) && player.direction != Direction::Right { player.direction = Direction::Left; }
        if env::btn_p(1) && player.direction != Direction::Left { player.direction = Direction::Right; }
        if env::btn_p(2) && player.direction != Direction::Down { player.direction = Direction::Up; }
        if env::btn_p(3) && player.direction != Direction::Up { player.direction = Direction::Down; }
        if env::btn_p(4) { paused = !paused; }

        if frame_count >= 6 && !paused {
            frame_count = 0;
            let player_head = &player.body.peek_head();

            player.body.enqueue(match player.direction {
                Direction::Right => { (player_head.0 + 1, player_head.1) },
                Direction::Left => { (player_head.0 - 1, player_head.1) },
                Direction::Down => { (player_head.0, player_head.1 + 1) },
                Direction::Up => { (player_head.0, player_head.1 - 1) },
            });
            while player.body.len() > player.score {
                player.body.dequeue();
            }

            for i in 0..foods.len() {
                if player.body.peek_head() == (foods[i].0, foods[i].1) {
                    player.score += 5;
                    foods[i] = new_food();
                }
            }

            for segment in player.body.into_iter().skip(1) {
                if player.body.peek_head() == segment {
                    player = Player::new();
                    foods = [new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food()];
                    break;
                }
            }
            if player.body.peek_head().0 >= GRID_DIMENSIONS.0 || player.body.peek_head().1 >= GRID_DIMENSIONS.1 {
                player = Player::new();
                foods = [new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food()];
            }
        }

        env::clear_screen(1);

        for segment in &player.body {
            env::rect_fill(segment.0 * GRID_SIZE, segment.1 * GRID_SIZE, GRID_SIZE, GRID_SIZE, 11);
        }

        for food in foods {
            env::sprite(food.0 * GRID_SIZE, food.1 * GRID_SIZE, food.0); // Apple
        }

        frame_count += 1;
        env::sync_frame();
    }
}

