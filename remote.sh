#!/bin/bash

# Setup
BUILD_DIR=./bin

# sync to target
mkdir -p "$BUILD_DIR"
if [[ -e "$BUILD_DIR/source.tar" ]]; then rm -f "$BUILD_DIR/source.tar"; fi
tar -c -f "$BUILD_DIR/source.tar" ./glfw ./src ./CMakeLists.txt ./shaders ./fonts ./include
ssh linvogel "rm -rf ~/workspace/wireless_firmware_remote/*"
ssh linvogel "mkdir -p ~/workspace/wireless_firmware_remote/src"
ssh linvogel "mkdir -p ~/workspace/wireless_firmware_remote/bin"
ssh linvogel "mkdir -p ~/workspace/wireless_firmware_remote/shaders"
ssh linvogel "mkdir -p ~/workspace/wireless_firmware_remote/fonts"
ssh linvogel "mkdir -p ~/workspace/wireless_firmware_remote/include"
scp "$BUILD_DIR/source.tar" linvogel:~/workspace/wireless_firmware_remote/source.tar
ssh linvogel "bsdtar -xvf ~/workspace/wireless_firmware_remote/source.tar -C ~/workspace/wireless_firmware_remote/src"
ssh linvogel "cp -r ~/workspace/wireless_firmware_remote/src/shaders/* ~/workspace/wireless_firmware_remote/shaders/"
ssh linvogel "cp -r ~/workspace/wireless_firmware_remote/src/fonts/* ~/workspace/wireless_firmware_remote/fonts/"
ssh linvogel "cp -r ~/workspace/wireless_firmware_remote/src/include/* ~/workspace/wireless_firmware_remote/include/"
ssh linvogel "cmake -S ~/workspace/wireless_firmware_remote/src -B ~/workspace/wireless_firmware_remote/bin -DTARGET=server"
ssh linvogel "make -j 8 -C ~/workspace/wireless_firmware_remote/bin firmware && cp ~/workspace/wireless_firmware_remote/bin/firmware ~/workspace/wireless_firmware_remote/firmware"
scp -r linvogel:~/workspace/wireless_firmware_remote/bin/firmware "$BUILD_DIR"/firmware

