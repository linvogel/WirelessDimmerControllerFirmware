#pragma once

#include <map>
#include <string>
#include <functional>
#include <vector>

namespace dim {
	namespace model {
		
		/**
		 * @brief The model_value class stores primitive values inside a model
		 * 
		 */
		class model_value {
			uint64_t m_value;
			void *m_model;
			std::string m_name;
			
		public:
			model_value();
			model_value(void *model, std::string name);
			~model_value();
			
			template<typename T>
			model_value& operator=(T new_value);
			
			template<typename T>
			operator T();
		};
		
		/**
		 * @brief The model class represents a single centralized and thread safe data-model
		 * to combine interfaces (e.g. GUIs) with controllers that may run in different threads.
		 * 
		 */
		class model {
			std::map<std::string, model_value> m_values;
			std::map<std::string, std::vector<std::function<void()>>> m_update_funcs;
			
		public:
			model();
			~model();
			
			void register_update_function(std::string &name, std::function<void()> func);
			
			model_value& operator[](const std::string &name);
		};
	}
}