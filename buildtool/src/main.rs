mod toml_path;
mod commands;
mod config;

use crate::config::*;
use crate::commands::*;
use clap::Parser;

fn main() {
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
            match new_config.create_files() {
                Ok(()) => {
                    println!("New project \"{}\" initialized successfully.", proj_name);
                    println!("You can modify project configuration in `{}`", CONFIG_FILENAME);
                },
                Err(error) => {
                    eprintln!("ERROR creating files: {}", error.to_string());
                    std::process::exit(1);
                }
            }
        },
        Cli::Build => { // Build existing project
            match config_read {
                Ok(config_file) => {
                    match toml::from_str::<ToolConfig>(&config_file) {
                        Ok(config) => {
                            // Build the program according to the config file
                            let result = config.build_project();
                            match result {
                                Ok((name, size)) => {
                                    println!("Built `{}` successfully ({} bytes WASM, {} bytes total).", name, size - 81984, size);
                                }
                                Err(error) => {
                                    eprintln!("ERROR: {}", error);
                                    std::process::exit(1);
                                }
                            }
                        },
                        Err(error) => {
                            eprintln!("ERROR reading `{}`: {}", CONFIG_FILENAME, error);
                        }
                    }
                },
                Err(error) => {
                    match error.kind() {
                        std::io::ErrorKind::NotFound => eprintln!("ERROR: file `{}` not found. Did you run \"wasmcarts-buildtool init\"?", CONFIG_FILENAME),
                        _ => eprintln!("Unexpected error occurred: {}", error),
                    }
                    std::process::exit(1);
                }
            }
        },
    }
}

