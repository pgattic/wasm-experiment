//! This module links to the functions exposed in the game engine environment

#![allow(dead_code)]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {
        sync_frame();
    }
}

#[link(wasm_import_module = "env")]
unsafe extern "C" {
    //#[link_name = "_print"]
    //fn native_print(string: *const u8);
    #[link_name = "_rand"]
    fn native_rand() -> u32;
    #[link_name = "_pSet"]
    fn native_pSet(x: u8, y: u8, c: u8);
    #[link_name = "_rect"]
    fn native_rect(x: u8, y: u8, w: u8, h: u8, c: u8);
    #[link_name = "_rectFill"]
    fn native_rectFill(x: u8, y: u8, w: u8, h: u8, c: u8);
    #[link_name = "_syncFrame"]
    fn native_syncFrame();
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

/// Wait for the current frame to finish (usually used at the end of the game loop)
pub fn sync_frame() {
    unsafe { native_syncFrame() }
}

/// Discover if the given button is being held down
pub fn btn(btn: u8) -> bool {
    unsafe { native_btn(btn) }
}

/// Discover if the given button was pressed this frame.
pub fn btn_p(btn: u8) -> bool {
    unsafe { native_btnP(btn) }
}

/// Prints a C-style (null-terminated string) to the debug console.
///
/// Example:
/// ```
/// mod env;
///
/// // NOTE the "\0" at the end of the string
/// let message = b"Hello, world!\0";
/// env::println_dbg(message);
/// ```
pub fn println_dbg(msg: &[u8]) {
    unsafe { _printLnDbg(msg.as_ptr() as i32); }
}

// User-code entrypoint
#[unsafe(no_mangle)]
pub extern "C" fn start() {
    crate::start();
}

