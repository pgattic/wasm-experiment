mod toml_path;
mod commands;
mod config;

use clap::Parser;
use std::{env, fs::{self, File}, io::Write};

const DEFAULT_PALETTE: &[u8; 16*3] = include_bytes!("assets/palette.pal");
const DEFAULT_BG_TILES: &[u8; 8192] = include_bytes!("assets/bg.chr");
const DEFAULT_SPR_TILES: &[u8; 8192] = include_bytes!("assets/spr.chr");
const DEFAULT_MAP_DATA: &[u8; 65536] = &[0; 65536];

fn main() {
    let args = commands::Cli::parse();
    match args {
        commands::Cli::Init => {
            let cur_dir = env::current_dir().unwrap();
            let proj_name = cur_dir.iter().last().unwrap().to_str().unwrap();

            { // Create config file
                let new_config = config::ToolConfig::with_name(proj_name);
                let cfg_string = toml::to_string(&new_config).unwrap();
                let _ = fs::write("config.toml", cfg_string);
            }

            // Create Palette file
            let _ = File::create("palette.pal").unwrap()
                .write_all(DEFAULT_PALETTE);

            // Create sprite nametable
            let _ = File::create("spr.chr").unwrap()
                .write_all(DEFAULT_SPR_TILES);

            // Create background nametable
            let _ = File::create("bg.chr").unwrap()
                .write_all(DEFAULT_BG_TILES);

            // Create background tilemap
                let _ = File::create("bg.map").unwrap()
                .write_all(DEFAULT_MAP_DATA);

            println!("New project \"{}\" initialized successfully.", proj_name);
        }
        commands::Cli::Build => {
            // Build code here...
        }
    }
}

