#pragma once

#include <map>
#include <string>
#include <vector>

namespace dim {
	namespace model {
		class model;
		
		/**
		 * @brief The model_value class stores primitive values inside a model
		 * 
		 */
		class model_value {
			friend model;
			std::string m_str_val;
			double m_double_val;
			int64_t m_sign_val;
			uint64_t m_unsign_val;
			bool m_is_str;
			bool m_is_double;
			bool m_is_signed;
			
			
		public:
			model_value(): m_is_str(false), m_is_double(false), m_is_signed(false), m_double_val(0), m_sign_val(0), m_unsign_val(0) {};
			model_value(const model_value &other);
			model_value(model_value &&other);
			~model_value() = default;
			
			bool is_str() const { return this->m_is_str; }
			bool is_double() const { return this->m_is_double; }
			bool is_signed() const { return this->m_is_signed; }
			
			model_value& operator=(const model_value &other);
			model_value& operator=(model_value &&other);
			
			uint8_t     operator=(uint8_t value)     { this->m_is_str = false; this->m_is_double = false; this->m_is_signed = false; return static_cast<uint8_t>(this->m_unsign_val = value); }
			uint16_t    operator=(uint16_t value)    { this->m_is_str = false; this->m_is_double = false; this->m_is_signed = false; return static_cast<uint16_t>(this->m_unsign_val = value); }
			uint32_t    operator=(uint32_t value)    { this->m_is_str = false; this->m_is_double = false; this->m_is_signed = false; return static_cast<uint32_t>(this->m_unsign_val = value); }
			uint64_t    operator=(uint64_t value)    { this->m_is_str = false; this->m_is_double = false; this->m_is_signed = false; return static_cast<uint64_t>(this->m_unsign_val = value); }
			int8_t      operator=(int8_t value)      { this->m_is_str = false; this->m_is_double = false; this->m_is_signed = true; return static_cast<int8_t>(this->m_sign_val = value); }
			int16_t     operator=(int16_t value)     { this->m_is_str = false; this->m_is_double = false; this->m_is_signed = true; return static_cast<int16_t>(this->m_sign_val = value); }
			int32_t     operator=(int32_t value)     { this->m_is_str = false; this->m_is_double = false; this->m_is_signed = true; return static_cast<int32_t>(this->m_sign_val = value); }
			int64_t     operator=(int64_t value)     { this->m_is_str = false; this->m_is_double = false; this->m_is_signed = true; return static_cast<int64_t>(this->m_sign_val = value); }
			float       operator=(float value)       { this->m_is_str = false; this->m_is_double = true; this->m_is_signed = false; return static_cast<float>(this->m_double_val = value); }
			double      operator=(double value)      { this->m_is_str = false; this->m_is_double = true; this->m_is_signed = false; return static_cast<double>(this->m_double_val = value); }
			std::string operator=(std::string value) { this->m_is_str = true; this->m_is_double = false; this->m_is_signed = false; return this->m_str_val = value; }
			
			operator uint8_t()     { if (this->m_is_double || this->m_is_str || this->m_is_signed) return 0; return static_cast<uint8_t>(this->m_unsign_val); }
			operator uint16_t()    { if (this->m_is_double || this->m_is_str || this->m_is_signed) return 0; return static_cast<uint16_t>(this->m_unsign_val); }
			operator uint32_t()    { if (this->m_is_double || this->m_is_str || this->m_is_signed) return 0; return static_cast<uint32_t>(this->m_unsign_val); }
			operator uint64_t()    { if (this->m_is_double || this->m_is_str || this->m_is_signed) return 0; return static_cast<uint64_t>(this->m_unsign_val); }
			operator int8_t()      { if (!this->m_is_signed) return 0; return static_cast<int8_t>(this->m_sign_val); }
			operator int16_t()     { if (!this->m_is_signed) return 0; return static_cast<int16_t>(this->m_sign_val); }
			operator int32_t()     { if (!this->m_is_signed) return 0; return static_cast<int32_t>(this->m_sign_val); }
			operator int64_t()     { if (!this->m_is_signed) return 0; return static_cast<int64_t>(this->m_sign_val); }
			operator float()       { if (!this->m_is_double) return 0; return static_cast<float>(this->m_double_val); }
			operator double()      { if (!this->m_is_double) return 0; return static_cast<double>(this->m_double_val); }
			operator std::string() { if (!this->m_is_str) return std::string(); return this->m_str_val; }
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
			
		private:
			void print_model_node(std::ostream &ostr, const std::string &prefix);
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
			
			void load_category(const std::string &name, const std::string &filename);
			bool save_category(const std::string &name, const std::string &filename);
			
			bool add(const std::string &name, std::string value);
			bool add(const std::string &name, uint8_t value);
			bool add(const std::string &name, uint16_t value);
			bool add(const std::string &name, uint32_t value);
			bool add(const std::string &name, uint64_t value);
			bool add(const std::string &name, int8_t value);
			bool add(const std::string &name, int16_t value);
			bool add(const std::string &name, int32_t value);
			bool add(const std::string &name, int64_t value);
			bool add(const std::string &name, float value);
			bool add(const std::string &name, double value);
			model_value& operator[](const std::string &name);
			
			const bool contains_key(const std::string &name) const;
			const bool contains_key_partial(const std::string &name) const;
		
		private:
			template<typename T>
			bool add_generic(const std::string &name, T value);
			const bool table_walk(std::vector<std::string> &names, model_node **retval) const;
			const bool table_walk(std::vector<std::string> &names, model_value **retval) const;
		};
	}
}