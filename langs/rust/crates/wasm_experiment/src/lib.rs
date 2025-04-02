#![no_std]

pub(crate) mod lazy_mut;
pub mod game_state;

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop { }
}

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

#[link(wasm_import_module = "env")]
unsafe extern "C" {
    //#[link_name = "_print"]
    //fn native_print(string: *const u8);
    #[link_name = "_rand"]
    fn native_rand() -> u32;
    #[link_name = "_clearScreen"]
    fn native_clearScreen(c: u8);
    #[link_name = "_pSet"]
    fn native_pSet(x: u8, y: u8, c: u8);
    #[link_name = "_rect"]
    fn native_rect(x: u8, y: u8, w: u8, h: u8, c: u8);
    #[link_name = "_rectFill"]
    fn native_rectFill(x: u8, y: u8, w: u8, h: u8, c: u8);
    #[link_name = "_sampleBg"]
    fn native_sample_bg();
    #[link_name = "_sprite"]
    fn native_sprite(x: u8, y: u8, sprite_id: u8);
    #[link_name = "_btn"]
    fn native_btn(btn: u8) -> bool;
    #[link_name = "_btnP"]
    fn native_btnP(btn: u8) -> bool;
    #[link_name = "_printLnDbg"]
    fn _printLnDbg(arg: i32);
}

/// Generates a random 32-bit integer.
pub fn rand() -> u32 {
    unsafe { native_rand() }
}

/// Clears the screen to a given color.
pub fn clear_screen(color: u8) {
    unsafe { native_clearScreen(color) }
}

/// Draw an outlined Rectangle, with the specified x and y position, width, height, and color index.
pub fn p_set(x: u8, y: u8, color: u8) {
    unsafe { native_pSet(x, y, color) }
}

/// Draw an outlined Rectangle, with the specified x and y position, width, height, and color index.
pub fn rect(x: u8, y: u8, w: u8, h: u8, color: u8) {
    unsafe { native_rect(x, y, w, h, color) }
}

/// Draw a filled Rectangle, with the specified x and y position, width, height, and color index.
pub fn rect_fill(x: u8, y: u8, w: u8, h: u8, color: u8) {
    unsafe { native_rectFill(x, y, w, h, color) }
}

pub fn sample_bg() {
    unsafe { native_sample_bg() }
}

/// Draw a sprite with the specified screen x and y position, and a given sprite id.
///
/// Example:
/// ```rust
/// sprite(player_x, player_y, 2);
/// ```
pub fn sprite(x: u8, y: u8, sprite_id: u8) {
    unsafe { native_sprite(x, y, sprite_id) }
}

/// Discover if the given button is being held down
///
/// Example:
/// ```rust
/// if btn(Button::Left) {
///     player_x -= 1;
/// }
/// if btn(Button::Right) {
///     player_y += 1;
/// }
/// ```
pub fn btn(button: Button) -> bool {
    unsafe { native_btn(button as u8) }
}

/// Discover if the given button was pressed this frame.
///
/// Example:
/// ```rust
/// if btn_p(Button::Start) {
///     paused = !paused;
/// }
/// ```
pub fn btn_p(button: Button) -> bool {
    unsafe { native_btnP(button as u8) }
}

/// Prints a C-style (null-terminated string) to the debug console.
///
/// Example:
/// ```
/// // NOTE the "\0" at the end of the string
/// let message = b"Hello, world!\0";
/// println_dbg(message);
/// ```
pub fn println_dbg(msg: &[u8]) {
    unsafe { _printLnDbg(msg.as_ptr() as i32); }
}

