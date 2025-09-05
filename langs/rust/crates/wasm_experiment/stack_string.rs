
use core::fmt;

/// Fixed-capacity, null-terminated UTF-8 string in stack buffer.
pub struct StackString<const N: usize> {
    buf: [u8; N],
    len: usize,
}

impl<const N: usize> StackString<N> {
    /// Create empty.
    pub const fn new() -> Self {
        let mut buf = [0; N];
        Self { buf, len: 0 }
    }

    pub const fn capacity(&self) -> usize { N.saturating_sub(1) } // usable space
    pub const fn len(&self) -> usize { self.len }
    pub const fn remaining(&self) -> usize { self.capacity() - self.len }

    pub fn clear(&mut self) {
        self.len = 0;
        self.buf[0] = 0; // keep terminator at start
    }

    pub fn as_str(&self) -> &str {
        unsafe { core::str::from_utf8_unchecked(&self.buf[..self.len]) }
    }

    /// View as C string (with trailing '\0').
    pub fn as_c_str(&self) -> &[u8] {
        &self.buf[..=self.len] // include null terminator
    }

    fn sync_null(&mut self) {
        self.buf[self.len] = 0;
    }
}

impl<const N: usize> fmt::Write for StackString<N> {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        if s.len() > self.remaining() {
            return Err(fmt::Error);
        }
        self.buf[self.len..self.len + s.len()].copy_from_slice(s.as_bytes());
        self.len += s.len();
        self.sync_null();
        Ok(())
    }

    fn write_char(&mut self, c: char) -> fmt::Result {
        let mut tmp = [0u8; 4];
        let s = c.encode_utf8(&mut tmp);
        self.write_str(s)
    }
}

impl<const N: usize> core::ops::Deref for StackString<N> {
    type Target = str;
    fn deref(&self) -> &Self::Target { self.as_str() }
}

impl<const N: usize> fmt::Display for StackString<N> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(self.as_str())
    }
}

impl<const N: usize> fmt::Debug for StackString<N> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_tuple("StackString").field(&self.as_str()).finish()
    }
}

/// Panic-on-overflow macro.
#[macro_export]
macro_rules! format_stack {
    ($cap:expr, $($arg:tt)*) => {{
        let mut __s = $crate::text::StackString::<$cap>::new();
        core::fmt::Write::write_fmt(&mut __s, core::format_args!($($arg)*))
            .expect("format_stack!: capacity exhausted");
        __s
    }};
}

/// Fallible version.
#[macro_export]
macro_rules! try_format_stack {
    ($cap:expr, $($arg:tt)*) => {{
        let mut __s = $crate::StackString::<$cap>::new();
        match core::fmt::Write::write_fmt(&mut __s, core::format_args!($($arg)*)) {
            Ok(()) => Ok(__s),
            Err(e) => Err(e),
        }
    }};
}

// #[cfg(test)]
// extern crate std;
//
// #[cfg(test)]
// mod tests {
//     use super::*;
//
//     #[test]
//     fn null_terminated() {
//         let s = format_stack!(16, "abc{}", 123);
//         assert_eq!(s.as_str(), "abc123");
//         assert_eq!(s.as_c_str().last().copied(), Some(0));
//     }
// }
//
