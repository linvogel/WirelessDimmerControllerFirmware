

all:
	@g++ -o test -Iinclude -Isrc/firmware main.cpp src/firmware/logging.cpp