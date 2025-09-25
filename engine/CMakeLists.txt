cmake_minimum_required(VERSION 3.5)

# Automatically select toolchain file if not already set
if(NOT DEFINED TARGET)
  message(FATAL_ERROR "No TARGET specified. Use `-DTARGET=` to specify one.\nSupported values: 'linux', 'nds', 'wii', 'gamecube'.")
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
  elseif("${TARGET}" STREQUAL "gamecube")
    if(DEFINED ENV{DEVKITPRO})
      set(CMAKE_TOOLCHAIN_FILE "$ENV{DEVKITPRO}/cmake/GameCube.cmake" CACHE STRING "Toolchain for GameCube" FORCE)
    else()
      message(FATAL_ERROR "DEVKITPRO environment variable not set")
    endif()
  endif()
endif()

# Actual project declaration
project(WASMCarts C)

include(CheckCCompilerFlag)
include(FetchContent)

option(WASM3_LTO "Try to enable LTO for wasm3" OFF)
set(WASM3_GIT_TAG "v0.5.0" CACHE STRING "wasm3 git tag or commit")

# Helper to add wasm3 to the build. Outputs: target + include dir
function(_wasm3_add_to_build out_target out_include_dir)
  FetchContent_Declare(
    wasm3
    GIT_REPOSITORY https://github.com/wasm3/wasm3.git
    GIT_TAG        ${WASM3_GIT_TAG}
  )
  # Skip unnecessary tests and other stuff
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  set(WASM3_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(WASM3_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

  FetchContent_MakeAvailable(wasm3)
  set(_wasm3_SOURCE_DIR "${wasm3_SOURCE_DIR}")

  # Might someday need to change this to "wasm3" instead of "m3"
  set(_wasm3_real_target m3)
  set(_wasm3_inc "${_wasm3_SOURCE_DIR}/source")

  # Optional LTO
  if(WASM3_LTO AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    check_c_compiler_flag(-flto HAS_FLTO)
    if(HAS_FLTO)
      target_compile_options(${_wasm3_real_target} PRIVATE -flto)
      target_link_options(${_wasm3_real_target} PRIVATE -flto)
    endif()
  endif()

  # PIE is fine across platforms and harmless on consoles
  set_target_properties(${_wasm3_real_target} PROPERTIES POSITION_INDEPENDENT_CODE ON)

  # Return real target + include dir
  set(${out_target} ${_wasm3_real_target} PARENT_SCOPE)
  set(${out_include_dir} "${_wasm3_inc}" PARENT_SCOPE)
endfunction()

add_executable(WASMCarts "")
target_compile_options(WASMCarts PRIVATE -Wall)
target_compile_definitions(WASMCarts PRIVATE "$<$<CONFIG:Debug>:DEBUG>") # Define DEBUG macro for CMake Debug config

# Set CMake variables, and preprocessor macros for the C code to use
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
  message(FATAL_ERROR "Unknown TARGET value '${TARGET}'.\nSupported values: 'linux', 'nds', 'wii', 'gamecube'.")
endif()

# General source files
target_sources(WASMCarts PRIVATE
  src/api.c
  src/f_sel.c
  src/game.c
  src/game_menu.c
  src/main.c
)

if(LINUX)

  # LINUX VERSION #

  set_target_properties(WASMCarts PROPERTIES OUTPUT_NAME wasmcarts) # Program name should be lowercased

  target_sources(WASMCarts PRIVATE
    src/linux/platform_impl.c
    src/linux/graphics.c
  )

  # Add wasm3 and link it
  set(BUILD_NATIVE OFF CACHE BOOL "" FORCE) # Don't assume native build
  set(BUILD_WASI none CACHE STRING "" FORCE) # Don't want WASM System Interface
  _wasm3_add_to_build(WASM3_TGT WASM3_INC)
  target_include_directories(WASMCarts PRIVATE ${WASM3_INC})
  target_link_libraries(WASMCarts PRIVATE ${WASM3_TGT} m)

  # SDL3 via pkg-config
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
  target_link_libraries(WASMCarts PRIVATE PkgConfig::SDL3)

elseif(NINTENDO_DS)

  # NINTENDO DS VERSION #

  # BlocksDS toolchain is already set above; just add your DS sources
  target_sources(WASMCarts PRIVATE
    src/nds/platform_impl.c
    src/nds/graphics.c
    src/nds/keys.c
  )

  # Add wasm3 and link it
  set(BUILD_NATIVE OFF CACHE BOOL "" FORCE) # Don't assume native build
  set(BUILD_WASI none CACHE STRING "" FORCE) # Don't want WASM System Interface
  _wasm3_add_to_build(WASM3_TGT WASM3_INC)
  target_include_directories(WASMCarts PRIVATE ${WASM3_INC})
  target_link_libraries(WASMCarts PRIVATE ${WASM3_TGT} m)

  nds_create_rom(WASMCarts
    NAME "WASMCarts"
    SUBTITLE "Lightweight Fantasy Console"
    AUTHOR "By pgattic"
    ICON "${CMAKE_CURRENT_SOURCE_DIR}/icon.bmp"
  )

elseif(NINTENDO_OGC)

  # NINTENDO WII/GAMECUBE VERSION #

  # devkitPro paths (libogc, portlibs) remain as before
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

  # Add wasm3 and link it
  set(BUILD_NATIVE OFF CACHE BOOL "" FORCE) # Don't assume native build
  set(BUILD_WASI none CACHE STRING "" FORCE) # Don't want WASM System Interface
  _wasm3_add_to_build(WASM3_TGT WASM3_INC)
  target_include_directories(WASMCarts PRIVATE ${WASM3_INC})

  if (OGC_PLATFORM STREQUAL "wii")
    target_link_libraries(WASMCarts PRIVATE
      ${WASM3_TGT}
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
  else()
    target_link_libraries(WASMCarts PRIVATE
      ${WASM3_TGT}
      grrlib
      freetype
      png
      jpeg
      z
      m
      fat
      ogc
    )
  endif()

  target_sources(WASMCarts PRIVATE
    src/gcwii/graphics.c
    src/gcwii/keys_${OGC_PLATFORM}.c
    src/gcwii/platform_impl.c
  )

  ogc_create_dol("WASMCarts")
endif()

