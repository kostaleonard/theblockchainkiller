FROM alpine:latest
RUN mkdir /app
COPY . /app
WORKDIR /app
RUN apk update && \
    apk add build-base cmake clang clang-extra-tools gdb valgrind
RUN mkdir build
WORKDIR /app/build
RUN cmake .. && \
    clang-tidy --checks="{ BasedOnStyle: Microsoft, ColumnLimit: 80, SortIncludes: False }" ../src/* && \
    cmake --build .
WORKDIR /app
