
# WASM

## Roadmap

- [X] Learn some Docker
- [X] Get [WASM on NDS](https://softwayre.com/blog/2021/09/13/webassembly-on-your-nintendo-ds) running in a reproducible environment
    - NOTE: Partially done (but satisfactory), It is confirmed to execute WASM code!
- [X] Have it run a wasm file compiled from a different language than AssemblyScript
- [X] Have it load a file from file storage instead of embedding it
    - [X] File picker (needs work)
- [X] Support sprites
- [ ] Support background tiles
- [ ] Memory mapping for tilemap/sprite manipulation
- [ ] Printing text on the screen
- [ ] Port to GBA?
    - [ ] Will require learning a new graphice library for GBA
- [ ] Define functions for game engine to expose
- [ ] Good editor/terminal
    - Goal: have it share a renderer with the main engine, hopefully make it equally/similarly portable

## Stretch Goals

Mostly writing these down just for the heck of it.

- Networking???

## Engine Spec

- Will read up on TIC-80 docs or PICO-8 docs for advice
- ~~The host hardware specifies the resolution of the sreen, up to 256x256 (two bytes, 0x00 means 256)~~
- Just spitballing here, I think the GBA resolution divided by two might be nice (120*80px)

### ROM Layout

| Section | Bytes | Address range |
| - | -:| -:|
| Header | 16 | `0x00000-0x0000F` |
| Global Palette | 48 | `0x00010-0x0003F` |
| Sprite Tiles | 8192 | `0x00040-0x0203F` |
| BG Tiles | 8192 | `0x02040-0x0403F` |
| Program Code | 1032128 | `0x04040-0xFFFFF` |

TODO: Background Tilemaps

### Graphics

- 256 sprite tiles (8x8 pixels)
    - 4bpp tiles * 64 pixels per tile * 256 tiles = 8192 bytes
- 256 BG tiles (8x8 pixels)
- 16 color global palette
    - 16 colors * 3 channels * 1 byte = 48
- Screen can be any dimensions up to 256x256

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
| ✅ | `syncFrame()` | Wait for the frame time to finish (currently also clears the screen to black) |
| ❌ | `tileMap(x, y, tileX, tileY, mapW, mapH)` | Display a tilemap |

#### Input

| Done | Function/Usage | Description |
| - | - | - |
| ✅ | `button(btn)` | Check if a given button ID is being held |
| ✅ | `buttonPressed(btn)` | Check if a given button ID was just pressed |

### Miscellaneous

| Done | Function/Usage | Description |
| - | - | - |
| ✅ | `debugPrint(text)` | Output a string to a debug console (stdout, log file, etc.) |
| ✅ | `rand()` | Calculate a pseudo-random 32-bit integer |

