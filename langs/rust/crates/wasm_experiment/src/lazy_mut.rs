use core::cell::UnsafeCell;

/// A simple struct that just lets you create a lazy, mutable object.
/// Definitely not thread-safe, which is fine for me. Code speed go vroom, code size go nyoom
/// Has no external dependencies, works allocator-free. Just the way the good Lord intended.
pub struct LazyMut<T> {
    value: UnsafeCell<Option<T>>,
}

impl<T> LazyMut<T> {
    pub const fn new() -> Self {
        Self {
            value: UnsafeCell::new(None),
        }
    }

    pub fn set(&self, value: T) {
        unsafe { *self.value.get() = Some(value); }
    }

    pub fn get_mut(&self) -> Option<&mut T> {
        unsafe { (*self.value.get()).as_mut() }
    }
}

// We're single-threaded
unsafe impl<T> Sync for LazyMut<T> {}

