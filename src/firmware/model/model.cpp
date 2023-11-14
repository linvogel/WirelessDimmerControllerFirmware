#include "model.hpp"

#include <iostream>
#include <fstream>

#define MODULE_NAME "model"
#include "../logging.hpp"

using namespace dim::model;



const bool is_valid_name(const std::string &name)
{
	ftrace();
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
	ftrace();
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

model::model() : m_root("root", nullptr)
{
	ftrace();
	this->add("tmp.dummy_string", "dummy_string");
	this->add("tmp.dummy_double", 0.0);
	this->add("tmp.dummy_signed", static_cast<int64_t>(0));
	this->add("tmp.dummy_unsigned", static_cast<uint64_t>(0));
}

model::~model() {ftrace();}

const bool model::table_walk(std::vector<std::string> &names, model_node **retval) const
{
	ftrace();
	// Note the const_cast here, since nothing is actually changes here
	model_node *current = const_cast<model_node*>(&(this->m_root));
	
	if (names.size() != 0) {
		for (size_t i = 0; i < names.size() - 1; i++) {
			if (current->m_nodes.find(names[i]) != current->m_nodes.end()) {
				// this node exists as a child of current
				current = &(current->m_nodes.at(names[i]));
			}
			else {
				// this node does not exist as a child of current
				*retval = nullptr;
				return false;
			}
		}
	}
	
	*retval = current;
	return true;
}

const bool model::table_walk(std::vector<std::string> &names, model_value **retval) const
{
	ftrace();
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
	ftrace();
	model_value *rptr;
	std::vector<std::string> names = ::separate(name);
	return this->table_walk(names, &rptr);
}

const bool model::contains_key_partial(const std::string &name) const
{
	ftrace();
	model_node *rptr;
	std::vector<std::string> names = ::separate(name);
	return this->table_walk(names, &rptr);
}


model_node::model_node(std::string name, model_node *parent)
{
	ftrace();
	this->m_name = name;
	this->m_parent = parent;
	this->m_nodes.clear();
	this->m_values.clear();
}

template<typename T>
bool model::add_generic(const std::string &name, T value)
{
	ftrace();
	if (this->contains_key(name)) {
		(*this)[name] = value;
		return true;
	}
	std::vector<std::string> names = ::separate(name);
	model_node *current = &(this->m_root);
	for (size_t i = 0; i < names.size() - 1; i++) {
		if (current->m_nodes.find(names[i]) == current->m_nodes.end()) {
			current->m_nodes.insert(std::make_pair(names[i], model_node::model_node(names[i], current)));
		}
		current = &(current->m_nodes.at(names[i])); // this cannot simply be the [] operator, since this could call the default constructor
	}
	std::string mv_name = names.back();
	model_value mv;
	current->m_values.insert(std::make_pair(mv_name, mv));
	current->m_values[names.back()] = value;
	return true;
}


bool model::add(const std::string &name, std::string value)	{ ftrace(); return this->add_generic(name, value); }
bool model::add(const std::string &name, uint8_t value)		{ ftrace(); return this->add_generic(name, value); }
bool model::add(const std::string &name, uint16_t value)	{ ftrace(); return this->add_generic(name, value); }
bool model::add(const std::string &name, uint32_t value)	{ ftrace(); return this->add_generic(name, value); }
bool model::add(const std::string &name, uint64_t value)	{ ftrace(); return this->add_generic(name, value); }
bool model::add(const std::string &name, int8_t value)		{ ftrace(); return this->add_generic(name, value); }
bool model::add(const std::string &name, int16_t value)		{ ftrace(); return this->add_generic(name, value); }
bool model::add(const std::string &name, int32_t value)		{ ftrace(); return this->add_generic(name, value); }
bool model::add(const std::string &name, int64_t value)		{ ftrace(); return this->add_generic(name, value); }
bool model::add(const std::string &name, float value)		{ ftrace(); return this->add_generic(name, value); }
bool model::add(const std::string &name, double value)		{ ftrace(); return this->add_generic(name, value); }


model_value::model_value(const model_value &other)
{
	ftrace();
	this->m_double_val = other.m_double_val;
	this->m_sign_val = other.m_sign_val;
	this->m_unsign_val = other.m_unsign_val;
	this->m_str_val = other.m_str_val;
	this->m_is_str = other.m_is_str;
	this->m_is_double = other.m_is_double;
	this->m_is_signed = other.m_is_signed;
}

model_value::model_value(model_value &&other)
{
	ftrace();
	this->m_double_val = other.m_double_val;
	this->m_sign_val = other.m_sign_val;
	this->m_unsign_val = other.m_unsign_val;
	this->m_str_val = other.m_str_val;
	this->m_is_str = other.m_is_str;
	this->m_is_double = other.m_is_double;
	this->m_is_signed = other.m_is_signed;
}

model_value& model_value::operator=(const model_value &other)
{
	ftrace();
	this->m_double_val = other.m_double_val;
	this->m_sign_val = other.m_sign_val;
	this->m_unsign_val = other.m_unsign_val;
	this->m_str_val = other.m_str_val;
	this->m_is_str = other.m_is_str;
	this->m_is_double = other.m_is_double;
	this->m_is_signed = other.m_is_signed;
	return *this;
}

model_value& model_value::operator=(model_value &&other)
{
	ftrace();
	this->m_double_val = other.m_double_val;
	this->m_sign_val = other.m_sign_val;
	this->m_unsign_val = other.m_unsign_val;
	this->m_str_val = other.m_str_val;
	this->m_is_str = other.m_is_str;
	this->m_is_double = other.m_is_double;
	this->m_is_signed = other.m_is_signed;
	return *this;
}


model_value& model::operator[](const std::string &name)
{
	ftrace();
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


void model::load_category(const std::string &name, const std::string &filename)
{
	ftrace();
	verbose("Loading model category: '%s'", name.c_str());
	std::ifstream input_stream(filename);
	std::string line;
	
	// read file line by line
	// every line is an entry in the model
	while (std::getline(input_stream, line)) {
		verbose("Reading line: '%s'", line.c_str());
		if (line.size() == 0) {
			verbose("Line was empty");
			continue;
		}
		size_t offset = line.find('=', 0);
		std::string path = name + "." + line.substr(0, offset);
		verbose("Found path: '%s'", path.c_str());
		
		// skip whitespace
		while (++offset < line.size() && line.at(offset) == ' ' || line.at(offset) == '\t');
		verbose("Skipped whitespace");
		
		// check for empty value
		if (offset >= line.size()) {
			verbose("Value was empty");
			this->add(path, 0);
			continue;
		}
		
		// check for string
		if (line.at(offset) == '"') {
			verbose("Found string value");
			// this is a string value
			offset++;
			size_t end = offset;
			size_t escaped = false;
			while (end < line.size()) {
				if (!escaped && line.at(end) == '"') break;
				else if (line.at(end) == '\\') escaped = true;
				else escaped = false;
				end++;
			}
			std::string string_value;
			for (size_t i = offset; i < end; i++) {
				if (line.at(i) == '\\' && line.at(i+1) == '\"') {
					string_value += '\"';
					i++;
				} else if (line.at(i) == '\\') {
					fatal("model file contains invalid escape sequence!");
					input_stream.close();
					throw std::runtime_error("model file contains invalid escape sequence!");
				} else {
					string_value += line.at(i);
				}
			}
			
			verbose("Read string value: '%s'", string_value.c_str());
			this->add(path, string_value);
			verbose("Added value to model");
		} else {
			// this must be a number
			if (line.find('.', offset) != std::string::npos || line.find('e', offset) != std::string::npos || line.find('E', offset) != std::string::npos) {
				// this is a float number
				double value = std::strtod(line.c_str() + offset, nullptr);
				this->add(path, value);
			} else if (line.at(offset) == '-') {
				// this is a negative integer
				int64_t value = std::strtoll(line.c_str() + offset, nullptr, 10);
				this->add(path, value);
			} else {
				// this is a positive intger
				uint64_t value = std::strtoull(line.c_str() + offset, nullptr, 10);
				this->add(path, value);
			}
		}
	}
	
	input_stream.close();
}

void model_node::print_model_node(std::ostream &ostr, const std::string &prefix)
{
	ftrace();
	// first print all the values
	for (std::pair<const std::string, model_value> kv : this->m_values) {
		if (prefix.size() != 0) ostr << prefix << ".";
		ostr << kv.first << "=";
		if (kv.second.is_str()) {
			// print string value
			ostr << '"' << static_cast<std::string>(kv.second) << '"';
		} else if (kv.second.is_double()) {
			// print double value
			ostr << static_cast<double>(kv.second);
		} else if (kv.second.is_signed()) {
			// print signed value
			ostr << static_cast<int64_t>(kv.second);
		} else {
			// print unsigned value
			ostr << static_cast<uint64_t>(kv.second);
		}
		ostr << '\n';
	}
	ostr << '\n';
	
	// then recurse into subnodes
	for (std::pair<std::string, model_node> kv : this->m_nodes) {
		if (prefix.size() != 0) kv.second.print_model_node(ostr, prefix + '.' + kv.first);
		else kv.second.print_model_node(ostr, kv.first);
	}
}

bool model::save_category(const std::string &name, const std::string &filename)
{
	ftrace();
	std::ofstream output_stream(filename);
	
	const std::map<std::string, model_node> &categories = this->m_root.m_nodes;
	std::map<std::string, model_node>::const_iterator it = categories.find(name);
	if (it == categories.cend()) {
		// this category does not exist!
		error("Unable to save model category '%s'! No such category!", name.c_str());
		return false;
	}
	
	dim::model::model_node *cat_node = const_cast<dim::model::model_node*>(&(it->second));
	cat_node->print_model_node(output_stream, "");
	output_stream.flush();
	output_stream.close();
	return true;
}