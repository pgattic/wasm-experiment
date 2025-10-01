
# WASM Experiment - WASMCarts

This is a work-in-progress Fantasy Console that targets embedded systems and allows users to create games in any language that [compiles to WebAssembly](https://webassembly.org/getting-started/developers-guide/).

![WASMCarts Game Engine](nds-linux-wii.jpg)

## What is it?

In short, WASMCarts is a [Fantasy Console](https://en.wikipedia.org/wiki/Fantasy_video_game_console) whose "CPU" is just a [WebAssembly](https://webassembly.org/) runtime. This project takes advantage of two unique properties of WebAssembly:

1. It is etremely efficient to virtualize and highly embeddable (see [wasm3](https://github.com/wasm3/wasm3))
2. It is supported as a compile target by [many popular programming languages](https://webassembly.org/getting-started/developers-guide/).

As a result, this fantasy console is lean and embeddable, and it automatically supports languages like TypeScript, Rust, C, Golang, Zig, and Swift.

WebAssembly is so efficient, that I was able to make it run on my Nintendo DS!

What the engine does is take the [wasm3](https://github.com/wasm3/wasm3) runtime and expose some additional [system-dependent functions](#functions) such as rendering things on the screen and obtaining user input. Then, when it is running the WASM-compiled code that you write, it can now correctly respond when you tell it to `rectFill(x, y, width, height, color)`. This means that you have an extremely simple API to work with, and there is minimal friction between your code and the native C code being executed.

Here's an example of a simple game written in TypeScript, which would run in WASMCarts:

```typescript
import * as env from "./env";

// Global data
let playerX: i32;
let playerY: i32;

export function setup(): void {
    playerX = 12; // Starting X position
    playerY = 12; // Starting Y position
}

export function update(): void {
    // Move the player
    if (env.btn(env.Button.Left))  { playerX -= 4; }
    if (env.btn(env.Button.Right)) { playerX += 4; }
    if (env.btn(env.Button.Up))    { playerY -= 4; }
    if (env.btn(env.Button.Down))  { playerY += 4; }

    env.clearScreen(env.Colors.Black);

    // Draw the player at new position
    env.rectFill(playerX, playerY, 8, 8, env.Colors.Red);
}
```

Please note that this project is in early stages of development, and the process for properly compiling code and preparing it to run in the console is not well-documented yet. However, this code has been demonstrated to compile and run in WASMCarts.

## Roadmap

- [X] Learn some Docker
- [X] Get [WASM on NDS](https://softwayre.com/blog/2021/09/13/webassembly-on-your-nintendo-ds) running in a reproducible environment
- [X] Have it run a wasm file compiled from a different language than AssemblyScript
- [X] Have it load a file from file storage instead of embedding it
    - [X] File picker
- [X] Support sprites
- [X] Define functions for game engine to expose
- [X] Port build system to CMake
    - [X] Create NDS implementation
    - [X] Create Desktop implementation ~(Raylib probably)~ *went with SDL3*
    - [X] Create Wii implementation
- [X] Build tool
- [X] Ingame menu
- [ ] Sprite/map editing solution
- [ ] Support background tiles
- [ ] Memory mapping for tilemap/sprite manipulation
- [X] Printing text on the screen

## Stretch Goals

- Networking
- In-engine editor
- Port to GBA?
    - Would require including the wasm as part of ROM data
    - Some heavy source code customization would be in order
    - Look into [Butano](https://github.com/GValiente/butano) for graphics

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
| Program Code | <=966592 | `0x14040-0xFFFFF` |

### Functions

#### Graphics

| Done | Function/Usage | Description |
| - | - | - |
| ❌ | `circle(x, y, rad)` | Render a circle |
| ❌ | `circleFill(x, y, rad)` | Render a filled circle |
| ✅ | `clearScreen(color)` | Clear the screen to the specified color |
| ❌ | `ellipse(x, y, xRad, yRad)` | Render an ellipse |
| ❌ | `ellipseFill(x, y, xRad, yRad)` | Render a filled ellipse |
| ✅ | `print(x, y, text)` | Print a string of text to the display |
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
- [vshymanskyy](https://github.com/vshymanskyy) for creating [wasm3](https://github.com/wasm3/wasm3)
- Anyone else who has contributed to WebAssembly, Nintendo DS Homebrew, and SDL3! You rock!

