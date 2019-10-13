# Get the base Ubuntu image from Docker Hub
FROM centos:latest

# Update apps on the base image
RUN yum -y update && yum clean all

# Install the build tools
RUN yum install -y clang cmake3 git make

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
