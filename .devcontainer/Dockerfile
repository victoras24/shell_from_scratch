ARG REGISTRY=mcr
ARG HOST=microsoft
FROM ${REGISTRY}.${HOST}.com/devcontainers/base:ubuntu-24.04

ENV DEBIAN_FRONTEND=noninteractive

# 1. Toolchain + man pages
#    (man pages restored directly instead of via `unminimize`, which failed the build)
RUN rm -f /etc/dpkg/dpkg.cfg.d/excludes \
    && if [ -f /usr/bin/man.REAL ]; then \
         rm -f /usr/bin/man && dpkg-divert --quiet --remove --rename /usr/bin/man; \
       fi \
    && apt-get update \
    && apt-get -y install --no-install-recommends \
        man-db \
        manpages \
        manpages-dev \
        manpages-posix-dev \
        build-essential \
        cmake \
        ninja-build \
        lldb \
        clangd \
        clang-format \
        libreadline-dev \
        pkg-config \
        zip \
        unzip \
        tar \
    && update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-18 100 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# 2. vcpkg — provides the toolchain file your_program.sh expects at
#    ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
#    Full clone (not shallow) so a manifest "builtin-baseline" commit can be resolved.
ENV VCPKG_ROOT=/usr/local/vcpkg
# Required on arm64 Linux (Apple Silicon): use the system cmake/ninja
ENV VCPKG_FORCE_SYSTEM_BINARIES=1

RUN git clone https://github.com/microsoft/vcpkg.git "$VCPKG_ROOT" \
    && "$VCPKG_ROOT/bootstrap-vcpkg.sh" -disableMetrics \
    && chown -R vscode:vscode "$VCPKG_ROOT"

ENV PATH="${VCPKG_ROOT}:${PATH}"