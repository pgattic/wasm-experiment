use std::path::PathBuf;
use serde::{Deserialize, Serialize};
use crate::toml_path::TomlPath;

#[derive(Serialize, Deserialize, Debug)]
pub struct ToolConfig {
    package: PackageConfig,
    build: BuildConfig,
}

#[derive(Serialize, Deserialize, Debug)]
struct PackageConfig {
    name: String,
    author: String,
    version: String,
}

#[derive(Serialize, Deserialize, Debug)]
struct BuildConfig {
    compile_command: Vec<String>,
    paths: PathsConfig,
}

#[derive(Serialize, Deserialize, Debug)]
struct PathsConfig {
    wasm_code: TomlPath,
    palette: String,
    sprite_tiles: String,
    background_tiles: String,
    background_map: String,
}
//
// impl Default for ToolConfig {
//     fn default() -> Self {
//         Self {
//             package: PackageConfig {
//                 name: "My Game".to_string(),
//                 author: "Me".to_string(),
//                 version: "0.1.0".to_string()
//             },
//             build: BuildConfig {
//                 compile_command: vec!["cargo".to_string(), "build".to_string(), "--release".to_string()],
//                 paths: PathsConfig {
//                     wasm_code: TomlPath(PathBuf::from("target/wasm32-unknown-unknown/release/output.wasm")),
//                     palette: "palette.pal".to_string(),
//                     sprite_tiles: "spr.chr".to_string(),
//                     background_tiles: "bg.chr".to_string(),
//                     background_map: "bg.map".to_string(),
//                 }
//             }
//         }
//     }
// }

impl ToolConfig {
    pub fn with_name(name: &str) -> Self {
        Self {
            package: PackageConfig {
                name: name.to_string(),
                author: "Me".to_string(),
                version: "0.1.0".to_string()
            },
            build: BuildConfig {
                compile_command: vec!["cargo".to_string(), "build".to_string(), "--release".to_string()],
                paths: PathsConfig {
                    wasm_code: TomlPath(PathBuf::from("target/wasm32-unknown-unknown/release/output.wasm")),
                    palette: "palette.pal".to_string(),
                    sprite_tiles: "spr.chr".to_string(),
                    background_tiles: "bg.chr".to_string(),
                    background_map: "bg.map".to_string(),
                }
            }
        }
    }
}

