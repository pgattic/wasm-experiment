use iced::{Alignment, Color, Element};
use iced::widget::{Button, text};
use iced::Background;

use iced_aw::helpers::color_picker;

#[derive(Debug, Clone)]
pub struct PalEntry {
    color: Color,
    show_picker: bool,
}

#[derive(Debug, Clone)]
pub enum PalEntryMessage {
    OpenPicker,
    ClosePicker,
    SetColor(Color),
}

impl Default for PalEntry {
    fn default() -> Self {
        Self {
            color: Color::WHITE,
            show_picker: false,
        }
    }
}

impl PalEntry {
    pub fn with_color(color: Color) -> Self {
        Self {
            color,
            show_picker: false,
        }
    }

    pub fn update(&mut self, message: PalEntryMessage) {
        match message {
            PalEntryMessage::OpenPicker => self.show_picker = true,
            PalEntryMessage::ClosePicker => self.show_picker = false,
            PalEntryMessage::SetColor(color) => {
                self.color = color;
                self.show_picker = false;
            },
        }
    }

    pub fn view(&self, id: usize) -> Element<PalEntryMessage> {

        let rect = text(id)
            .width(24)
            .align_x(Alignment::End);
        let button = Button::new(rect)
            .style(|_theme, _status| iced::widget::button::Style::default().with_background(Background::Color(self.color)))
            .on_press(PalEntryMessage::OpenPicker);

        color_picker(
            self.show_picker,
            self.color,
            button,
            PalEntryMessage::ClosePicker,
            PalEntryMessage::SetColor
        ).into()
    }
}

