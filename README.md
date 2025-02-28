
# WASM

## Roadmap

- [ ] Learn some Docker
- [ ] Get [WASM on NDS](https://softwayre.com/blog/2021/09/13/webassembly-on-your-nintendo-ds) running in a reproducible environment
- [ ] Have it run a wasm file compiled from a different language than AssemblyScript
- [ ] Have it load a file from file storage instead of embedding it?
    - [ ] Also add file picker?
- [ ] Port to GBA
    - [ ] Will require learning a new graphice library for GBA
- [ ] Define functions for game engine to expose

## Engine Spec

- Will read up on TIC-80 docs or PICO-8 docs for advice
- The host hardware specifies the resolution of the sreen, up to 256x256 (two bytes, 0x00 means 256)

### ROM Layout

| Section | Bytes | Address range |
| - | - | - |
| Header | 16 | 0x0000-0x000F |
| Global Palette | 48 | 0x0010-0x003F |
| Sprite Tiles | 8192 | 0x0040 - 0x203F |
| BG Tiles | 8192 | 0x2040-0x4040 |

### Graphics

- 256 sprite tiles (8x8 pixels)
    - 4bpp tiles * 64 pixels per tile * 256 tiles = 8192 bytes
- 256 BG tiles (8x8 pixels)
- 16 color global palette
    - 16 colors * 3 channels * 1 byte = 48
- Screen can be any dimensions up to 256x256

### Functions

<!--| Function | Usage | Description |-->
<!--| - | - | - |-->
<!--| `cls(color)` | -->

