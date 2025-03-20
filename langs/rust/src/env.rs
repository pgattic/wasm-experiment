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
    fn native_rand() -> u8;
    #[link_name = "_rect"]
    fn native_rect(x: u8, y: u8, w: u8, h: u8, c: u8);
    #[link_name = "_fillRect"]
    fn native_fillRect(x: u8, y: u8, w: u8, h: u8, c: u8);
    #[link_name = "_syncFrame"]
    fn native_syncFrame();
    #[link_name = "_btn"]
    fn native_btn(btn: u8) -> bool;
    #[link_name = "_btnp"]
    fn native_btnp(btn: u8) -> bool;
}

//pub fn print(string: &str) {
//    let c_str = CString::new(string).unwrap();
//    unsafe { native_print(c_str.as_ptr() as *const u8)}
//}

pub fn rand() -> u8 {
    unsafe { native_rand() }
}

/// Draw an outlined Rectangle, with the specified x and y position, width, height, and color index.
pub fn rect(x: u8, y: u8, w: u8, h: u8, c: u8) {
    unsafe { native_rect(x, y, w, h, c) }
}

/// Draw a filled Rectangle, with the specified x and y position, width, height, and color index.
pub fn fill_rect(x: u8, y: u8, w: u8, h: u8, c: u8) {
    unsafe { native_fillRect(x, y, w, h, c) }
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
pub fn btnp(btn: u8) -> bool {
    unsafe { native_btnp(btn) }
}

// User-code entrypoint
#[unsafe(no_mangle)]
pub extern "C" fn start() {
    crate::start();
}

