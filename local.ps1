param(
	[Parameter()][Switch]$clean,
	[Parameter()][Switch]$cmake
)

Set-Variable -Name BUILD_DIR -Value ./bin

if ($clean) {
	Remove-Item -Recurse -Path "$BUILD_DIR\*"
}

if ($clean -or $cmake) {
	cmake -S . -B $BUILD_DIR -DTARGET=local
}

MSBuild.exe -maxcpucount:8 $BUILD_DIR/glfw/src/glfw.vcxproj
MSBuild.exe -maxcpucount:8 $BUILD_DIR/firmware.vcxproj