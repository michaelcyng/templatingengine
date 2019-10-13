# Get the base Alpine image from Docker Hub
FROM alpine:latest as build

# Install the build tools
RUN apk update && apk add --no-cache \
    build-base clang cmake git make

# Copy the source files
COPY ./ /opt/templatingengine/src

# Build the unit test binaries
RUN mkdir -p /opt/templatingengine/build
WORKDIR /opt/templatingengine/build
ENV CXX=clang++
ENV CC=clang
RUN cmake ../src && cmake --build . --target TemplatingEngineUnitTest -- -j 4

# Run the unit test
ENTRYPOINT ["/opt/templatingengine/build/bin/TemplatingEngineUnitTest"]
