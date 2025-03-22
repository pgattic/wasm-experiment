use iced::{Element, widget::{column, text, Row}, Color, Theme};

mod pal_entry;
use crate::pal_entry::PalEntry;

mod options;
use crate::options::Options;

//enum AbsOrRel {
//    Absolute(PathBuf),
//    Relative(PathBuf),
//}
//
//impl Default for AbsOrRel {
//    fn default() -> Self {
//        Self::Relative(PathBuf::new())
//    }
//}

struct App {
    pal_entries: [PalEntry; 16],
    options: Options,
}

#[derive(Debug, Clone)]
enum AppMessage {
    PalEntryMsg(usize, pal_entry::PalEntryMessage),
    OptionMsg(options::OptionsMessage),
}


impl Default for App {
    fn default() -> Self {
        Self {
            pal_entries: [ // Stolen from PICO-8
                PalEntry::with_color(Color::from_rgb8(  0,   0,   0)),
                PalEntry::with_color(Color::from_rgb8( 29,  43,  83)),
                PalEntry::with_color(Color::from_rgb8(126,  37,  83)),
                PalEntry::with_color(Color::from_rgb8(  0, 135,  81)),
                PalEntry::with_color(Color::from_rgb8(171,  82,  54)),
                PalEntry::with_color(Color::from_rgb8( 95,  87,  79)),
                PalEntry::with_color(Color::from_rgb8(194, 195, 199)),
                PalEntry::with_color(Color::from_rgb8(255, 241, 232)),
                PalEntry::with_color(Color::from_rgb8(255,   0,  77)),
                PalEntry::with_color(Color::from_rgb8(255, 163,   0)),
                PalEntry::with_color(Color::from_rgb8(255, 236,  39)),
                PalEntry::with_color(Color::from_rgb8(  0, 228,  54)),
                PalEntry::with_color(Color::from_rgb8( 41, 173, 255)),
                PalEntry::with_color(Color::from_rgb8(131, 118, 156)),
                PalEntry::with_color(Color::from_rgb8(255, 119, 168)),
                PalEntry::with_color(Color::from_rgb8(255, 204, 170)),
            ],
            options: Options::default()
        }
    }
}

impl App {
    fn update(&mut self, message: AppMessage) {
        match message {
            AppMessage::PalEntryMsg(id, message) => self.pal_entries[id].update(message),
            AppMessage::OptionMsg(message) => self.options.update(message),
        }
    }

    fn view(&self) -> Element<AppMessage> {
        column![
            text("Project Options:"),
            self.options.view().map(|msg| AppMessage::OptionMsg(msg)),
            text("Palette:"),
            {
                // 8x2 grid of colors
                let mut row1 = Row::new();
                let mut row2 = Row::new();

                for (id, entry) in self.pal_entries.iter().enumerate() {
                    let child = entry.view(id).map(move |msg| AppMessage::PalEntryMsg(id, msg));

                    match id {
                        ..8 => row1 = row1.push(child),
                        8.. => row2 = row2.push(child),
                    }
                }
                column![row1, row2]
            } 
        ]
            .spacing(12)
            .padding(16)
            .into()
    }
}

pub fn main() -> iced::Result {
    iced::application("Wasm Experiment - Editor", App::update, App::view)
        .theme(|_| Theme::Dark)
        .run()
}

