# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest

# Update apps on the base image
RUN apt-get -y update && apt-get install -y

RUN apt-get -y install apt-utils

# Install the build tools
RUN apt-get -y install clang cmake git

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
