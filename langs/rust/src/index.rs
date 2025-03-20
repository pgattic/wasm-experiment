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

fn new_food() -> (u8, u8) {
    (crate::rand() % GRID_DIMENSIONS.0, crate::rand() % GRID_DIMENSIONS.1)
}

pub fn start() {
    let mut player = Player::new();
    let mut food: (u8, u8) = new_food();
    let mut paused = false;
    let mut frame_count: u8 = 0;
    loop {
        if crate::btnp(0) && player.direction != Direction::Right { player.direction = Direction::Left; }
        if crate::btnp(1) && player.direction != Direction::Left { player.direction = Direction::Right; }
        if crate::btnp(2) && player.direction != Direction::Down { player.direction = Direction::Up; }
        if crate::btnp(3) && player.direction != Direction::Up { player.direction = Direction::Down; }
        if crate::btnp(4) { paused = !paused; }

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

            if player.body.peek_head() == food {
                player.score += 5;
                food = new_food();
            }

            for segment in player.body.into_iter().skip(1) {
                if player.body.peek_head() == segment {
                    player = Player::new();
                    food = new_food();
                    break;
                }
            }
            if player.body.peek_head().0 >= GRID_DIMENSIONS.0 || player.body.peek_head().1 >= GRID_DIMENSIONS.1 {
                player = Player::new();
                food = new_food();
            }
        }

        for segment in &player.body {
            crate::fill_rect(segment.0 * GRID_SIZE, segment.1 * GRID_SIZE, GRID_SIZE, GRID_SIZE, 11);
        }

        crate::fill_rect(food.0 * GRID_SIZE, food.1 * GRID_SIZE, GRID_SIZE, GRID_SIZE, 8);

        frame_count += 1;
        crate::sync_frame();
    }
}

