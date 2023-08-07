FROM ubuntu:latest
RUN mkdir /app
COPY . /app
WORKDIR /app
RUN apt update && \
    apt install -y gcc gdb cmake valgrind
RUN mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build .
