mod toml_path;
mod commands;
mod config;

use crate::config::*;
use crate::commands::*;
use clap::Parser;

fn main() {
    let config_read = std::fs::read_to_string(CONFIG_FILENAME);
    match Cli::parse() {
        Cli::Init => {
            if config_read.is_ok() {
                eprintln!("ERROR: `WASMCarts.toml` already exists. Will not overwrite!");
                std::process::exit(1);
            }
            let cur_dir = std::env::current_dir().unwrap();
            let proj_name = cur_dir.iter().last().unwrap().to_str().unwrap();

            let new_config = ToolConfig {
                package: PackageConfig {
                    name: proj_name.to_owned(),
                    ..Default::default()
                },
                ..Default::default()
            };

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
        Cli::Build => {
            match config_read {
                Ok(config_file) => {
                    let config: ToolConfig = toml::from_str(&config_file).unwrap();
                    println!("Found config:");
                    println!("{:?}", config);
                },
                Err(error) => {
                    match error.kind() {
                        std::io::ErrorKind::NotFound => eprintln!("ERROR: file `{}` not found. Did you run \"wasmcarts-buildtool init\"?", CONFIG_FILENAME),
                        _ => eprintln!("Unexpected error occurred: {}", error.to_string()),
                    }
                    std::process::exit(1);
                }
            }
        },
    }
}

