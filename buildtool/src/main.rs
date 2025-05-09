mod toml_path;
mod commands;
mod config;

use crate::config::*;
use crate::commands::*;
use clap::Parser;

fn main() -> std::io::Result<()> {
    // We basically always need to (attempt to) read to the config file
    let config_read = std::fs::read_to_string(CONFIG_FILENAME);
    match Cli::parse() {
        Cli::Init => { // Initialize a new project
            if config_read.is_ok() {
                eprintln!("ERROR: `WASMCarts.toml` already exists. Will not overwrite!");
                std::process::exit(1);
            }

            // Try to get the current directory name, fall back to a default
            let proj_name: String = std::env::current_dir()
                .ok()
                .and_then(|p| p.iter().last().map(|s| s.to_os_string()))
                .and_then(|s| s.to_str().map(|s| s.to_owned()))
                .unwrap_or("My project".to_string());

            // Generate new config struct
            let new_config = ToolConfig {
                package: PackageConfig {
                    name: proj_name.clone(),
                    ..Default::default()
                },
                ..Default::default()
            };

            // Use it to create the required files (graphics, tilemap, source code)
            new_config.create_files()?;
            println!("New project \"{}\" initialized successfully.", proj_name);
            println!("You can modify project configuration in `{}`", CONFIG_FILENAME);
        },
        Cli::Build => { // Build existing project
            let (name, size) = build(config_read)?;
            println!("Built `{}` successfully ({} bytes WASM, {} bytes total).", name, size - 81984, size);
        },
        Cli::Run => { // Build and run project
            let (name, size) = build(config_read)?;
            println!("Built `{}` successfully ({} bytes WASM, {} bytes total).", name, size - 81984, size);
            println!("Running `WASMCarts {}`...", name);

            let mut cmd = std::process::Command::new("WASMCarts");
            cmd.arg(name);
            let _ = cmd.status()?;
        }
    }
    Ok(())
}

fn build(config_read: std::io::Result<String>) -> std::io::Result<(String, usize)> {
    match config_read {
        Ok(config_file) => {
            match toml::from_str::<ToolConfig>(&config_file) {
                Ok(config) => {
                    // Build the program according to the config file
                    return config.build_project();
                },
                Err(error) => {
                    eprintln!("ERROR reading `{}`: {}", CONFIG_FILENAME, error);
                    std::process::exit(1);
                }
            }
        },
        Err(error) => {
            if error.kind() == std::io::ErrorKind::NotFound {
                eprintln!("ERROR: file `{}` not found. Did you run \"wasmcarts-buildtool init\"?", CONFIG_FILENAME);
                std::process::exit(1);
            }
            Err(error)
        }
    }
}

