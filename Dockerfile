FROM alpine:3.21.3@sha256:a8560b36e8b8210634f77d9f7f9efd7ffa463e380b75e2e74aff4511df3ef88c

RUN apk add --no-cache clang18 cmake git gtest-dev make

ENV CXX=clang++-18
ENV CC=clang-18

WORKDIR /build

# renovate: datasource=github-releases depName=open-telemetry/opentelemetry-cpp
ENV OPENTELEMETRY_CPP_VERSION=v1.19.0
RUN \
    git clone https://github.com/open-telemetry/opentelemetry-cpp.git \
        --depth=1 -b "${OPENTELEMETRY_CPP_VERSION}" --single-branch && \
    cd opentelemetry-cpp && \
    cmake \
        -B build \
        -DWITH_DEPRECATED_SDK_FACTORY=OFF -DWITH_NO_DEPRECATED_CODE=ON -DWITH_OTLP_HTTP=OFF \
        -DBUILD_TESTING=OFF -DWITH_BENCHMARK=OFF -DWITH_FUNC_TESTS=OFF -DWITH_EXAMPLES=OFF && \
    cmake --build build -j "$(nproc)" && \
    cmake --install build

WORKDIR /build/opentelemetry_resource_detectors
COPY . .
RUN cmake -B build && cmake --build build && cmake --install build && ctest --test-dir build
