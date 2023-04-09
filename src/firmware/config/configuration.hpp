#pragma once

#include <yaml-cpp/yaml.h>

#include <string>

namespace dim {
	
	class configuration {
		std::string m_filename;
		YAML::Node m_values;
		
	public:
		configuration(std::string filename);
		~configuration();
		
		template<typename T>
		T operator()(std::string name, T val);
		
		template<typename T>
		T operator[](std::string name);
	};
	
}