
use core::fmt;

/// Stack-allocated, null-terminated UTF-8 string with 64-byte storage.
/// Usable capacity is 63 bytes; one slot is always the trailing '\0'.
pub struct StackString {
    buf: [u8; 64],
    len: usize, // bytes used, excluding the trailing '\0'
}

impl StackString {
    #[inline]
    pub const fn new() -> Self {
        // buffer is zeroed; NUL at [0] is valid for empty string
        Self { buf: [0; 64], len: 0 }
    }

    #[inline] pub const fn capacity(&self) -> usize { 63 }           // usable payload
    #[inline] pub const fn len(&self) -> usize { self.len }
    #[inline] pub const fn remaining(&self) -> usize { 63 - self.len }
    #[inline] pub const fn is_empty(&self) -> bool { self.len == 0 }

    #[inline]
    pub fn clear(&mut self) {
        self.len = 0;
        self.buf[0] = 0; // keep NUL-termination valid
    }

    /// UTF-8 view (does not include the trailing NUL).
    #[inline]
    pub fn as_str(&self) -> &str {
        // Safety: write path only accepts valid UTF-8 & maintains `len`.
        unsafe { core::str::from_utf8_unchecked(&self.buf[..self.len]) }
    }

    /// C-string view including trailing NUL (length = len + 1).
    #[inline]
    pub fn as_c_str(&self) -> &[u8] {
        &self.buf[..=self.len]
    }

    #[inline]
    fn write_bytes(&mut self, s: &str) -> Result<(), fmt::Error> {
        let n = s.len();
        if n > self.remaining() {
            return Err(fmt::Error);
        }
        self.buf[self.len..self.len + n].copy_from_slice(s.as_bytes());
        self.len += n;
        self.buf[self.len] = 0; // keep trailing NUL in sync
        Ok(())
    }
}

impl ufmt::uWrite for StackString {
    type Error = fmt::Error;

    #[inline]
    fn write_str(&mut self, s: &str) -> Result<(), Self::Error> {
        self.write_bytes(s)
    }

    #[inline]
    fn write_char(&mut self, c: char) -> Result<(), Self::Error> {
        let mut tmp = [0u8; 4];
        let s = c.encode_utf8(&mut tmp);
        self.write_bytes(s)
    }
}

/// Panic-on-overflow formatter using a fixed 64-byte buffer.
#[macro_export]
macro_rules! wc_format {
    ($($tt:tt)*) => {{
        let mut __s = $crate::text::StackString::new();
        $crate::ufmt::uwrite!(&mut __s, $($tt)*)
            .expect("wc_format!: capacity exhausted (64B, 63B usable)");
        __s
    }};
}

/// Fallible formatter: `Result<StackString, core::fmt::Error>`.
#[macro_export]
macro_rules! wc_try_format {
    ($($tt:tt)*) => {{
        let mut __s = $crate::text::StackString::new();
        match $crate::ufmt::uwrite!(&mut __s, $($tt)*) {
            Ok(()) => Ok(__s),
            Err(e) => Err(e),
        }
    }};
}

/// Helper macro to work with string formatting and text display.
///
/// The first two params are the x and y coordinates of the text, followed by the template string
/// and its parameters.
///
/// Example:
/// ```rust
/// let score = 23;
///
/// // Displays "Your score is 23 points" with the top-left corner of the 'Y' being at the screen
/// // coordinates (12, 20)
/// wc_print!(12, 20, "Your score is {} points", score);
/// ```
#[macro_export]
macro_rules! wc_print {
    ($x:expr, $y:expr, $($tt:tt)*) => {{
        $crate::api::print($x, $y, wc_format!($($tt)*).as_c_str());
    }};
}

/* ---------- optional tests (need std for test harness) ---------- */
// #[cfg(test)]
// extern crate std;
//
// #[cfg(test)]
// mod tests {
//     use super::*;
//
//     #[test]
//     fn basic() {
//         let s = format64!("hello {} {}", "wasm", 42u32);
//         assert_eq!(s.as_str(), "hello wasm 42");
//         assert_eq!(s.as_c_str().last().copied(), Some(0));
//     }
//
//     #[test]
//     fn overflow() {
//         let r = try_format64!("{:064}", "x"); // will exceed 63B payload
//         assert!(r.is_err());
//     }
// }
//
