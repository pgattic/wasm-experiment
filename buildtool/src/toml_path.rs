use std::path::PathBuf;
use serde::{Deserialize, Serialize};

#[derive(Debug)]
pub struct TomlPath(pub PathBuf);

impl Serialize for TomlPath {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where S: serde::Serializer {
        serializer.serialize_str(self.0.to_str().unwrap())
    }
}

impl<'de> Deserialize<'de> for TomlPath {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where D: serde::Deserializer<'de> {
        let s = String::deserialize(deserializer)?;
        Ok(TomlPath(PathBuf::from(s)))
    }
}

