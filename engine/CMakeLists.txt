
cmake_minimum_required(VERSION 3.16)

project(WASMCarts C)

add_executable(WASMCarts
  src/main.c
)

# General source files
target_sources(WASMCarts PRIVATE
  src/cartridge.c
  src/api.c
  src/platform.c
)

if(NINTENDO_DS)
  set(BLOCKSDS $ENV{BLOCKSDS})
  set(BLOCKSDSEXT $ENV{BLOCKSDSEXT})

  # Link WASM3 build
  target_include_directories(WASMCarts PRIVATE
    ${BLOCKSDSEXT}/wasm3/include
  )
  target_link_libraries(WASMCarts PRIVATE
    ${BLOCKSDSEXT}/wasm3/lib/libwasm3.a
  )

  # NDS-specific source files
  target_sources(WASMCarts PRIVATE
    src/nds/graphics.c
    src/nds/keys.c
    src/nds/platform_impl.c
  )
  nds_create_rom(WASMCarts
    NAME "WASMCarts"
    SUBTITLE "Lightweight Fantasy Console"
    AUTHOR "By pgattic"
    ICON "icon.bmp"
  )
# elseif(LINUX)
#   target_sources(WASMCarts PRIVATE
#     source/linux/platform_impl.c
#   )
endif()


