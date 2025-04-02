use crate::lazy_mut::LazyMut;

pub trait Game {
    fn setup() -> Self;
    fn update(&mut self);
}

pub struct GameWrapper<T: Game> {
    pub state: LazyMut<T>,
}

impl<T: Game> GameWrapper<T> {
    pub const fn new() -> Self {
        Self {
            state: LazyMut::new(),
        }
    }

    pub fn init(&self) {
        self.state.set(T::setup());
    }

    pub fn update(&self) {
        if let Some(mut game) = self.state.get_mut() {
            game.update();
        }
    }
}

/// Takes a struct that implements `Game`, creates a global instance of it, and hooks the member
/// methods of the struct the game into the engine's expected `setup` and `update` functions.
///
/// Example:
/// ```rust
/// struct GameState = {
///     counter: usize
/// }
///
/// impl Game for GameState {
///     fn setup() -> Self {
///         Self { counter: 0 }
///     }
///     fn update(&mut self) {
///         self.counter += 1;
///     }
/// }
/// ```
#[macro_export]
macro_rules! static_game {
    ($ty:ty) => {
        static GAME: $crate::game_state::GameWrapper<$ty> = $crate::game_state::GameWrapper::new();

        #[unsafe(no_mangle)]
        pub extern "C" fn setup() {
            GAME.init();
        }

        #[unsafe(no_mangle)]
        pub extern "C" fn update() {
            GAME.update();
        }
    };
}

