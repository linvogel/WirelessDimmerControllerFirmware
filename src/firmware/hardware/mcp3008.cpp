#include "mcp3008.hpp"

#define MODULE_NAME "MCP3008"
#include <logging.hpp>

using namespace dim::hw;

mcp3008::mcp3008() : device(16ULL) {}

void mcp3008::sync()
{
	ftrace();
	// this device is read only
	// TODO: implement sync over SPI
	
}