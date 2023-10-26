#pragma once

#include <map>
#include <string>
#include <vector>

namespace dim {
	namespace model {
		class model;
		
		union u_value {
			uint8_t     uint8_value;
			uint16_t    uint16_value;
			uint32_t    uint32_value;
			uint64_t    uint64_value;
			int8_t      int8_value;
			int16_t     int16_value;
			int32_t     int32_value;
			int64_t     int64_value;
			float       float_value;
			double      double_value;
			std::string string_value;
			
		public:
			u_value() { this->string_value = std::string(""); }
			~u_value() {}
		};
		
		/**
		 * @brief The model_value class stores primitive values inside a model
		 * 
		 */
		class model_value {
			friend model;
			std::string m_name;
			union u_value m_value;
			
		public:
			model_value() = default;
			model_value(std::string name);
			~model_value() {}
			
			uint8_t     operator=(uint8_t value)     { return this->m_value.uint8_value = value; }
			uint16_t    operator=(uint16_t value)    { return this->m_value.uint16_value = value; }
			uint32_t    operator=(uint32_t value)    { return this->m_value.uint32_value = value; }
			uint64_t    operator=(uint64_t value)    { return this->m_value.uint64_value = value; }
			int8_t      operator=(int8_t value)      { return this->m_value.int8_value = value; }
			int16_t     operator=(int16_t value)     { return this->m_value.int16_value = value; }
			int32_t     operator=(int32_t value)     { return this->m_value.int32_value = value; }
			int64_t     operator=(int64_t value)     { return this->m_value.int64_value = value; }
			float       operator=(float value)       { return this->m_value.float_value = value; }
			double      operator=(double value)      { return this->m_value.double_value = value; }
			std::string operator=(std::string value) { return this->m_value.string_value = value; }
			
			operator uint8_t()     { return this->m_value.uint8_value; }
			operator uint16_t()    { return this->m_value.uint16_value; }
			operator uint32_t()    { return this->m_value.uint32_value; }
			operator uint64_t()    { return this->m_value.uint64_value; }
			operator int8_t()      { return this->m_value.int8_value; }
			operator int16_t()     { return this->m_value.int16_value; }
			operator int32_t()     { return this->m_value.int32_value; }
			operator int64_t()     { return this->m_value.int64_value; }
			operator float()       { return this->m_value.float_value; }
			operator double()      { return this->m_value.double_value; }
			operator std::string() { return this->m_value.string_value; }
		};
		
		/**
		 * @brief The model node class describes the structure of a model.
		 * it contains a list of named sub nodes and a list of named model values.
		 */
		class model_node {
			friend model;
			model_node *m_parent;
			std::string m_name;
			std::map<std::string, model_value> m_values;
			std::map<std::string, model_node> m_nodes;
		
		public:
			model_node(std::string name, model_node *parent);
		};
		
		/**
		 * @brief The model class represents a single centralized and thread safe data-model
		 * to combine interfaces (e.g. GUIs) with controllers that may run in different threads.
		 * 
		 */
		class model {
			model_node m_root;
			
		public:
			model();
			~model();
			
			bool add(const std::string &name, std::string value);
			model_value& operator[](const std::string &name);
			
			const bool contains_key(const std::string &name) const;
		
		private:
			const bool table_walk(std::vector<std::string> &names, model_node **retval) const;
			const bool table_walk(std::vector<std::string> &names, model_value **retval) const;
		};
	}
}