#![no_std]

pub(crate) mod lazy_mut;
pub mod game_state;
pub mod api;

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    api::println_dbg(b"PANIC!\n\0");
    loop { }
}

// I have also considered 160x144 (GBC screen res.)

/// Width of the game screen.
/// (Fun fact: screen dimensions are the same as the Game Boy Advance!)
pub const SCREEN_WIDTH: i32 = 240;

/// Height of the game screen.
/// (Fun fact: screen dimensions are the same as the Game Boy Advance!)
pub const SCREEN_HEIGHT: i32 = 160;

/// The width and height of the screen, as a tuple (if that's easier to work with).
/// (Fun fact: screen dimensions are the same as the Game Boy Advance!)
pub const SCREEN_DIMENSIONS: (i32, i32) = (SCREEN_WIDTH, SCREEN_HEIGHT);

/// The various buttons supported by WASMCart.
///
/// Used by functions like `btn()` and `btnp()`
pub enum Button {
    Left = 0,
    Right = 1,
    Up = 2,
    Down = 3,
    A = 4,
    B = 5,
    X = 6,
    Y = 7,
    L = 8,
    R = 9,
    Start = 10,
    Select = 11,
}

