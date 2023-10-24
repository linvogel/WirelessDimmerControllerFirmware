#include "configuration.hpp"

#include <fstream>

#define MODULE_NAME "config"
#include "../logging.hpp"

using namespace dim;


configuration::configuration(std::string filename)
{
	this->m_filename = filename;
	try {
		this->m_values = YAML::LoadFile(filename);
	} catch (const std::exception e) {
		warn("Configuration file '%s' could not be loaded!", filename.c_str());
		this->m_values = YAML::Node();
	}
}

configuration::~configuration()
{
	std::ofstream out(this->m_filename);
	out << this->m_values;
	out.flush();
	out.close();
}

template<typename T>
T configuration::operator()(std::string name, T value)
{
	trace("CONFIG LOOKUP: %s", name.c_str());
	// walk through the path
	YAML::Node current = this->m_values;
	std::string cname = name;
	size_t found = std::string::npos;
	while ((found = cname.find('.')) != std::string::npos) {
		trace("CONFIG LOOKUP ITER: %s <==> %s", cname.c_str(), cname.substr(0, found));
		if (!current[cname.substr(0, found)]) {
			current[cname.substr(0, found)] = YAML::Node();
		}
		current = current[cname.substr(0, found)];
		cname = cname.substr(found + 1, cname.size() - found - 1);
	}
	trace("CONFIG LOOKUP FINAL: %s", cname.c_str());
	if (current[cname]) return current[cname].as<T>(); // Note: this semicolon is correct, even if the msvc compiler doesn't seem to care about its absence???
	
	current[cname] = value;
	return value;
}