#![no_std]
#![no_main]

use wasm_experiment::*;
mod circ_buf;

use crate::circ_buf::CircBuf;

const CELL_SIZE: i32 = 8;
const GRID_WIDTH: i32 = SCREEN_WIDTH / CELL_SIZE;
const GRID_HEIGHT: i32 = SCREEN_HEIGHT / CELL_SIZE;

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
    let sprite_id = match api::rand() % 3 {
        0 => 1,
        1 => 4,
        2 => 5,
        _ => unreachable!()
    };
    (
        (api::rand() % GRID_WIDTH as u32) as u8,
        (api::rand() % GRID_HEIGHT as u32) as u8,
        sprite_id
    )
}

struct GameState {
    player: Player,
    foods: [(u8, u8, u8); 8],
    paused: bool,
    frame_count: u8,
}

impl game_state::Game for GameState {
    fn setup() -> Self {
        Self {
            player: Player::new(),
            foods: [new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food()],
            paused: false,
            frame_count: 0,
        }
    }

    fn update(&mut self) {
        if api::btn_p(Button::Left) && self.player.direction != Direction::Right { self.player.direction = Direction::Left; }
        if api::btn_p(Button::Right) && self.player.direction != Direction::Left { self.player.direction = Direction::Right; }
        if api::btn_p(Button::Up) && self.player.direction != Direction::Down { self.player.direction = Direction::Up; }
        if api::btn_p(Button::Down) && self.player.direction != Direction::Up { self.player.direction = Direction::Down; }
        if api::btn_p(Button::Start) { self.paused = !self.paused; }

        if self.frame_count >= 6 && !self.paused {
            self.frame_count = 0;
            let player_head = &self.player.body.peek_head();

            self.player.body.enqueue(match self.player.direction {
                Direction::Right => { (player_head.0 + 1, player_head.1) },
                Direction::Left => { (player_head.0 - 1, player_head.1) },
                Direction::Down => { (player_head.0, player_head.1 + 1) },
                Direction::Up => { (player_head.0, player_head.1 - 1) },
            });
            while self.player.body.len() > self.player.score {
                self.player.body.dequeue();
            }

            for i in 0..self.foods.len() {
                if self.player.body.peek_head() == (self.foods[i].0, self.foods[i].1) {
                    self.player.score += 5;
                    self.foods[i] = new_food();
                }
            }

            for segment in self.player.body.into_iter().skip(1) {
                if self.player.body.peek_head() == segment {
                    self.player = Player::new();
                    self.foods = [new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food()];
                    break;
                }
            }
            if self.player.body.peek_head().0 >= GRID_WIDTH as u8 || self.player.body.peek_head().1 >= GRID_HEIGHT as u8 {
                self.player = Player::new();
                self.foods = [new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food(), new_food()];
            }
        }

        api::clear_screen(1);

        for (x, y) in &self.player.body {
            api::rect_fill(x as i32 * CELL_SIZE, y as i32 * CELL_SIZE, CELL_SIZE as u32, CELL_SIZE as u32, 11);
        }

        for (x, y, spr) in self.foods {
            api::sprite(x as i32 * CELL_SIZE, y as i32 * CELL_SIZE, spr);
        }
        wc_print!(0, SCREEN_HEIGHT - 8, "Score: {}", self.player.score);

        self.frame_count += 1;
    }
}

wasm_game!(GameState);

