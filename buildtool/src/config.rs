use serde::{Deserialize, Serialize};
use crate::toml_path::TomlPath;
use std::path::PathBuf;

#[derive(Serialize, Deserialize, Debug)]
pub struct ToolConfig {
    pub package: PackageConfig,
    pub build: BuildConfig,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct PackageConfig {
    pub name: String,
    pub author: String,
    pub version: String,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct BuildConfig {
    pub compile_command: Vec<String>,
    pub paths: PathsConfig,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct PathsConfig {
    pub wasm_code: TomlPath,
    pub palette: String,
    pub sprite_tiles: String,
    pub background_tiles: String,
    pub background_map: String,
}

impl Default for ToolConfig {
    fn default() -> Self {
        Self {
            package: PackageConfig::default(),
            build: BuildConfig::default()
        }
    }
}

impl Default for PackageConfig {
    fn default() -> Self {
        Self {
            name: "My Game".to_string(),
            author: "Me".to_string(),
            version: "0.1.0".to_string()
        }
    }
}

impl Default for BuildConfig {
    fn default() -> Self {
        Self {
            compile_command: vec![
                "cargo".to_string(),
                "build".to_string(),
                "--target".to_string(),
                "wasm32-unknown-unknown".to_string(),
                "--release".to_string()
            ],
            paths: PathsConfig::default()
        }
    }
}

impl Default for PathsConfig {
    fn default() -> Self {
        Self {
            wasm_code: TomlPath(PathBuf::from("target/wasm32-unknown-unknown/release/output.wasm")),
            palette: "palette.pal".to_string(),
            sprite_tiles: "spr.chr".to_string(),
            background_tiles: "bg.chr".to_string(),
            background_map: "bg.map".to_string(),
        }
    }
}

const DEFAULT_PALETTE: &[u8; 16*3] = include_bytes!("assets/palette.pal");
const DEFAULT_SPR_TILES: &[u8; 8192] = include_bytes!("assets/spr.chr");
const DEFAULT_BG_TILES: &[u8; 8192] = include_bytes!("assets/bg.chr");
const DEFAULT_MAP_DATA: &[u8; 65536] = &[0; 65536];
pub const CONFIG_FILENAME: &str = "WASMCarts.toml";

impl ToolConfig {
    pub fn create_files(&self) -> std::io::Result<()> {
        use std::fs;
        let paths = &self.build.paths;

        fs::write(&paths.palette, DEFAULT_PALETTE)?;
        fs::write(&paths.sprite_tiles, DEFAULT_SPR_TILES)?;
        fs::write(&paths.background_tiles, DEFAULT_BG_TILES)?;
        fs::write(&paths.background_map, DEFAULT_MAP_DATA)?;

        let cfg_string = toml::to_string(&self).unwrap();
        fs::write(CONFIG_FILENAME, cfg_string)?;

        Ok(())
    }
}

