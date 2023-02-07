
param(
	[Parameter()][Switch]$cmake
)

if ($cmake) {
	# first clear the build directory
	Remove-Item -Recurse -Force bin
	
	# regenerate project file
	cmake -B bin
}

# build project
msbuild -maxcpucount:8 bin/firmware.vcxproj

# run generated executable
bin/Debug/firmware.exe