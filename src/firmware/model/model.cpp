#include "model.hpp"

#define MODULE_NAME "model"
#include "../logging.hpp"

using namespace dim::model;


model::model() : m_root("root", nullptr) {}



const bool model::is_valid_name(const std::string &name)
{
	auto is_ascii_lower = [](char c) { return c >= 'a' && c <= 'z'; };
	auto is_ascii_upper = [](char c) { return c >= 'A' && c <= 'Z'; };
	auto is_ascii_digit = [](char c) { return c >= '0' && c <= '9'; };
	auto is_ascii_sym = [](char c) { return c == '_' || c == '-' || c == '.'; };
	for (char c : name) {
		if (!(is_ascii_lower(c) || is_ascii_upper(c) || is_ascii_digit(c) || is_ascii_sym(c))) return false;
	}
	
	for (size_t i = 0; i < name.size()-1; i++) {
		if (name.at(i) == '.' && name.at(i) == '.') return false;
	}
	
	return true;
}

const std::vector<std::string> separate(const std::string &name)
{
	std::vector<std::string> names;
	
	size_t off = 0;
	size_t end = 0;
	while (off < name.size() && end < name.size()) {
		end = name.find_first_of('.', off);
		if (end == std::string::npos) {
			names.push_back(name.substr(off));
			break;
		}
		names.push_back(name.substr(off, end - off));
	}
	
	return names;
}

const bool model::table_walk(std::vector<std::string> &names, model_node **retval)
{
	model_node *current = &(this->m_root);
	
	for (size_t i = 0; i < names.size()-1; i++) {
		if (current->m_nodes.find(names[i]) != current->m_nodes.end()) {
			// this node exists as a child of current
			current = &(current->m_nodes.at(names[i]));
		} else {
			// this node does not exist as a child of current
			*retval = nullptr;
			return false;
		}
	}
	
	*retval = current;
	return true;
}


template<typename T>
bool model::add(const std::string name, T value)
{
	return true;
}



model_value& model::operator[](const std::string &name)
{
	model_node *node;
	std::vector<std::string> names = this->separate(name);
	bool success = this->table_walk(names, &node);
	
	if (!success) {
		error("Model tablewalk failed!");
		throw std::runtime_error("model tablewalk failed");
	}
	
	if (node->m_values.find(names.back()) == node->m_values.end()) {
		error("model does not contain leaf node: '%s'", names.back().c_str());
		throw std::runtime_error("model does not contain leaf node");
	}
	
	return node->m_values[names.back()];
}