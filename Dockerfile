# use docker build -t 'tag' . to build this container.
# then use docker images to list images
# docker run -d -p 128.0.0.1:3000:3000 tag_name to run the container
FROM ubuntu:latest

WORKDIR /docker

RUN apt-get update && apt-get install -y \
    g++ \
    libcpprest-dev \
    libboost-all-dev \
    libssl-dev \
    libpcap-dev \
    cmake
    
RUN rm -rf /var/lib/apt/lists/*

COPY src/ src/
COPY CMakeLists.txt .

RUN cmake -S . -B build \
&& cmake --build build --config Release

CMD [ "build/ids-dissertation" ]
EXPOSE 3000