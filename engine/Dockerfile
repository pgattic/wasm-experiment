
FROM archlinux:latest AS wasmcarts-dev

# ===============
# GENERAL SETUP
# ===============

RUN pacman -Syu --noconfirm && \
    pacman -Syu --noconfirm ca-certificates curl make cmake git

# =============
# LINUX SETUP
# =============

RUN pacman -Syu --noconfirm gcc pkg-config sdl3

# ===================
# NINTENDO DS SETUP
# ===================

# Set up BlocksDS (https://blocksds.skylyrac.net/docs/setup/options/linux/)

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

RUN wf-pacman -Syu --noconfirm && \
    wf-pacman -Syu --noconfirm wf-tools && \
    wf-config repo enable blocksds && \
    wf-pacman -Syu --noconfirm && \
    wf-pacman -S --noconfirm blocksds-toolchain

# Set up the environment variables for a wf-pacman installation
ENV BLOCKSDS=/opt/wonderful/thirdparty/blocksds/core
ENV BLOCKSDSEXT=/opt/wonderful/thirdparty/blocksds/external
ENV WONDERFUL_TOOLCHAIN=/opt/wonderful

# Set locale for iconv, used by ndstool
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

# =================
# DEVKITPRO SETUP
# =================

# Following https://devkitpro.org/wiki/devkitPro_pacman#Customising_Existing_Pacman_Install

ENV DEVKITPRO=/opt/devkitpro
ENV DEVKITARM=/opt/devkitpro/devkitARM
ENV DEVKITPPC=/opt/devkitpro/devkitPPC

# Set up DevKitPro repos on Pacman
RUN pacman-key --init && \
    pacman-key --populate archlinux
# RUN pacman-key --recv BC26F752D25B92CE272E0F44F7FD5492264BB9D0 --keyserver keys.openpgp.org # Broken
RUN curl -sS "https://keyserver.ubuntu.com/pks/lookup?op=get&search=0xbc26f752d25b92ce272e0f44f7fd5492264bb9d0" | pacman-key --add - && \
    pacman-key --lsign BC26F752D25B92CE272E0F44F7FD5492264BB9D0 && \
    pacman -U https://pkg.devkitpro.org/devkitpro-keyring.pkg.tar.zst --noconfirm && \
    pacman-key --populate devkitpro

# Add DevKitPro repositories to pacman
RUN printf "[dkp-libs]\nServer = https://pkg.devkitpro.org/packages\n\n" >> /etc/pacman.conf && \
    printf "[dkp-linux]\nServer = https://pkg.devkitpro.org/packages/linux/\$arch/\n\n" >> /etc/pacman.conf

# =============================
# NINTENDO WII/GAMECUBE SETUP
# =============================

# Update repo lists; Install Wii/GameCube development stuff
RUN pacman -Syu --noconfirm && \
    pacman -S wii-dev gamecube-dev --noconfirm

# GRRLIB requirements
RUN pacman --sync --needed --noconfirm libfat-ogc ppc-libpng ppc-freetype ppc-libjpeg-turbo

# Download and build GRRLIB
RUN set -e; \
    cd /opt; \
    curl -L -o GRRLIB.tar.gz https://github.com/GRRLIB/GRRLIB/archive/refs/tags/v4.6.1.tar.gz; \
    tar -xzvf GRRLIB.tar.gz; \
    rm GRRLIB.tar.gz; \
    cd GRRLIB-4.6.1; \
    # Places the headers in $DEVKITPRO/portlibs/wii/include/
    # Places the binaries in $DEVKITPRO/portlibs/wii/lib/
    make -C GRRLIB clean all install; \
    # Places the headers in $DEVKITPRO/portlibs/gamecube/include/
    # Places the binaries in $DEVKITPRO/portlibs/gamecube/lib/
    make PLATFORM=cube -C GRRLIB clean all install

WORKDIR /work/

