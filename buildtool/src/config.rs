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
    pub code: BuildCodeConfig,
    pub assets: BuildAssetsConfig,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct BuildCodeConfig {
    pub command: Vec<String>,
    pub output: TomlPath,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct BuildAssetsConfig {
    pub dir: String,
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
            name: "New game".to_string(),
            author: "Your name here".to_string(),
            version: "0.1.0".to_string()
        }
    }
}

impl Default for BuildConfig {
    fn default() -> Self {
        Self {
            code: BuildCodeConfig::default(),
            assets: BuildAssetsConfig::default()
        }
    }
}

impl Default for BuildCodeConfig {
    fn default() -> Self {
        Self {
            command: vec![
                "wat2wasm".to_string(),
                "src/main.wat".to_string(),
                "-o".to_string(),
                "src/main.wasm".to_string(),
                // "cargo".to_string(),
                // "build".to_string(),
                // "--target".to_string(),
                // "wasm32-unknown-unknown".to_string(),
                // "--release".to_string()
            ],
            output: TomlPath(PathBuf::from("src/main.wasm")),
        }
    }
}

impl Default for BuildAssetsConfig {
    fn default() -> Self {
        Self {
            dir: "assets".to_string(),
            sprite_tiles: "spr.4bpp".to_string(),
            background_tiles: "bg.4bpp".to_string(),
            background_map: "bg.map".to_string(),
        }
    }
}

const DEFAULT_SPR_TILES: &[u8; 8192] = include_bytes!("assets/spr.4bpp");
const DEFAULT_BG_TILES: &[u8; 8192] = include_bytes!("assets/bg.4bpp");
const DEFAULT_MAP_DATA: &[u8; 65536] = &[0; 65536];
const DEFAULT_PROGRAM_CODE: &[u8] = include_bytes!("assets/main.wat");
pub const CONFIG_FILENAME: &str = "WASMCarts.toml";

impl ToolConfig {
    pub fn create_files(&self) -> std::io::Result<()> {
        use std::fs;
        let assets = &self.build.assets;
        let assets_dir = PathBuf::from(&assets.dir);
        let src_dir = PathBuf::from("src");

        let _ = fs::create_dir(&assets_dir);
        let _ = fs::create_dir(&src_dir);
        fs::write(assets_dir.join(&assets.sprite_tiles), DEFAULT_SPR_TILES)?;
        fs::write(assets_dir.join(&assets.background_tiles), DEFAULT_BG_TILES)?;
        fs::write(assets_dir.join(&assets.background_map), DEFAULT_MAP_DATA)?;
        fs::write(src_dir.join("main.wat"), DEFAULT_PROGRAM_CODE)?;

        let cfg_string = toml::to_string(&self).unwrap();
        fs::write(CONFIG_FILENAME, cfg_string)?;

        Ok(())
    }

    pub fn build_project(&self) -> std::io::Result<(String, usize)> {
        let mut cmd = std::process::Command::new(&self.build.code.command[0]);
        for c in self.build.code.command[1..].iter() {
            cmd.arg(c);
        }
        let result = cmd.status().expect("Failed to build");
        if !result.success() {
            return Err(std::io::Error::new(std::io::ErrorKind::Other, "Failed to build"));
        }

        use std::fs;
        let assets = &self.build.assets;
        let assets_dir = PathBuf::from(&assets.dir);
        let spr_tiles = fs::read(assets_dir.join(&self.build.assets.sprite_tiles))?;
        let bg_tiles = fs::read(assets_dir.join(&self.build.assets.background_tiles))?;
        let bg_map = fs::read(assets_dir.join(&self.build.assets.background_map))?;
        let prog = fs::read(&self.build.code.output.0)?;

        let mut output: Vec<u8> = vec![0; 0x40];
        output.extend_from_slice(&spr_tiles);
        output.extend_from_slice(&bg_tiles);
        output.extend_from_slice(&bg_map);
        output.extend_from_slice(&prog);

        let art_name = format!("{}.bin", &self.package.name);
        fs::write(&art_name, &output)?;

        Ok((art_name, output.len()))
    }
}

