use clap::Parser;

#[derive(Parser)]
pub enum Cli {
    /// Build an existing project according to the `WASMCarts.toml` file in the current working
    /// directory
    Build,
    /// Initialize a WASMCarts project in the current working directory
    Init,
    /// Ensure the project is built, and attempt to run it in the engine
    Run,
}

