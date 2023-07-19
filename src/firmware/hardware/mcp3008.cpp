#include "mcp3008.hpp"

using namespace dim::hw;

mcp3008::mcp3008() : device(16ULL) {}

void mcp3008::sync()
{
	// this device is read only
	// TODO: implement sync over SPI
	
}