
# WASM Runtime + Game Framework

Note that this is very much a work-in-progress

## Setup

The installation scripts require [Docker](https://www.docker.com/) be installed on your system. I understand how it can sometimes feel unnecessary, but I did not want all these build dependencies installed on my system. Besides, I saw this project as an excellent opportunity to learn Docker.

- `./docker_build.sh` (may require `sudo`)
- `./dev_env.sh` (may require `sudo`)
    - Nintendo DS build:
        - `mkdir -p build/nds && cd build/nds`
        - `cmake ../.. -DTARGET=nds -DCMAKE_TOOLCHAIN_FILE=$BLOCKSDS/cmake/BlocksDS.cmake`
        - `make`
    - Linux build:
        - `mkdir -p build/linux && cd build/linux`
        - `cmake ../.. -DTARGET=linux`
        - `make`

