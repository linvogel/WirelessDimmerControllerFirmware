FROM ubuntu:latest

# prepare environment
RUN mkdir /workspace
WORKDIR /workspace
VOLUME [ "/workspace" ]

# install necessary packages
RUN apt -y update
RUN apt-get -y install gcc-arm-linux-gnueabihf
RUN apt-get -y install g++-arm-linux-gnueabihf
RUN apt-get -y install binutils-arm-linux-gnueabihf
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install pkg-config-arm-linux-gnueabihf
RUN apt-get -y install make
RUN apt-get -y install cmake
RUN apt-get -y install xorg-dev
