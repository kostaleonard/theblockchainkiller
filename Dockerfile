FROM alpine:latest
RUN mkdir /app
COPY . /app
WORKDIR /app
RUN apk update && \
    apk add build-base cmake clang gdb valgrind
RUN mkdir build
WORKDIR /app/build
RUN cmake .. && \
    cmake --build .
WORKDIR /app
