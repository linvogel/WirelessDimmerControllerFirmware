#pragma once

#include <yaml-cpp/yaml.h>

#include <string>

namespace dim {
	
	class cfg_value {
		YAML::Node m_value;
	public:
		cfg_value(YAML::Node value);
		
		template<typename T>
		operator T();
		
		template<typename T>
		cfg_value& operator=(T& value);
		
		template<typename T>
		T& operator[](std::string name);
		
		template<typename T>
		T& operator[](size_t idx);
	};
	
	class configuration {
		std::string m_filename;
		YAML::Node m_values;
		
	public:
		configuration(std::string filename);
		~configuration();
		
		template<typename T>
		cfg_value& operator()(std::string name, T val);
		cfg_value& operator[](std::string name);
	};
	
}