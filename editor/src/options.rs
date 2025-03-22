use iced::{widget::{column, row, text, text_input}, Alignment, Element};
//use std::path::PathBuf;

#[derive(Default)]
pub struct Options {
    work_dir: String,
    build_command: String,
    build_output_path: String,
}

#[derive(Clone, Debug)]
pub enum OptionsMessage {
    UpdateWorkDir(String),
    UpdateBuildCommand(String),
    UpdateBuildOutput(String),
}

impl Options {
    pub fn update(&mut self, message: OptionsMessage) {
        match message {
            OptionsMessage::UpdateWorkDir(dir) => self.work_dir = dir,
            OptionsMessage::UpdateBuildCommand(cmd) => self.build_command = cmd,
            OptionsMessage::UpdateBuildOutput(file) => self.build_output_path = file,
        }
    }

    pub fn view(&self) -> Element<OptionsMessage> {
        column![
            row![
                text("Work Directory: "),
                text_input("Type something here...", &self.work_dir).on_input(OptionsMessage::UpdateWorkDir)
            ].align_y(Alignment::Center),
            row![
                text("Build Command: "),
                text_input("Type something here...", &self.build_command).on_input(OptionsMessage::UpdateBuildCommand)
            ].align_y(Alignment::Center),
            row![
                text("Build Output Path (to .wasm file): "),
                text_input("Type something here...", &self.build_output_path).on_input(OptionsMessage::UpdateBuildOutput)
            ].align_y(Alignment::Center),
        ]
            .spacing(2)
            .into()
    }
}

