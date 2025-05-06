
# Officially Supported Languages

Contained here are the programming languages that I have gotten to run reasonably fast on the fantasy console.

Each folder contains examples and, if applicable, libraries to facilitate making games for the fantasy console.

## Language Support Matrix

I will not put a language in this matrix unless it has full, portable WASM compilation support. That excludes Python and Lua, unfortunately.

| Language | Support | Comments |
| - | - | - |
| C/C++ | ❌ |  |
| D | 💭 | [Should be simple](https://wiki.dlang.org/Generating_WebAssembly_with_LDC). |
| Dart | ❌ | Need more info about its ability to run in embedded environments |
| Golang | 🚧 | Just has a demo working |
| Haskell | ❌ | Need to research more about how optimal the produced code is |
| Rust | ✅ | Made a library with thorough documentation |
| Swift | ❌ |  |
| TypeScript (AssemblyScript) | 🚧 | Just has an example working |
| Zig | 💭 |  |

Legend

| Symbol | Meaning |
| - | - |
| ❌ | Support seems possible, and I am open to the idea. |
| 💭 | Currently considering support, work may begin soon. Looking for contributors! |
| 🚧 | Support is WIP, may be suitable for you if you're pretty familiar with the language. |
| ✅ | Full engine API support, along with reasonable documentation. suitable for full games. |

Despite Rust having the most complete implementation, it probably shouldn't be used as an example for other languages. Ideally the implementation should be as small as possible. Rust just [needed extra work](rust/README.md).

