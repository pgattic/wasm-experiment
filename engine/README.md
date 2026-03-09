
# WASMCarts Engine

## Compiling

This project uses [CMake](https://cmake.org) and [Nix](https://nixos.org) as the main supported build setup.

### Linux

- `nix build ..#engine-linux` to build, or
- `nix run ..#engine-linux` to run

### Nintendo DS

- `nix build ..#engine-nds` to build

### Nintendo Wii

- `nix build ..#engine-wii` to build

### Nintendo GameCube

- `nix build ..#engine-gamecube` to build

Output will be in `./result`.

## Development Shell

Type `nix develop ..#engine` to enter the development shell, which includes all required toolchains and libraries. Once in the shell, run these commands to build for the following systems:

- Build for Linux:
    - `cmake -S . -B build/linux -DTARGET=linux`
    - `cmake --build build/linux`
- Build for Nintendo DS:
    - `cmake -S . -B build/nds -DTARGET=nds -DCMAKE_TOOLCHAIN_FILE="$BLOCKSDS/cmake/BlocksDS.cmake"`
    - `cmake --build build/nds`
- Build for Nintendo Wii:
    - `cmake -S . -B build/wii -DTARGET=wii -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/Wii.cmake"`
    - `cmake --build build/wii`
- Build for Nintendo GameCube:
    - `cmake -S . -B build/gamecube -DTARGET=gamecube -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/GameCube.cmake"`
    - `cmake --build build/gamecube`

The outputted program will be in `build/linux` or `build/nds`, etc. depending on the selected system.

Add `-DCMAKE_BUILD_TYPE=Debug` to the first `cmake` invocation to produce debug build scripts (disables LTO and other compiler optimizations, uses bottom screen as console on NDS, etc.)

## Not using Nix?

Too bad!

Just kidding. However, the following build steps aren't officially supported.

1. Have the [wasm3](https://github.com/wasm3/wasm3) repo cloned somewhere, with the `WASM3_SOURCE` env var set to the repository root.
2. Ensure you have the following dependencies installed for each system you'd like to build for:
    - Linux: `sdl3` development libraries (sometimes called `SDL3-devel`, `libsdl3-dev` or something, depending on your distribution)
    - Nintendo DS: [BlocksDS](https://blocksds.skylyrac.net/docs/setup), including the `BLOCKSDS` and `BLOCKSDSEXT` env vars
    - Nintendo Wii/GameCube: [DevkitPro](https://devkitpro.org/wiki/devkitPro_pacman) with the `wii-dev` and/or `gamecube-dev` packages, including the `DEVKITPRO` and `DEVKITPPC` env vars
3. Run the desired commands in the [Development Shell](#development-shell) section to build WASMCarts!

## File Structure

`assets/` contains the static palette and font file used by all target systems. It also has an icon file, used by only the Nintendo DS build so far. `src/` has the engine source code.

Since this engine is meant to be cross-platform, the codebase is as reusable as possible. Each platform's code is supposed to implement `src/platform.h`, which contains the names of all hardware-related functions that the engine requires. The Linux implementation of `src/platform.h` for example, is found in `src/linux/platform.c`.

`src/api.c` defines the public API that WASMCarts games can access. It exposes certain platform-side functions to the WASM runtime.

