
# WASM Runtime + Game Framework

Note that this is very much a work-in-progress

## Setup

I recommend using [Docker](https://www.docker.com/) for building this project. If you'd rather avoid that, refer to the [Dockerfile](./Dockerfile), which can also serve as a description for what is required to build the project.

- `docker build --tag wasmcarts:latest .` (may require `sudo`)
- `docker run --rm -v .:/work -it wasmcarts:latest` (may require `sudo`)
- Then, within the container, do one of the following:
    - Nintendo DS build:
        - `mkdir -p build/nds && cd build/nds`
        - `cmake ../.. -DTARGET=nds`
        - `make`
    - Linux build:
        - `mkdir -p build/linux && cd build/linux`
        - `cmake ../.. -DTARGET=linux`
        - `make`
    - Nintendo Wii build:
        - `mkdir -p build/wii && cd build/wii`
        - `cmake ../.. -DTARGET=wii`
        - `make`

