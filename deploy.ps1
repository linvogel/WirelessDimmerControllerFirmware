
# Setup
Set-Variable -Name BUILD_DIR -Value .\bin

# sync to target
New-Item -Force -ItemType "directory" $BUILD_DIR
if ([System.IO.File]::Exists($BUILD_DIR)) { Remove-Item -Force $BUILD_DIR\source.tar }
tar.exe -c -f $BUILD_DIR\source.tar .\glfw .\src .\CMakeLists.txt
scp.exe $BUILD_DIR\source.tar dimmer:/firmware/source.tar
ssh dimmer rm -rf /firmware/bin
ssh dimmer mkdir -p /firmware/src
ssh dimmer mkdir -p /firmware/bin
ssh dimmer bsdtar -xvf source.tar -C src
ssh dimmer cmake -S src -B bin -DTARGET=raspi
ssh dimmer make -j 4 -C bin firmware