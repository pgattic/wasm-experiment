
FROM ubuntu:24.04 AS wasmcarts-dev

ARG BRANCH=master

# Set TARGETARCH e.g. amd64, arm64, riscv64.
ARG TARGETARCH

# Set locale for iconv, used by ndstool
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
        ca-certificates curl git make


RUN mkdir -p /opt/wonderful/
ADD https://wonderful.asie.pl/bootstrap/wf-bootstrap-x86_64.tar.gz /opt/wonderful/
ADD https://wonderful.asie.pl/bootstrap/wf-bootstrap-aarch64.tar.gz /opt/wonderful/
# Note that the downloads are cached; no impact if it's a multi-platform build.
# Needed because Docker's TARGETARCH doesn't match Linux's `uname -m` format.

RUN cd /opt/wonderful/ && \
    tar xzvf wf-bootstrap-$(uname -m).tar.gz && \
    rm wf-bootstrap-*.tar.gz

ENV PATH=/opt/wonderful/bin:$PATH

# TODO: This is a workaround for pacman that generates the file /etc/mtab. Maybe
# there is a better way to fix it.
RUN cd etc && \
    ln -sf ../proc/self/mounts mtab

# Install SDK, then clear the package cache
RUN wf-pacman -Syu --noconfirm && \
    wf-pacman -Syu --noconfirm wf-tools && \
    wf-config repo enable blocksds && \
    wf-pacman -Syu --noconfirm && \
    wf-pacman -S --noconfirm blocksds-toolchain

# Setup the environment variables for a wf-pacman installation
ENV BLOCKSDS=/opt/wonderful/thirdparty/blocksds/core
ENV BLOCKSDSEXT=/opt/wonderful/thirdparty/blocksds/external
ENV WONDERFUL_TOOLCHAIN=/opt/wonderful

# Build and install wasm3 under BlocksDS toolchain using /tmp as workspace
RUN set -e; \
    cd /tmp; \
    curl -L -o v0.5.0.tar.gz https://github.com/wasm3/wasm3/archive/refs/tags/v0.5.0.tar.gz; \
    tar -xzvf v0.5.0.tar.gz; \
    mkdir -p /tmp/release/include /tmp/release/lib; \
    cd /tmp/wasm3-0.5.0/source/; \
    echo "Compiling wasm3 sources"; \
    /opt/wonderful/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-gcc -O3 -c *.c; \
    /opt/wonderful/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-ld -r -o libwasm3.a *.o; \
    find . -name '*.h' -exec cp --parents {} /tmp/release/include \;; \
    cp libwasm3.a /tmp/release/lib; \
    echo "Installing wasm3 into $BLOCKSDSEXT/wasm3"; \
    mkdir -p $BLOCKSDSEXT/wasm3/include $BLOCKSDSEXT/wasm3/lib; \
    cp -r /tmp/release/include/* $BLOCKSDSEXT/wasm3/include/; \
    cp /tmp/release/lib/libwasm3.a $BLOCKSDSEXT/wasm3/lib/

RUN wf-pacman -Sy --noconfirm blocksds-ulibrary

# Use /work/ as working directory of this container.
WORKDIR /work/

