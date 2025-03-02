mod index;

//#[global_allocator]
//static ALLOC: wee_alloc::WeeAlloc = wee_alloc::WeeAlloc::INIT;

#[link(wasm_import_module = "env")]
extern "C" {
    //#[link_name = "_print"]
    //fn native_print(string: *const u8);
    #[link_name = "_rand"]
    fn native_rand() -> u8;
    #[link_name = "_fillRedRect"]
    fn native_fillRedRect(x: u8, y: u8, w: u8, h: u8);
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

pub fn fill_red_rect(x: u8, y: u8, w: u8, h: u8) {
    unsafe { native_fillRedRect(x, y, w, h) }
}

pub fn sync_frame() {
    unsafe { native_syncFrame() }
}

pub fn btn(btn: u8) -> bool {
    unsafe { native_btn(btn) }
}

pub fn btnp(btn: u8) -> bool {
    unsafe { native_btnp(btn) }
}

// User-code entrypoint
#[no_mangle]
pub extern "C" fn start() {
    index::start();
}

