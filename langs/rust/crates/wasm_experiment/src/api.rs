
#[link(wasm_import_module = "env")]
unsafe extern "C" {
    #[link_name = "_rand"]
    fn native_rand() -> u32;
    #[link_name = "_clearScreen"]
    fn native_clearScreen(c: u8);
    #[link_name = "_pSet"]
    fn native_pSet(x: i32, y: i32, c: u8);
    #[link_name = "_rect"]
    fn native_rect(x: i32, y: i32, w: u32, h: u32, c: u8);
    #[link_name = "_rectFill"]
    fn native_rectFill(x: i32, y: i32, w: u32, h: u32, c: u8);
    #[link_name = "_sprite"]
    fn native_sprite(x: i32, y: i32, sprite_id: u8);
    #[link_name = "_showChar"]
    fn native_showChar(x: i32, y: i32, char_code: u8);
    #[link_name = "_print"]
    fn native_print(x: i32, y: i32, ptr: i32);
    #[link_name = "_tileMap"]
    fn native_tileMap(draw_x: i32, draw_y: i32, map_x: u8, map_y: u8, map_w: u8, map_h: u8);
    #[link_name = "_btn"]
    fn native_btn(btn: u8) -> bool;
    #[link_name = "_btnP"]
    fn native_btnP(btn: u8) -> bool;
    #[link_name = "_printLnDbg"]
    fn _printLnDbg(arg: i32);
}

/// Generate a random 32-bit integer.
///
/// Note that the randomness of the number may depend on engine-side implementation details. If you
/// want the random numbers to follow a certain sequence every time the game is started or be based
/// on a seed, you will have to create your own generator.
///
/// Example:
/// ```rust
/// let new_position: (u8, u8) = (
///     (api::rand() % SCREEN_WIDTH as u32 ) as u8,
///     (api::rand() % SCREEN_HEIGHT as u32 ) as u8
/// );
/// ```
pub fn rand() -> u32 {
    unsafe { native_rand() }
}

/// Clear the screen to a given color.
///
/// Example:
/// ```rust
/// api::clear_screen(1);
/// ```
pub fn clear_screen(color: u8) {
    unsafe { native_clearScreen(color) }
}

/// Draw a pixel with the specified x and y position and color index.
///
/// Example:
/// ```rust
/// api::p_set(12, 12, 3);
/// ```
pub fn p_set(x: i32, y: i32, color: u8) {
    unsafe { native_pSet(x, y, color) }
}

/// Draw an outlined rectangle with the specified x and y position, width, height, and color index.
///
/// Example:
/// ```rust
/// api::rect(12, 12, 24, 24, 3);
/// ```
pub fn rect(x: i32, y: i32, w: u32, h: u32, color: u8) {
    unsafe { native_rect(x, y, w, h, color) }
}

/// Draw a filled rectangle with the specified x and y position, width, height, and color index.
///
/// Example:
/// ```rust
/// api::rect_fill(12, 12, 24, 24, 3);
/// ```
pub fn rect_fill(x: i32, y: i32, w: u32, h: u32, color: u8) {
    unsafe { native_rectFill(x, y, w, h, color) }
}

/// Draw a sprite with the specified screen x and y position, and a given sprite id.
///
/// Example:
/// ```rust
/// api::sprite(player_x, player_y, 2);
/// ```
pub fn sprite(x: i32, y: i32, sprite_id: u8) {
    unsafe { native_sprite(x, y, sprite_id) }
}

/// Draw the given character with the specified screen x and y position (top-left corner of char).
///
/// Example:
/// ```rust
/// api::show_char(12, 12, b'H');
/// ```
pub fn show_char(x: i32, y: i32, char_code: u8) {
    unsafe { native_showChar(x, y, char_code) }
}

/// Prints a raw byte string (lacks unicode support) at a given (top-left corner) location.
///
/// Currently, the string must be null-terminated. The `wc_format!` macro handles this, but raw
/// byte buffers are allowed, if used with caution.
///
/// See also the `wc_print!` macro.
///
/// Example:
/// ```rust
/// // Formatted string
/// api::print(12, 20, wc_format!("Your score is {} points", 23).as_c_str());
///
/// // Constant string
/// api::print(12, 12, c"Hello World!".to_bytes()); // Use a C-string, please
/// ```
pub fn print(x: i32, y: i32, text: &[u8]) {
    unsafe { native_print(x, y, text.as_ptr() as i32); }
}

/// Draw a series of tiles according to the tilemap data of the game (unstable).
///
/// Example:
/// ```rust
/// // Draw the top-left corner of the tilemap on the screen
/// api::tile_map(0, 0, 0, 0, SCREEN_WIDTH/8, SCREEN_HEIGHT/8);
/// ```
pub fn tile_map(draw_x: i32, draw_y: i32, map_x: u8, map_y: u8, map_w: u8, map_h: u8) {
    unsafe { native_tileMap(draw_x, draw_y, map_x, map_y, map_w, map_h) }
}

/// Discover if the given button is being held down
///
/// Example:
/// ```rust
/// if api::btn(Button::Left) {
///     player_x -= 1;
/// }
/// if api::btn(Button::Right) {
///     player_y += 1;
/// }
/// ```
pub fn btn(button: crate::Button) -> bool {
    unsafe { native_btn(button as u8) }
}

/// Discover if the given button was pressed this frame.
///
/// Example:
/// ```rust
/// if api::btn_p(Button::Start) {
///     paused = !paused;
/// }
/// ```
pub fn btn_p(button: crate::Button) -> bool {
    unsafe { native_btnP(button as u8) }
}

/// Prints a C-style (null-terminated string) to the debug console.
///
/// Example:
/// ```
/// // NOTE the "\0" at the end of the string
/// let message = b"Hello, world!\0";
/// api::println_dbg(message);
/// ```
pub fn println_dbg(msg: &[u8]) {
    unsafe { _printLnDbg(msg.as_ptr() as i32); }
}

