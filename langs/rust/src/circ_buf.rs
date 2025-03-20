
//#[inline]
//pub fn unwrap_abort<T>(o: Option<T>) -> T {
//    use std::process;
//    match o {
//        Some(t) => t,
//        None => process::abort(),
//    }
//}

pub struct CircBuf<T> where T: Copy {
    data: [T; 256],
    head: u8,
    tail: u8,
}

impl<T> CircBuf<T> where T: Copy {
    /// Returns the number of items in the buffer.
    pub fn len(&self) -> u8 {
        self.head.wrapping_sub(self.tail)
    }

    /// Creates a new circular buffer with all slots initialized to `default`.
    pub fn new(default: T) -> Self {
        Self {
            data: [default; 256],
            head: 0,
            tail: 0,
        }
    }

    /// Enqueues an element. Panics if the buffer is full.
    pub fn enqueue(&mut self, val: T) {
        //assert_ne!(self.len(), 255, "Buffer is full");
        self.data[self.head as usize] = val;
        self.head = self.head.wrapping_add(1);
    }

    /// Dequeues an element. Panics if the buffer is empty.
    pub fn dequeue(&mut self) -> T {
        //assert_ne!(self.len(), 0, "Buffer is empty");
        let result = self.data.get(self.tail as usize).unwrap();
        self.tail = self.tail.wrapping_add(1);
        *result
    }

    /// Returns the last enqueued element.
    pub fn peek_head(&self) -> T {
        let index = self.head.wrapping_sub(1) as usize;
        self.data[index]
    }

    ///// Returns the element at the front of the queue.
    //pub fn peek_tail(&self) -> T {
    //    self.data[self.tail as usize]
    //}
}

/// Iterator over the elements of a circular buffer.
pub struct CircBufRevIter<'a, T: Copy> {
    buf: &'a CircBuf<T>,
    remaining: u8,
    current: u8,
}

impl<'a, T: Copy> Iterator for CircBufRevIter<'a, T> {
    type Item = T;

    fn next(&mut self) -> Option<Self::Item> {
        if self.remaining == 0 {
            None
        } else {
            let item = self.buf.data[self.current as usize];
            self.current = self.current.wrapping_sub(1);
            self.remaining -= 1;
            Some(item)
        }
    }
}

/// Implements IntoIterator for &CircBuf<T> to yield items in reverse order.
/// This lets you use a for loop like:
///     for value in &buf { println!("{}", value); }
impl<'a, T: Copy> IntoIterator for &'a CircBuf<T> {
    type Item = T;
    type IntoIter = CircBufRevIter<'a, T>;

    fn into_iter(self) -> Self::IntoIter {
        let length = self.len();
        let start = self.head.wrapping_sub(1); // Start at the most recently enqueued element.
        CircBufRevIter {
            buf: self,
            remaining: length,
            current: start,
        }
    }
}

