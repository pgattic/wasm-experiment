
FROM archlinux:latest AS wasmcarts-dev

# ===============
# GENERAL SETUP
# ===============

RUN pacman -Syu --noconfirm && \
    pacman -Syu --noconfirm ca-certificates curl make cmake

# Download WASM3 source code
RUN set -e; \
    cd /opt; \
    curl -L -o wasm3.tar.gz https://github.com/wasm3/wasm3/archive/refs/tags/v0.5.0.tar.gz; \
    tar -xzvf wasm3.tar.gz; \
    rm wasm3.tar.gz

# ===================
# NINTENDO DS SETUP
# ===================

# Set locale for iconv, used by ndstool
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

RUN mkdir -p /opt/wonderful/
ADD https://wonderful.asie.pl/bootstrap/wf-bootstrap-x86_64.tar.gz /opt/wonderful/
ADD https://wonderful.asie.pl/bootstrap/wf-bootstrap-aarch64.tar.gz /opt/wonderful/

RUN cd /opt/wonderful/ && \
    tar xzvf wf-bootstrap-$(uname -m).tar.gz && \
    rm wf-bootstrap-*.tar.gz

ENV PATH=/opt/wonderful/bin:$PATH

# This is a workaround for pacman that generates the file /etc/mtab. Maybe there is a better way to fix it.
RUN cd etc && \
    ln -sf ../proc/self/mounts mtab

# Install SDK, then clear the package cache
RUN wf-pacman -Syu --noconfirm && \
    wf-pacman -Syu --noconfirm wf-tools && \
    wf-config repo enable blocksds && \
    wf-pacman -Syu --noconfirm && \
    wf-pacman -S --noconfirm blocksds-toolchain

# Set up the environment variables for a wf-pacman installation
ENV BLOCKSDS=/opt/wonderful/thirdparty/blocksds/core
ENV BLOCKSDSEXT=/opt/wonderful/thirdparty/blocksds/external
ENV WONDERFUL_TOOLCHAIN=/opt/wonderful

# Build WASM3 for NDS with Wonderful Toolchain
RUN set -e; \
    cd /opt/wasm3-0.5.0/source/; \
    $WONDERFUL_TOOLCHAIN/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-gcc -O3 -c *.c; \
    $WONDERFUL_TOOLCHAIN/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-ld -r -o libwasm3.a *.o; \
    mkdir -p $BLOCKSDSEXT/wasm3/lib; \
    mkdir -p $BLOCKSDSEXT/wasm3/include; \
    find . -name '*.h' -exec cp --parents {} $BLOCKSDSEXT/wasm3/include \;; \
    mv libwasm3.a $BLOCKSDSEXT/wasm3/lib/

# =============
# LINUX SETUP
# =============

# Install required packages
RUN pacman -Syu --noconfirm gcc pkg-config sdl3

# Build WASM3 for Linux
RUN set -e; \
    cd /opt/wasm3-0.5.0/source/; \
    gcc -O3 -c *.c; \
    ld -r -o libwasm3.a *.o; \
    mkdir -p /usr/local/lib; \
    mkdir -p /usr/local/include; \
    find . -name '*.h' -exec cp --parents {} /usr/local/include \;; \
    mv libwasm3.a /usr/local/lib/

# =====================
# NINTENDO WII SETUP
# =====================

# Install DevKitPro's Pacman
# RUN set -e; \
#     sh <(curl -sfL https://apt.devkitpro.org/install-devkitpro-pacman); \
#     dkp-pacman -S wii-dev

WORKDIR /work/

