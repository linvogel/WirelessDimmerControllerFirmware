#include "configuration.hpp"

#define MODULE_NAME "config"
#include "../logging.hpp"

using namespace dim;

cfg_value::cfg_value(YAML::Node value)
{
	this->m_value = value;
}

template<typename T>
cfg_value::operator T()
{
	if (!this->m_value.IsScalar()) {
		error("cannot convert YAML node to scalar value");
		return T();
	}
	
	return this->m_value.as<T>();
}

template<typename T>
T& cfg_value::operator[](std::string name)
{
	if (!this->m_value.IsMap()) {
		error("YAML node is not a map");
		return T();
	}
	
	return this->m_value[name].as<T>();
}

template<typename T>
T& cfg_value::operator[](size_t idx)
{
	if (!this->m_value.IsSequence()) {
		error("YAML node is not a sequence");
		return T();
	}
	
	return this->m_value[idx];
}

template<typename T>
cfg_value& cfg_value::operator=(T& value)
{
	this->m_value.Assign(value);
	return *this;
}