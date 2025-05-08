use clap::Parser;

#[derive(Parser)]

pub enum Cli {
    Build,
    Init,
}

