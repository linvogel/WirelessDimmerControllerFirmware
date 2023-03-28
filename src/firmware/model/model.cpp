#include "model.hpp"

using namespace dim::model;


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
	for (std::function<void()> f : ((model*)(this->m_model))->m_update_funcs[this->m_name]) f();
	return *this;
}


model::model() {}
model::~model() {}

model_value& model::operator[](std::string &name)
{
	if (!this->m_values.contains(name)) this->m_values.insert(std::make_pair(name, model_value(this, name)));
	return this->m_values[name];
}

void model::register_update_function(std::string &name, std::function<void()> func)
{
	this->m_update_funcs[name].push_back(func);
}