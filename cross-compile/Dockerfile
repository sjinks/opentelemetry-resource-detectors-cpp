FROM --platform=${BUILDPLATFORM} tonistiigi/xx:master AS xx
FROM --platform=${BUILDPLATFORM} crazymax/osxcross:latest-ubuntu AS osxcross

FROM --platform=${BUILDPLATFORM} ubuntu:noble AS build
COPY --from=xx / /
COPY --from=osxcross / /
RUN ln -s /osxcross/SDK /xx-sdk

RUN \
    apt-get update && \
    apt-get install -y eatmydata && \
    eatmydata apt-get install -y --no-install-recommends \
        autoconf automake ca-certificates cmake curl file git jq less make ninja-build pkg-config unzip wget zip && \
    eatmydata apt-get install -y --no-install-recommends \
        clang clang-format clang-tidy clang-tools libclang-rt-dev lld llvm mingw-w64 && \
    rm -rf /var/lib/apt/lists/*

RUN \
    git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg && \
    cd /opt/vcpkg && \
    ./bootstrap-vcpkg.sh && \
    ln -s /opt/vcpkg/vcpkg /usr/local/bin/vcpkg

ENV VCPKG_ROOT=/opt/vcpkg

RUN \
    wget -q https://mirror.it.ubc.ca/ubuntu/pool/main/i/icu/libicu72_72.1-3ubuntu3_amd64.deb && \
    dpkg -i libicu72_72.1-3ubuntu3_amd64.deb && \
    wget -q https://github.com/PowerShell/PowerShell/releases/download/v7.4.5/powershell_7.4.5-1.deb_amd64.deb && \
    dpkg -i powershell_7.4.5-1.deb_amd64.deb && \
    rm powershell_7.4.5-1.deb_amd64.deb libicu72_72.1-3ubuntu3_amd64.deb

RUN \
    for i in linux/arm linux/arm64 linux/386 linux/amd64; do \
        TARGETPLATFORM="${i}" xx-clang --setup-target-triple; \
        TARGETPLATFORM="${i}" xx-apt-get install -y gcc g++ libc6-dev libstdc++-13-dev; \
    done

RUN \
    for i in darwin/arm64 darwin/amd64; do \
        TARGETPLATFORM="${i}" xx-clang --setup-target-triple; \
    done; \
    ln -s /osxcross/bin/x86_64-apple-darwin23.6-otool /usr/local/bin/otool; \
    ln -s /osxcross/bin/x86_64-apple-darwin23.6-install_name_tool /usr/local/bin/install_name_tool; \
    wget -q https://github.com/tonistiigi/xx/releases/download/prebuilt%2Fld-2.38-0/ld64-signed-linux-amd64.tar.gz -O ld64-signed-linux-amd64.tar.gz; \
    tar xzf ld64-signed-linux-amd64.tar.gz -C /usr/bin; \
    rm -f ld64-signed-linux-amd64.tar.gz

RUN \
    for i in windows/386 windows/amd64; do \
        TARGETPLATFORM="${i}" xx-clang --setup-target-triple; \
    done

RUN mkdir -m 0777 /src
WORKDIR /src
