
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
  # Ensure environment variables are set
  if(NOT DEFINED ENV{BLOCKSDS})
    message(FATAL_ERROR "Ensure that environment variable BLOCKSDS is set.\n(see https://blocksds.skylyrac.net/docs/setup/options/linux/)")
  else()
    set(BLOCKSDS $ENV{BLOCKSDS})
  endif()
  if(NOT DEFINED ENV{BLOCKSDSEXT})
    message(FATAL_ERROR "Ensure that environment variable BLOCKSDSEXT is set.\n(see https://blocksds.skylyrac.net/docs/setup/options/linux/)")
  else()
    set(BLOCKSDSEXT $ENV{BLOCKSDSEXT})
  endif()

  # Ensure WASM3 installed correctly
  if(NOT EXISTS ${BLOCKSDSEXT}/wasm3/include OR NOT EXISTS ${BLOCKSDSEXT}/wasm3/lib/libwasm3.a)
    message(FATAL_ERROR "WASM3 not compiled and installed to the correct location.\n(see Dockerfile, search for \"Build WASM3 for NDS\")")
  endif()

  # Include/Link WASM3 build
  target_include_directories(WASMCarts PRIVATE
    ${BLOCKSDSEXT}/wasm3/include
  )
  target_link_libraries(WASMCarts PRIVATE
    ${BLOCKSDSEXT}/wasm3/lib/libwasm3.a
  )
  # NDS-specific source files
  target_sources(WASMCarts PRIVATE
    src/nds/platform_impl.c
    src/nds/graphics.c
    src/nds/keys.c
  )

  # Build NDS rom (function provided in blocksds's `BlocksDS.cmake` file)
  nds_create_rom(WASMCarts
    NAME "WASMCarts"
    SUBTITLE "Lightweight Fantasy Console"
    AUTHOR "By pgattic"
    ICON "${CMAKE_CURRENT_SOURCE_DIR}/icon.bmp"
  )
elseif(LINUX)
  target_sources(WASMCarts PRIVATE
    src/linux/platform_impl.c
    src/linux/graphics.c
  )

  # Include/Link WASM3 build
  target_include_directories(WASMCarts PRIVATE
    /usr/local/include
  )
  target_link_libraries(WASMCarts PRIVATE
    /usr/local/lib/libwasm3.a
  )
endif()

