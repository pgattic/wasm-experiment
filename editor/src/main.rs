use macroquad::prelude::*;

fn window_conf() -> Conf {
    Conf {
        window_title: "WASMCarts".to_owned(),
        ..Default::default()
    }
}

const SCREEN_WIDTH: u32 = 240;
const SCREEN_HEIGHT: u32 = 160;

#[macroquad::main(window_conf)]
async fn main() {

    let game_screen = render_target(SCREEN_WIDTH, SCREEN_HEIGHT);
    game_screen.texture.set_filter(FilterMode::Nearest);

    loop {

        set_camera(&Camera2D {
            render_target: Some(game_screen),
            zoom: vec2(
                2.0 / SCREEN_WIDTH as f32,
                -2.0 / SCREEN_HEIGHT as f32,
            ),
            ..Default::default()
        });

        clear_background(RED);

        draw_line(40.0, 40.0, 100.0, 200.0, 15.0, BLUE);
        draw_rectangle(screen_width() / 2.0 - 60.0, 100.0, 120.0, 60.0, GREEN);

        draw_text("Hello, Macroquad!", 20.0, 20.0, 30.0, DARKGRAY);
        draw_text(&format!("Width: {}", screen_width()), 20.0, 40.0, 30.0, DARKGRAY);

        next_frame().await
    }
}

