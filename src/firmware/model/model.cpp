#include "model.hpp"

#define MODULE_NAME "model"
#include "../logging.hpp"

#include <vector>
#include <yaml-cpp/yaml.h>

using namespace dim::model;

model_value::model_value() : m_value(0ULL), m_model(nullptr), m_name(std::string()) {}
model_value::model_value(void *model, std::string name) : m_value(0ULL), m_model(model), m_name(name) {}

model_value::~model_value() {}

template<typename T>
model_value::operator T()
{
	return (T)this->m_value;
}

template<typename T>
model_value& model_value::operator=(T value)
{
	// assign value
	this->m_value = (uint64_t)value;
	// invoke update hooks
	for (std::function<void()> f : ((model*)this->m_model)->m_values[this->m_name].m_funcs) f();
	return *this;
}


model::model() {}

model::model(std::string filename)
{
	try {
		info("Loading model: %s", filename.c_str());
		std::vector<YAML::Node> all_nodes = YAML::LoadAllFromFile(filename);
		
		if (all_nodes.size() == 0)
			throw std::runtime_error("No data in model file");
		
		
		int k = -1;
		for (int i = 0; i < all_nodes.size(); i++) {
			if (all_nodes[i]["model"]) {
				if (k != -1) warn("Multiple model nodes, uncertain if choosing the correct one!");
				k = i;
			}
		}
		YAML::Node parent_node = all_nodes[k];
		
		if (!parent_node.IsMap())
			throw std::runtime_error("Malformed model: parent node must be of map type!");
		
		if (!parent_node["model"])
			throw std::runtime_error("Malformed model: root node not present!");
		
		YAML::Node value_map = parent_node["model"];
		std::cout << "Full data: " << std::endl << parent_node["model"] << std::endl;
		for (auto it = value_map.begin();  it != value_map.end(); ++it) {
			std::string value_name = it->first.as<std::string>();
			model_value(this, value_name);
		}
		
		
	} catch (YAML::BadFile e) {
		error("Failed to load module file: %s", filename.c_str());
	} catch (std::runtime_error e) {
		error("Error parsing model file: %s", filename.c_str());
		error(e.what());
	}
}

model::~model() {}

model_value& model::operator[](const std::string &name)
{
	if (this->m_values.find(name) == this->m_values.end()) {
		error("No such model value: %s", name.c_str());
		throw std::runtime_error("invalid model value");
	}
	return this->m_values[name];
}

void model::register_update_function(std::string &name, std::function<void()> func)
{
	if (this->m_values[name]) {
		this->m_values[name].m_funcs.push_back(func);
	}
}