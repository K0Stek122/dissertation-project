FROM ubuntu:latest

WORKDIR /docker

RUN apt-get update && apt-get install -y \
    g++ \
    libcpprest-dev \
    libboost-all-dev \
    libssl-dev \
    libpcap-dev \
    cmake

COPY src/* .
COPY CMakeLists.txt .

RUN cmake --build build/
CMD [ "./ids_dissertation" ]
EXPOSE 3000