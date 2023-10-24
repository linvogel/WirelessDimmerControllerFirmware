#pragma once

#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <exception>

#ifdef MODULE_NAME
#pragma message("MODULE_NAME defined as: " MODULE_NAME)
#endif
#define MODULE_NAME "custom_array"
#include "../logging.hpp"

namespace dim {
	
	template<typename T>
	class custom_array {
		struct custom_array_container {
			size_t ref_count;
			size_t size;
			T* ptr;
		};
		
		custom_array_container *m_arr;
		
	public:
		custom_array()
		{
			this->m_arr = (custom_array_container*)std::malloc(sizeof(custom_array_container));
			assert(this->m_arr != nullptr);
			this->m_arr->ptr = 0;
			this->m_arr->ref_count = 1;
			this->m_arr->size = 0;
		}

		custom_array(size_t size)
			: custom_array()
		{
			this->m_arr->ptr = (T*)std::malloc(size * sizeof(T));
			assert(this->m_arr->ptr != nullptr);
			this->m_arr->size = size;
		}
		
		custom_array(custom_array &carr)
		{
			this->m_arr = carr.m_arr;
			if (this->m_arr == nullptr) {
				fatal("copy constructed destructed custom_array!");
				exit(1);
			}
			this->m_arr->ref_count++;
		}
		
		custom_array(custom_array &&carr)
		{
			this->m_arr = carr.m_arr;
			if (this->m_arr == nullptr) {
				fatal("copy constructed destructed custom_array!");
				exit(1);
			}
			carr.m_arr->ref_count++;
		}

		~custom_array()
		{
			if (this->m_arr == nullptr) {
				fatal("Attempted to destruct an already destructed custom_array!");
				exit(1);
			}
			
			this->m_arr->ref_count--;
			if (this->m_arr->ref_count < 1) {
				if (this->m_arr->ptr != nullptr) {
					std::free(this->m_arr->ptr);
					this->m_arr->ptr = nullptr;
				}
				
				std::free(this->m_arr);
			}
			this->m_arr = nullptr;
		}
		
		void resize(size_t new_size)
		{
			// distinguish cases where this is a first allocation whether a reallocation
			if (this->m_arr->ptr == nullptr) {
				this->m_arr->ptr = (T*)std::malloc(new_size * sizeof(T));
				this->m_arr->size = new_size;
				this->m_arr->ref_count = 1;
			} else {
				assert(this->m_arr->ptr != nullptr);
				assert(this->m_arr->ref_count != 0);
				T* new_ptr = (T*)std::realloc(this->m_arr->ptr, new_size * sizeof(T));
				if (new_ptr == nullptr) {
					fatal("Failed to reallocate buffer");
					throw std::bad_alloc();
				}
				this->m_arr->ptr = new_ptr;
				this->m_arr->size = new_size;
			}
		}
		
		constexpr custom_array<T>& operator=(custom_array<T> &carr)
		{
			if (this == &carr) return *this;
			this->~custom_array();
			this->m_arr = carr.m_arr;
			if (this->m_arr == nullptr) {
				fatal("Copy assigned uninitialized custom_array");
				exit(1);
			}
			this->m_arr->ref_count++;
			
			return *this;
		}
		
		constexpr custom_array<T>& operator=(const custom_array<T> &&carr)
		{
			this->~custom_array();
			this->m_arr = carr.m_arr;
			if (this->m_arr == nullptr) {
				fatal("Move assigned uninitialized custom_array");
				exit(1);
			}
			
			carr.m_arr->ref_count++;
			return *this;
		}
		
		T* get()
		{
			if (this->m_arr == nullptr) return nullptr;
			return this->m_arr->ptr;
		}

		size_t size()
		{
			return this->m_arr->size;
		}
		
		T& operator[](size_t i)
		{
			if (i >= this->m_arr->size) {
				fatal("Memory access violation detected (index: %lu of %lu)!", i, this->m_arr->size);
				throw std::domain_error("Memory access out of bounds error!");
			}
			return this->m_arr->ptr[i];
		}
		
	};
	
}

#undef MODULE_NAME