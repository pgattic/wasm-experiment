
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

# =================
# DEVKITPRO SETUP
# =================

ENV DEVKITPRO=/opt/devkitpro
ENV DEVKITARM=/opt/devkitpro/devkitARM
ENV DEVKITPPC=/opt/devkitpro/devkitPPC

# Set up DevKitPro repos on Pacman
RUN pacman-key --init && \
    pacman-key --recv BC26F752D25B92CE272E0F44F7FD5492264BB9D0 --keyserver keyserver.ubuntu.com && \
    pacman-key --lsign BC26F752D25B92CE272E0F44F7FD5492264BB9D0 && \
    pacman -U https://pkg.devkitpro.org/devkitpro-keyring.pkg.tar.zst --noconfirm && \
    pacman-key --populate devkitpro

# Add DevKitPro repositories to pacman
RUN printf "[dkp-libs]\nServer = https://pkg.devkitpro.org/packages\n\n" >> /etc/pacman.conf && \
    printf "[dkp-linux]\nServer = https://pkg.devkitpro.org/packages/linux/\$arch/\n\n" >> /etc/pacman.conf

# =====================
# NINTENDO WII SETUP
# =====================

# Update repo lists; Install Wii development stuff
RUN pacman -Syu --noconfirm && \
    pacman -S wii-dev --noconfirm

# GRRLIB requirements
RUN pacman --sync --needed --noconfirm libfat-ogc ppc-libpng ppc-freetype ppc-libjpeg-turbo

# Download and build GRRLIB
RUN set -e; \
    cd /opt; \
    curl -L -o GRRLIB.tar.gz https://github.com/GRRLIB/GRRLIB/archive/refs/tags/v4.6.1.tar.gz; \
    tar -xzvf GRRLIB.tar.gz; \
    rm GRRLIB.tar.gz; \
    cd GRRLIB-4.6.1; \
    make -C GRRLIB clean all install

# Build WASM3 for PowerPC-EABI architecture
RUN set -e; \
    cd /opt/wasm3-0.5.0/source/; \
    $DEVKITPPC/bin/powerpc-eabi-gcc -O3 -c *.c; \
    $DEVKITPPC/bin/powerpc-eabi-ld -r -o libwasm3.a *.o; \
    mkdir -p $DEVKITPPC/local/wasm3/lib; \
    mkdir -p $DEVKITPPC/local/wasm3/include; \
    find . -name '*.h' -exec cp --parents {} $DEVKITPPC/local/wasm3/include \;; \
    mv libwasm3.a $DEVKITPPC/local/wasm3/lib/

WORKDIR /work/

