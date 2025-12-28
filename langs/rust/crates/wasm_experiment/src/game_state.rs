
// Developer's note: I didn't want to impose this much control over the users with regard to
// maintaining game state. However, Rust's self-imposed safety measures led me to it. All things
// considered, I think it is a quite elegant solution that hopefully shouldn't get in people's way.

/// Defines the functions required for a wasmcarts game to run:
///
/// - `setup() -> Self`
///     - Called by the engine once when the game starts
///     - Used to set up the starting state of your game
/// - `update(&mut self)`
///     - Called by the engine once per frame
///     - Use it for any kind of physics updates, movement, key reactions, etc.
///     - You are also expected to render any kind of graphics here
///
/// Don't forget to use the `wasm_game!` macro to connect the data of that struct to the engine's
/// global hooks.
///
/// Here is an example of basically the simplest game you can make:
/// ```rust
/// #![no_std]
/// #![no_main]
///
/// use wasm_experiment::*;
///
/// /// Data that need to persist between frames
/// struct GameState {
///     counter: usize
/// }
///
/// impl game_state::Game for GameState {
///     fn setup() -> Self {
///         // Define the default starting state
///         Self { counter: 0 }
///     }
///     fn update(&mut self) {
///         // Modify the state
///         self.counter += 1;
///         // Render based on the state
///         api::rect(12 + (self.counter % 24), 12, 8, 8, 3);
///     }
/// }
///
/// // Hook `setup` and `update` to the engine
/// wasm_game!(GameState)
/// ```
pub trait Game {
    fn setup() -> Self;
    fn update(&mut self);
}

/// Takes a struct that implements `game_state::Game`, creates a global instance of it, and hooks
/// the member methods of the struct the game into the engine's expected `setup` and `update`
/// functions.
///
/// Example:
/// ```rust
/// /// Data that need to persist between frames
/// struct GameState = {
///     counter: usize
/// }
///
/// impl game_state::Game for GameState {
///     /// Called once when the game starts
///     fn setup() -> Self {
///         Self { counter: 0 }
///     }
///     /// Called each frame
///     fn update(&mut self) {
///         self.counter += 1;
///         api::rect(12 + (self.counter % 24), 12, 8, 8, 3);
///     }
/// }
/// wasm_game!(GameState)
/// ```
#[macro_export]
macro_rules! wasm_game {
    ($ty:ty) => {
        static mut GAME_STATE: Option<$ty> = None;

        #[unsafe(no_mangle)]
        pub extern "C" fn setup() {
            unsafe {
                GAME_STATE = Some(<$ty>::setup());
            }
        }

        #[unsafe(no_mangle)]
        pub extern "C" fn update() {
            unsafe {
                if let Some(state) = GAME_STATE.as_mut() {
                    state.update();
                }
            }
        }
    };
}

