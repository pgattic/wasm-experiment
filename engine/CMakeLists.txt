cmake_minimum_required(VERSION 4.1)

# ------------------------------------------------------------------------------
# Target selection / toolchain
# ------------------------------------------------------------------------------

if(NOT DEFINED TARGET)
  message(FATAL_ERROR
    "No TARGET specified. Use `-DTARGET=` to specify one.\n"
    "Supported values: 'linux', 'nds', 'wii', 'gamecube'."
  )
endif()

# Automatically configure toolchain file based on TARGET (unless user already set one)
# if(NOT CMAKE_TOOLCHAIN_FILE)
  if("${TARGET}" STREQUAL "nds")
    if(DEFINED ENV{BLOCKSDS})
      set(CMAKE_TOOLCHAIN_FILE "$ENV{BLOCKSDS}/cmake/BlocksDS.cmake" CACHE STRING "Toolchain for NDS" FORCE)
    else()
      message(FATAL_ERROR "BLOCKSDS environment variable not set")
    endif()
  elseif("${TARGET}" STREQUAL "wii")
    if(DEFINED ENV{DEVKITPRO})
      set(CMAKE_TOOLCHAIN_FILE "$ENV{DEVKITPRO}/cmake/Wii.cmake" CACHE STRING "Toolchain for Wii" FORCE)
    else()
      message(FATAL_ERROR "DEVKITPRO environment variable not set")
    endif()
  elseif("${TARGET}" STREQUAL "gamecube")
    if(DEFINED ENV{DEVKITPRO})
      set(CMAKE_TOOLCHAIN_FILE "$ENV{DEVKITPRO}/cmake/GameCube.cmake" CACHE STRING "Toolchain for GameCube" FORCE)
    else()
      message(FATAL_ERROR "DEVKITPRO environment variable not set")
    endif()
  endif()
# endif()

project(WASMCarts C)

# ------------------------------------------------------------------------------
# wasm3 from $WASM3_SOURCE
# ------------------------------------------------------------------------------

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

# This collects a list of wasm3's .c files
file(GLOB WASM3_SOURCES "${WASM3_SRC}/*.c")

# ------------------------------------------------------------------------------
# Main executable
# ------------------------------------------------------------------------------

add_executable(WASMCarts "")
target_compile_options(WASMCarts PRIVATE -Wall)
target_compile_definitions(WASMCarts PRIVATE "$<$<CONFIG:Debug>:DEBUG>")

# Include + compile wasm3 directly into the executable
target_include_directories(WASMCarts PRIVATE "${WASM3_SRC}")
target_sources(WASMCarts PRIVATE ${WASM3_SOURCES})

# Set platform defines
if("${TARGET}" STREQUAL "linux")
  set(LINUX TRUE)
  target_compile_definitions(WASMCarts PRIVATE PLATFORM_LINUX)
elseif("${TARGET}" STREQUAL "nds")
  set(NINTENDO_DS TRUE)
  target_compile_definitions(WASMCarts PRIVATE PLATFORM_DS)
elseif("${TARGET}" STREQUAL "wii")
  set(NINTENDO_OGC TRUE)
  set(OGC_PLATFORM "wii")
  target_compile_definitions(WASMCarts PRIVATE PLATFORM_WII)
elseif("${TARGET}" STREQUAL "gamecube")
  set(NINTENDO_OGC TRUE)
  set(OGC_PLATFORM "gamecube")
  target_compile_definitions(WASMCarts PRIVATE PLATFORM_GAMECUBE)
else()
  message(FATAL_ERROR
    "Unknown TARGET value '${TARGET}'.\n"
    "Supported values: 'linux', 'nds', 'wii', 'gamecube'."
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
# Platform-specific sections
# ------------------------------------------------------------------------------

if(LINUX)

  # LINUX VERSION
  set_target_properties(WASMCarts PROPERTIES OUTPUT_NAME wasmcarts)

  target_sources(WASMCarts PRIVATE
    src/linux/platform.c
    src/linux/graphics.c
  )

  # wasm3 uses math on some platforms/configs
  target_link_libraries(WASMCarts PRIVATE m)

  # SDL3 via pkg-config
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
  target_link_libraries(WASMCarts PRIVATE PkgConfig::SDL3)

elseif(NINTENDO_DS)

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
    ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.bmp"
  )

elseif(NINTENDO_OGC)

  # NINTENDO WII / GAMECUBE VERSION

  if(NOT DEFINED ENV{DEVKITPRO})
    message(FATAL_ERROR
      "Ensure that environment variable DEVKITPRO is set.\n"
      "(see https://devkitpro.org/wiki/devkitPro_pacman)"
    )
  else()
    set(DEVKITPRO $ENV{DEVKITPRO})
  endif()

  if(NOT DEFINED ENV{DEVKITPPC})
    message(FATAL_ERROR
      "Ensure that environment variable DEVKITPPC is set.\n"
      "(see https://devkitpro.org/wiki/devkitPro_pacman)"
    )
  else()
    set(DEVKITPPC $ENV{DEVKITPPC})
  endif()

  target_include_directories(WASMCarts PRIVATE
    ${DEVKITPRO}/libogc/include
    ${DEVKITPRO}/portlibs/ppc/include
    ${DEVKITPRO}/portlibs/${OGC_PLATFORM}/include
  )
  target_link_directories(WASMCarts PRIVATE
    ${DEVKITPRO}/libogc/lib
    ${DEVKITPRO}/portlibs/ppc/lib
    ${DEVKITPRO}/portlibs/${OGC_PLATFORM}/lib
  )

  target_sources(WASMCarts PRIVATE
    src/gcwii/graphics.c
    src/gcwii/keys_${OGC_PLATFORM}.c
    src/gcwii/platform.c
  )

  if(OGC_PLATFORM STREQUAL "wii")
    target_link_libraries(WASMCarts PRIVATE
      grrlib freetype png jpeg z m wiiuse fat ogc
    )
  else()
    target_link_libraries(WASMCarts PRIVATE
      grrlib freetype png jpeg z m fat ogc
    )
  endif()

  ogc_create_dol("WASMCarts")
endif()

