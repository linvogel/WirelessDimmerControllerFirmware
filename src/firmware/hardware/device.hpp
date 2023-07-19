#pragma once

#include <stdint.h>

namespace dim {
	namespace hw {
		
		class device {
			size_t m_mem_size;
			uint8_t *m_memory;
			
		public:
			device(size_t memory_size);
			~device() = default;
			
			virtual void set_value8(size_t address, uint8_t value);
			virtual void set_value16(size_t address, uint16_t value);
			virtual void set_value32(size_t address, uint32_t value);
			virtual void set_value64(size_t address, uint64_t value);
			virtual void set_values(size_t address, size_t length, void* data);
			virtual void get_values(size_t address, size_t length, void* data);
			
			virtual uint8_t get_value8(size_t address);
			virtual uint16_t get_value16(size_t address);
			virtual uint32_t get_value32(size_t address);
			virtual uint64_t get_value64(size_t address);
			
			virtual void sync() = 0;
		};
		
	}
}