
cmake_minimum_required(VERSION 4.1)
project(WASMCarts C)

add_executable(WASMCarts)
target_compile_options(WASMCarts PRIVATE -Wall)
target_compile_definitions(WASMCarts PRIVATE "$<$<CONFIG:Debug>:DEBUG>") # Define DEBUG for C sources

# ------------------------------------------------------------------------------
# wasm3 from $WASM3_SOURCE
# ------------------------------------------------------------------------------

# I would try to use wasm3's CMakeLists, but it expects a deprecated CMake version

if(NOT DEFINED ENV{WASM3_SOURCE})
  message(FATAL_ERROR
    "WASM3_SOURCE environment variable not set.\n"
    "Set it to the root of the wasm3 source checkout (it must contain 'source/*.c')."
  )
endif()

set(WASM3_ROOT "$ENV{WASM3_SOURCE}")
set(WASM3_SRC  "${WASM3_ROOT}/source")

if(NOT EXISTS "${WASM3_SRC}")
  message(FATAL_ERROR "WASM3_SOURCE='${WASM3_ROOT}' does not contain a 'source/' directory.")
endif()

file(GLOB WASM3_SOURCES "${WASM3_SRC}/*.c") # List of wasm3's .c files

target_include_directories(WASMCarts PRIVATE "${WASM3_SRC}")
target_sources(WASMCarts PRIVATE ${WASM3_SOURCES})

# ------------------------------------------------------------------------------
# Main executable
# ------------------------------------------------------------------------------

# Set platform defines (for C code to use)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_compile_definitions(WASMCarts PRIVATE PLATFORM_LINUX)
elseif(CMAKE_SYSTEM_NAME STREQUAL "NintendoDS")
  target_compile_definitions(WASMCarts PRIVATE PLATFORM_DS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "NintendoWii")
  set(OGC_PLATFORM "wii")
  target_compile_definitions(WASMCarts PRIVATE PLATFORM_WII)
elseif(CMAKE_SYSTEM_NAME STREQUAL "NintendoGameCube")
  set(OGC_PLATFORM "gamecube")
  target_compile_definitions(WASMCarts PRIVATE PLATFORM_GAMECUBE)
else()
  message(FATAL_ERROR
    "Could not determine target system.\n"
    "Please ensure CMAKE_SYSTEM_NAME variable is set by a toolchain file."
  )
endif()

# General source files
target_sources(WASMCarts PRIVATE
  src/api.c
  src/f_sel.c
  src/file_list.c
  src/game.c
  src/game_menu.c
  src/main.c
)

# ------------------------------------------------------------------------------
# Platform-specific stuff
# ------------------------------------------------------------------------------

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

  # LINUX VERSION
  set_target_properties(WASMCarts PROPERTIES OUTPUT_NAME wasmcarts)

  target_sources(WASMCarts PRIVATE
    src/linux/platform.c
    src/linux/graphics.c
  )

  # SDL3 via pkg-config
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)

  target_link_libraries(WASMCarts PRIVATE m PkgConfig::SDL3)

elseif(CMAKE_SYSTEM_NAME STREQUAL "NintendoDS")

  # NINTENDO DS VERSION
  target_sources(WASMCarts PRIVATE
    src/nds/platform.c
    src/nds/graphics.c
    src/nds/keys.c
  )

  target_link_libraries(WASMCarts PRIVATE m)

  nds_create_rom(WASMCarts
    NAME "WASMCarts"
    SUBTITLE "Lightweight Fantasy Console"
    AUTHOR "By pgattic"
    ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/nds-icon.bmp"
  )

elseif(CMAKE_SYSTEM_NAME STREQUAL "NintendoWii" OR CMAKE_SYSTEM_NAME STREQUAL "NintendoGameCube")

  # NINTENDO WII / GAMECUBE VERSION
  target_sources(WASMCarts PRIVATE
    src/gcwii/graphics.c
    src/gcwii/keys_${OGC_PLATFORM}.c
    src/gcwii/platform.c
  )

  if(CMAKE_SYSTEM_NAME STREQUAL "NintendoWii")
    target_link_libraries(WASMCarts PRIVATE wiiuse)
  endif()
  target_link_libraries(WASMCarts PRIVATE m fat ogc)

  ogc_create_dol(WASMCarts)

endif()

