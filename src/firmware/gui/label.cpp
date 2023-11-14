#include "label.hpp"

using namespace dim::gui;
using namespace dim::math;

label::label(dim::model::model &model, std::string value_name, renderer &renderer, float x, float y, float w, float h, float font_size)
	: component(x, y, 1, 1, 0, w, h)
	, m_model(model)
	, m_value_name(value_name)
	, m_font_size(font_size)
{
	ftrace();
}

void label::set_value_name(const std::string &value_name)
{
	ftrace();
	this->m_value_name = value_name;
}

void label::draw_component(renderer &renderer)
{
	ftrace();
	dim::model::model_value &mval = this->m_model[this->m_value_name];
	char tmp[64] = {0};
	std::string sval;
	if (mval.is_str()) {
		sval = static_cast<std::string>(mval);
	} else if (mval.is_double()) {
		sprintf(tmp, "%.2f", static_cast<double>(mval));
		sval.assign(tmp);
	} else if (mval.is_signed()) {
		sprintf(tmp, "%lld", static_cast<int64_t>(mval));
		sval.assign(tmp);
	} else {
		sprintf(tmp, "%llu", static_cast<uint64_t>(mval));
		sval.assign(tmp);
	}
	
	renderer.draw_text_centered(sval, this->m_size(0)*0.5f, this->m_size(1)*0.5f, this->m_font_size);
}
