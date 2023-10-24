#include "device.hpp"

#include <memory>

#define MODULE_NAME "device"
#include "../logging.hpp"

using namespace dim::hw;

device::device(size_t mem_size) {
	this->m_mem_size = mem_size;
	this->m_memory = new uint8_t[mem_size];
}

device::~device()
{
	delete[] this->m_memory;
}

void device::set_value8(size_t address, uint8_t value)
{
	if (address >= this->m_mem_size) {
		error("Device access out of bounds: %d (size: 1)", address);
		return;
	}
	*(uint8_t*)(this->m_memory + address) = value;
}

void device::set_value16(size_t address, uint16_t value)
{
	if (address + 1 >= this->m_mem_size) {
		error("Device access out of bounds: %d (size: 2)", address);
		return;
	}
	*(uint16_t*)(this->m_memory + address) = value;
}

void device::set_value32(size_t address, uint32_t value)
{
	if (address + 3 >= this->m_mem_size) {
		error("Device access out of bounds: %d (size: 4)", address);
		return;
	}
	*(uint32_t*)(this->m_memory + address) = value;
}

void device::set_value64(size_t address, uint64_t value)
{
	if (address + 7 >= this->m_mem_size) {
		error("Device access out of bounds: %d (size: 8)", address);
		return;
	}
	*(uint64_t*)(this->m_memory + address) = value;
}

uint8_t device::get_value8(size_t address)
{
	if (address >= this->m_mem_size) {
		error("Device access out of bounds: %d (size: 1)", address);
		return 0;
	}
	return *(uint8_t*)(this->m_memory + address);
}

uint16_t device::get_value16(size_t address)
{
	if (address + 1 >= this->m_mem_size) {
		error("Device access out of bounds: %d (size: 2)", address);
		return 0;
	}
	return *(uint16_t*)(this->m_memory + address);
}

uint32_t device::get_value32(size_t address)
{
	if (address + 3 >= this->m_mem_size) {
		error("Device access out of bounds: %d (size: 4)", address);
		return 0;
	}
	return *(uint32_t*)(this->m_memory + address);
}

uint64_t device::get_value64(size_t address)
{
	if (address + 7 >= this->m_mem_size) {
		error("Device access out of bounds: %d (size: 8)", address);
		return 0;
	}
	return *(uint64_t*)(this->m_memory + address);
}

void device::set_values(size_t address, size_t length, void* data)
{
	if (address + length > this->m_mem_size) {
		error("Device access out of bounds: %d (size: %d)", address, length);
		return;
	}
	std::memcpy(this->m_memory + address, data, length);
}

void device::get_values(size_t address, size_t length, void* data)
{
	if (address + length > this->m_mem_size) {
		error("Device access out of bounds: %d (size: %d)", address, length);
		return;
	}
	std::memcpy(data, this->m_memory, length);
}