
# WASMCarts Engine

Note that this is very much a work-in-progress

## Setup

[Nix](https://nixos.org/) is available as a build option for the Linux target. [Docker](https://www.docker.com/) is used for building all other targets, and can also build for Linux. If you'd rather use some other setup, refer to the [Dockerfile](./Dockerfile), which can also serve as a description for what is required to build the project.

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

