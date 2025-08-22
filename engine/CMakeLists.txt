cmake_minimum_required(VERSION 3.16)

# Automatically select toolchain file if not already set
if(NOT DEFINED TARGET)
  message(FATAL_ERROR "No TARGET specified. Use `-DTARGET=` to specify one.\nSupported values: 'nds', 'linux', 'wii'.")
endif()

# Automatically configure toolchain file based on TARGET
if(NOT CMAKE_TOOLCHAIN_FILE)
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
  endif()
endif()

# Actual project declaration
project(WASMCarts C)

add_executable(WASMCarts "")

# Validate and set up per-target
if("${TARGET}" STREQUAL "nds")
  set(NINTENDO_DS TRUE)
  target_compile_definitions(WASMCarts PRIVATE NINTENDO_DS)
elseif("${TARGET}" STREQUAL "linux")
  set(LINUX TRUE)
  target_compile_definitions(WASMCarts PRIVATE LINUX)
elseif("${TARGET}" STREQUAL "wii")
  set(NINTENDO_WII TRUE)
  target_compile_definitions(WASMCarts PRIVATE NINTENDO_WII)
else()
  message(FATAL_ERROR "Unknown TARGET value '${TARGET}'.\nSupported values: 'nds', 'linux', 'wii'.")
endif()

# General source files
target_sources(WASMCarts PRIVATE
  src/api.c
  src/f_sel.c
  src/game.c
  src/game_menu.c
  src/main.c
)

if(NINTENDO_DS)
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
  if(NOT EXISTS ${BLOCKSDSEXT}/wasm3/include OR NOT EXISTS ${BLOCKSDSEXT}/wasm3/lib/libwasm3.a)
    message(FATAL_ERROR "WASM3 not compiled and installed to the correct location.\n(see Dockerfile, search for \"Build WASM3 for NDS\")")
  endif()

  target_include_directories(WASMCarts PRIVATE ${BLOCKSDSEXT}/wasm3/include)
  target_link_directories(WASMCarts PRIVATE ${BLOCKSDSEXT}/wasm3/lib)
  target_link_libraries(WASMCarts PRIVATE wasm3)

  target_sources(WASMCarts PRIVATE
    src/nds/platform_impl.c
    src/nds/graphics.c
    src/nds/keys.c
  )

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

  target_include_directories(WASMCarts PRIVATE /usr/local/include)
  target_link_directories(WASMCarts PRIVATE /usr/local/lib)
  target_link_libraries(WASMCarts PRIVATE wasm3 m)

  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
  target_link_libraries(WASMCarts PRIVATE PkgConfig::SDL3)
elseif(NINTENDO_WII)
  if(NOT DEFINED ENV{DEVKITPRO})
    message(FATAL_ERROR "Ensure that environment variable DEVKITPRO is set.\n(see https://devkitpro.org/wiki/devkitPro_pacman)")
  else()
    set(DEVKITPRO $ENV{DEVKITPRO})
  endif()
  if(NOT DEFINED ENV{DEVKITPPC})
    message(FATAL_ERROR "Ensure that environment variable DEVKITPPC is set.\n(see https://devkitpro.org/wiki/devkitPro_pacman)")
  else()
    set(DEVKITPPC $ENV{DEVKITPPC})
  endif()

  # Includes
  target_include_directories(WASMCarts PRIVATE
    ${DEVKITPPC}/local/wasm3/include
    ${DEVKITPRO}/libogc/include
    ${DEVKITPRO}/portlibs/ppc/include     # GRRLIB + deps (common)
    ${DEVKITPRO}/portlibs/wii/include     # some setups place headers here
  )
  # Libraries (search paths)
  target_link_directories(WASMCarts PRIVATE
    ${DEVKITPPC}/local/wasm3/lib
    ${DEVKITPRO}/libogc/lib
    ${DEVKITPRO}/portlibs/ppc/lib         # GRRLIB + deps (common)
    ${DEVKITPRO}/portlibs/wii/lib         # some setups place libs here
  )

  # Link order: keep grrlib before its deps
  target_link_libraries(WASMCarts PRIVATE
    wasm3
    grrlib
    freetype
    png
    jpeg
    z
    m
    wiiuse
    fat
    ogc
  )

  target_sources(WASMCarts PRIVATE
    src/wii/graphics.c
    src/wii/keys.c
    src/wii/platform_impl.c
  )

  target_compile_features(WASMCarts PRIVATE c_std_11)
  target_compile_definitions(WASMCarts PRIVATE __WII__)

  ogc_create_dol("WASMCarts")
endif()

