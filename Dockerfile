FROM alpine:3.19 as builder
COPY . /tmp/app
RUN apk update && \
    apk add build-base cmake gdb valgrind cmocka-dev openssl-dev

RUN mkdir /tmp/app/build && \
    cd /tmp/app/build && \
    cmake /tmp/app && \
    cmake --build /tmp/app/build

FROM alpine:3.19
COPY --from=builder /tmp/app/build/main /
CMD ["/main"]
