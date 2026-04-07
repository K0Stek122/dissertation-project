FROM ubuntu:24.04

WORKDIR /app

RUN apt-get update && apt-get install -y \
    g++ \
    libpcap-dev \
    libsqlite3-dev \
    libyaml-cpp-dev \
    cmake \
    git \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

COPY src/ src/
COPY CMakeLists.txt .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --config Release

RUN mkdir -p /var/log/regspy /etc/regspy

COPY config.yaml /etc/regspy/config.yaml

CMD ["build/ids-dissertation", "--config", "/etc/regspy/config.yaml"]
