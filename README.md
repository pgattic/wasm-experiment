
# WASM

All I wanted was to make Nintendo DS games in Rust...

This is a work-in-progress Fantasy Console that targets embedded systems and allows users to create games in any language that [compiles to WebAssembly](https://webassembly.org/getting-started/developers-guide/).

## Roadmap

- [X] Learn some Docker
- [X] Get [WASM on NDS](https://softwayre.com/blog/2021/09/13/webassembly-on-your-nintendo-ds) running in a reproducible environment
- [X] Have it run a wasm file compiled from a different language than AssemblyScript
- [X] Have it load a file from file storage instead of embedding it
    - [X] File picker (needs work)
- [X] Support sprites
- [X] Define functions for game engine to expose
- [X] Port build system to CMake
    - [X] Create NDS implementation
    - [X] Create Desktop implementation ~(Raylib probably)~ *went with SDL3*
    - [ ] Create Wii implementation
- [ ] Sprite/map editor
- [ ] Support background tiles
- [ ] Memory mapping for tilemap/sprite manipulation
- [ ] Printing text on the screen
- [ ] Port to GBA?
    - Would require including the wasm as part of ROM data
    - Some heavy source code customization would be in order
    - Look into [Butano](https://github.com/GValiente/butano) for graphics

## Stretch Goals

- Networking
- In-engine editor

## Engine Spec

### Graphics

- 256 sprite tiles (8x8 pixels)
- 256 BG tiles (8x8 pixels)
- 16 color global palette
- Screen can be any dimensions up to 256x256

### ROM Layout

| Section | Bytes | Address range |
| - | -:| -:|
| Header | 16 | `0x00000-0x0000F` |
| Global Palette | 48 | `0x00010-0x0003F` |
| Sprite Tiles | 8192 | `0x00040-0x0203F` |
| BG Tiles | 8192 | `0x02040-0x0403F` |
| Tile Map (256x256 tiles) | 65536 | `0x04040-0x1403F` |
| Program Code | 966592 | `0x14040-0xFFFFF` |

### Functions

#### Graphics

| Done | Function/Usage | Description |
| - | - | - |
| ❌ | `circle(x, y, rad)` | Render a circle |
| ❌ | `circleFill(x, y, rad)` | Render a filled circle |
| ✅ | `clearScreen(color)` | Clear the screen to the specified color |
| ❌ | `ellipse(x, y, xRad, yRad)` | Render an ellipse |
| ❌ | `ellipseFill(x, y, xRad, yRad)` | Render a filled ellipse |
| ❌ | `print(x, y, text)` | Print a string of text to the display |
| ✅ | `pixel(x, y, color)` | Draw a single pixel at the specified location |
| ✅ | `rect(x, y, w, h, color)` | Draw an outlined rectangle at the specified position and size |
| ✅ | `rectFill(x, y, w, h, color)` | Draw a filled rectangle at the specified position and size |
| ✅ | `sprite(x, y, id)` | Render a sprite of the designated id |
| ❌ | `tileMap(x, y, tileX, tileY, mapW, mapH)` | Display a tilemap |

#### Input

| Done | Function/Usage | Description |
| - | - | - |
| ✅ | `button(btn)` | Check if a given button ID is being held |
| ✅ | `buttonPressed(btn)` | Check if a given button ID was just pressed |

#### Miscellaneous

| Done | Function/Usage | Description |
| - | - | - |
| ✅ | `debugPrint(text)` | Output a string to a debug console (stdout, log file, etc.) |
| ✅ | `rand()` | Calculate a pseudo-random 32-bit integer |

## Thanks

This project stands on the backs of giants. During my work, I felt a need to give special thanks to:

- Martin Moxon for his [WebAssembly On Your Nintendo DS](https://softwayre.com/blog/2021/09/13/webassembly-on-your-nintendo-ds) blog post, which gave me just the right kind of inspiration I needed to start this thing!
- [AntonioND](https://github.com/AntonioND) for his superhuman work put into the NDS and GBA development scene, particularly through [BlocksDS](https://blocksds.skylyrac.net/docs)
- [Asie](https://github.com/asiekierka) for giving us [Wonderful Toolchain](https://wonderful.asie.pl/), which included very up-to-date compilers and other build tools for the Nintendo DS.
- [vshymanskyy](https://github.com/vshymanskyy) for creating [wasm3](https://github.com/wasm3)
- Anyone else who has contributed to WebAssembly, Nintendo DS Homebrew, and SDL3! You rock!

