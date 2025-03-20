
# Nintendo DS WASM Runtime + Game Framework

Note that this is very much a work-in-progress

## Setup

The installation scripts require [Docker](https://www.docker.com/) be installed on your system. I understand how it can sometimes feel unnecessary, but I did not want all these build dependencies installed on my system. Besides, I saw this project as an excellent opportunity to learn Docker.

- `./docker_build.sh` (may require `sudo`)
- `./dev_env.sh` (may require `sudo`)
    - Within the dev environment: `make`

## Resources

The following resources were of indispensable help to me as I made this happen:

- Martin Moxon's [WebAssembly On Your Nintendo DS](https://softwayre.com/blog/2021/09/13/webassembly-on-your-nintendo-ds)
    - The [WASM build workflow](https://github.com/moxon6/nds-dependencies/blob/main/.github/workflows/wasm3.yml) was excellent! Reworked it to work with BlocksDS too
- The [BlocksDS Docs](https://blocksds.skylyrac.net/docs) for giving me a superb starting place for everything

## Thanks

A special thanks to:

- [AntonioND](https://github.com/AntonioND) for his superhuman work put into the NDS and GBA development scene
- [vshymanskyy](https://github.com/vshymanskyy) for creating [wasm3](https://github.com/wasm3)
- Brunni for [µlib](https://www.gamebrew.org/wiki/%CE%9CLibrary)

