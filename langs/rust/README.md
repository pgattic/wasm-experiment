
# Rust Programming Language Support

Overall, Rust has been given the most attention out of the supported programming languages.

## Getting Started

- Ensure you have the WASM toolchain installed: `rustup target add wasm32-unknown-unknown`
- Make and enter a new directory for your game's code to go in.
- Run `cargo init --lib`
- Set the default build target to `wasm32-unknown-unknown`
    - Make a new file at .cargo/config.toml:

```toml
[build]
target = "wasm32-unknown-unknown"
```

- Add the wasm_experiment crate to Cargo (TODO: define clearly how to do that)
- Add `use wasm_experiment::*` and `#![no_main]` to your lib.rs (and you'll probably want to add `#![no_std]` as well)
- Create a struct that stores your game state and `impl game_state::Game` for it, then use the `wasm_game!` macro to hook it into global scope
    - Here is a dead-simple `lib.rs` example that you could use as a starting point:

```rust
#![no_std]
#![no_main]

use wasm_experiment::*;

/// Data that need to persist between frames
struct GameState {
    counter: usize
}

impl game_state::Game for GameState {
    fn setup() -> Self {
        // Define the default starting state
        Self { counter: 0 }
    }
    fn update(&mut self) {
        // Modify the state
        self.counter += 1;
        // Render based on the state
        api::rect(12 + (self.counter % 24), 12, 8, 8, 3);
    }
}

// Hook `setup` and `update` to the console
wasm_game!(GameState)
```

- Build your game with `cargo build --release`
- Prepend the other required game data using the buildtool

## Rationale

As specified in the documentation, the fantasy console expects your WebAssembly code to expose global `setup` and `update` functions that it calls at the required times. This design in the console makes it so that all game state that needs to be preserved between frames must be global. This is the way that all other fantasy consoles work, that I have seen. For most programming languages, global mutable state is not a problem, but for Rust, it isn't as easy (and for good reason).

The inclusion of the macro and the trait adds very little overhead to the compiled code (only about 100-150 bytes) and basically no extra memory usage. It has no external dependencies, and is `no_std`-friendly. I made sure to only have it do the minimum work required to achieve global mutable state, and I would expect any hand-rolled solution to be about the same in terms of resource usage.

However, if you would rather create your own solution or if your game will be stateless, you are welcome to expose global `setup` and `update` functions yourself:

```rust
#![no_std]
#![no_main]

#[unsafe(no_mangle)]
pub extern "C" fn setup() {
    // Setup code here...
}

#[unsafe(no_mangle)]
pub extern "C" fn update() {
    // Update code here...
}
```

