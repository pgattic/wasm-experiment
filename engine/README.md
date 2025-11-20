
# WASMCarts Engine

Note that this is very much a work-in-progress

## Setup

[CMake](https://cmake.org/) is the build system used for compiling this software. [Nix](https://nixos.org/) is available as a build environment for the Linux target, and [Docker](https://www.docker.com/) is available for building all targets, including Linux. If you'd rather use some other setup, refer to the [Dockerfile](./Dockerfile), which can also serve as a description for what is required to build the project.

### Nix (Targeting Linux)

- `nix develop`
- `mkdir -p build/linux && cd build/linux`
- `cmake ../.. -DTARGET=linux`
- `make`

### Docker (Targeting All Supported Systems)

First, make sure the Docker service is running (`sudo systemctl start docker`)

- (`sudo`) `docker build --tag wasmcarts:latest .` (may require `--network=host`)
- (`sudo`) `docker run --rm -v .:/work -it wasmcarts:latest` (may require `--network=host`)
- Then, within the container, do one of the following:
    - Linux build:
        - `mkdir -p build/linux && cd build/linux`
        - `cmake ../.. -DTARGET=linux`
        - `make`
    - Nintendo DS build:
        - `mkdir -p build/nds && cd build/nds`
        - `cmake ../.. -DTARGET=nds`
        - `make`
    - Nintendo Wii build:
        - `mkdir -p build/wii && cd build/wii`
        - `cmake ../.. -DTARGET=wii`
        - `make`
    - Nintendo GameCube build:
        - `mkdir -p build/gamecube && cd build/gamecube`
        - `cmake ../.. -DTARGET=gamecube`
        - `make`

Add `-DCMAKE_BUILD_TYPE=Debug` to the `cmake` invocation to produce a debug build environment (disables LTO and other compiler optimizations, uses bottom screen as console on NDS, etc.)

## File Structure

`assets/` contains the static palette and font file used by all target systems. It also has an icon file, used by only the Nintendo DS build so far. `src/` has the engine source code.

Since this engine is meant to be cross-platform, the codebase is as reusable as possible. Each platform's code is supposed to implement `src/platform.h`, which contains the names of all hardware-related functions that the engine requires. The Linux implementation of `src/platform.h` for example, is found in `src/linux/platform.c`.

`src/api.c` defines the public API that WASMCarts games can access. It exposes certain platform-side functions to the WASM runtime.

