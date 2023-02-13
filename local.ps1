param(
	[Parameter()][Switch]$clean
)

Set-Variable -Name BUILD_DIR -Value ./bin

if ($clean) {
	Remove-Item -Recurse $BUILD_DIR
	cmake -S . -B $BUILD_DIR -DTARGET=local
}

MSBuild.exe -maxcpucount:8 $BUILD_DIR/glfw/src/glfw.vcxproj
MSBuild.exe -maxcpucount:8 $BUILD_DIR/firmware.vcxproj