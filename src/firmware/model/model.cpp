#include "model.hpp"

#define MODULE_NAME "model"
#include "../logging.hpp"

using namespace dim::model;



const bool is_valid_name(const std::string &name)
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
		off = end + 1;
	}
	
	return names;
}

model::model() : m_root("root", nullptr) {}

model::~model() {}

const bool model::table_walk(std::vector<std::string> &names, model_node **retval) const
{
	// Note the const_cast here, since nothing is actually changes here
	model_node *current = const_cast<model_node*>(&(this->m_root));
	
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

const bool model::table_walk(std::vector<std::string> &names, model_value **retval) const
{
	// Note the const_cast here, since nothing is actually changes here
	model_node *current = const_cast<model_node*>(&(this->m_root));
	
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
	
	if (current->m_values.find(names.back()) == current->m_values.end()) {
		*retval = nullptr;
		return false;
	} else {
		*retval = &(current->m_values.at(names.back()));
		return true;
	}
	
}

const bool model::contains_key(const std::string &name) const
{
	model_value *rptr;
	std::vector<std::string> names = ::separate(name);
	return this->table_walk(names, &rptr);
}


model_node::model_node(std::string name, model_node *parent)
{
	this->m_name = name;
	this->m_parent = parent;
	this->m_nodes.clear();
	this->m_values.clear();
}


bool model::add(const std::string &name, std::string value)
{
	std::vector<std::string> names = ::separate(name);
	model_node *current = &(this->m_root);
	for (size_t i = 0; i < names.size() - 1; i++) {
		if (current->m_nodes.find(names[i]) == current->m_nodes.end()) {
			current->m_nodes.insert(std::make_pair(names[i], model_node::model_node(names[i], current)));
		}
		current = &(current->m_nodes.at(names[i])); // this cannot simply be the [] operator, since this could call the default constructor
	}
	current->m_values[names.back()] = value;
	return true;
}



model_value& model::operator[](const std::string &name)
{
	model_node *node;
	std::vector<std::string> names = ::separate(name);
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
