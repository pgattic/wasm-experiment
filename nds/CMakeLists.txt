
cmake_minimum_required(VERSION 3.16)

project(WASMCarts C)

add_executable(WASMCarts
  src/main.c
)

target_sources(WASMCarts PRIVATE
  src/cartridge.c
  src/api.c
  src/platform.c
)

if(NINTENDO_DS)
  set(BLOCKSDS $ENV{BLOCKSDS})
  set(BLOCKSDSEXT $ENV{BLOCKSDSEXT})
  target_include_directories(WASMCarts PRIVATE
    ${BLOCKSDSEXT}/wasm3/include
  )
  target_link_libraries(WASMCarts PRIVATE
    ${BLOCKSDSEXT}/wasm3/lib/libwasm3.a
  )
  target_sources(WASMCarts PRIVATE
    src/nds/graphics.c
    src/nds/keys.c
    src/nds/platform_impl.c
  )
  nds_create_rom(WASMCarts
    NAME "WASMCarts"
    SUBTITLE "Lightweight Fantasy Console"
    AUTHOR "By pgattic"
    # GAME_ICON icon.bmp
  )
# else()
#   target_sources(WASMCarts PRIVATE
#     source/nds/platform_impl.c
#   )
endif()


