#include "onscreen_keyboard.hpp"

#include <gui/label.hpp>
#include <gui/button.hpp>
#include <gui/panel.hpp>

#define MODULE_NAME "onscreen_kbd"
#include "logging.hpp"

using namespace dim::gui;

#define LOWER 0
#define UPPER 1
#define SYMBOL 2

onscreen_keyboard::onscreen_keyboard(window *window, model::model *model)
{
	debug("Creating onscreen keyboard...");
	this->m_window = window;
	renderer &renderer = this->m_window->get_renderer();
	this->m_scene = this->m_window->create_scene();
	this->m_components = std::vector<component*>(128);
	
	// create the two content panes for letters and symbols
	trace("Creating keyboard panels...");
	this->m_keys[0][0] = new panel(renderer, 0, 80, 800, 400);
	this->m_keys[0][1] = new panel(renderer, 0, 80, 800, 400);
	this->m_keys[1][0] = new panel(renderer, 0, 80, 800, 400);
	this->m_keys[1][1] = this->m_keys[1][0];
	
	// setup buffer
	this->m_buffer = std::string();
	this->m_buffer.reserve(256);
	this->m_buffer.clear();
	
	// setup model information
	this->m_key_counter = 0;
	this->m_model = model;
	if (!this->m_model->contains_key("language")) {
		fatal("Model does not contain language section!");
		throw std::runtime_error("Model does not contain a language section!");
	}
	
	trace("Creating keys...");
	// buttons for lower letters
	this->add_simple_key(LOWER, "q", 0.0f, 0);
	this->add_simple_key(LOWER, "w", 1.0f, 0);
	this->add_simple_key(LOWER, "e", 2.0f, 0);
	this->add_simple_key(LOWER, "r", 3.0f, 0);
	this->add_simple_key(LOWER, "t", 4.0f, 0);
	this->add_simple_key(LOWER, "y", 5.0f, 0);
	this->add_simple_key(LOWER, "u", 6.0f, 0);
	this->add_simple_key(LOWER, "i", 7.0f, 0);
	this->add_simple_key(LOWER, "o", 8.0f, 0);
	this->add_simple_key(LOWER, "p", 9.0f, 0);
	this->add_key(LOWER, "Backspace", [this]() { this->backspace(); }, 10, 0, 1);
	
	this->add_simple_key(LOWER, "a", 0.0f, 1);
	this->add_simple_key(LOWER, "s", 1.0f, 1);
	this->add_simple_key(LOWER, "d", 2.0f, 1);
	this->add_simple_key(LOWER, "f", 3.0f, 1);
	this->add_simple_key(LOWER, "g", 4.0f, 1);
	this->add_simple_key(LOWER, "h", 5.0f, 1);
	this->add_simple_key(LOWER, "j", 6.0f, 1);
	this->add_simple_key(LOWER, "k", 7.0f, 1);
	this->add_simple_key(LOWER, "l", 8.0f, 1);
	this->add_key(LOWER, "Enter", [this]() { this->enter(); } , 9, 1, 1.5);
	
	this->add_key(LOWER, "Shift", [this]() { this->shift(); }, 0.0f, 2, 1.0f);
	this->add_simple_key(LOWER, "z", 1.0f, 2);
	this->add_simple_key(LOWER, "x", 2.0f, 2);
	this->add_simple_key(LOWER, "c", 3.0f, 2);
	this->add_simple_key(LOWER, "v", 4.0f, 2);
	this->add_simple_key(LOWER, "b", 5.0f, 2);
	this->add_simple_key(LOWER, "n", 6.0f, 2);
	this->add_simple_key(LOWER, "m", 7.0f, 2);
	this->add_simple_key(LOWER, ",", 8.0f, 2);
	this->add_simple_key(LOWER,	".", 9.0f, 2);
	this->add_key(LOWER, "Shift", [this]() { this->shift(); }, 10.0f, 2, 1.0f);
	
	this->add_key(LOWER, "Syms", [this]() { this->change(); }, 0.0f, 3, 1.5f);
	this->add_key(LOWER, "", [this]() { this->enter_char(" "); }, 1.5f, 3, 6.9f);
	this->add_key(LOWER, "Syms", [this]() { this->change(); }, 8.0f, 3, 1.5f);
	this->add_key(LOWER, "Cancel", [this]() { this->cancel(); }, 9.5f, 3, 1.5f);
	
	// buttons for upper letters
	this->add_simple_key(UPPER, "Q", 0.0f, 0);
	this->add_simple_key(UPPER, "W", 1.0f, 0);
	this->add_simple_key(UPPER, "E", 2.0f, 0);
	this->add_simple_key(UPPER, "R", 3.0f, 0);
	this->add_simple_key(UPPER, "T", 4.0f, 0);
	this->add_simple_key(UPPER, "Y", 5.0f, 0);
	this->add_simple_key(UPPER, "U", 6.0f, 0);
	this->add_simple_key(UPPER, "I", 7.0f, 0);
	this->add_simple_key(UPPER, "O", 8.0f, 0);
	this->add_simple_key(UPPER, "P", 9.0f, 0);
	this->add_key(UPPER, "Backspace", [this]() { this->backspace(); }, 10.0f, 0, 1.0f);
	
	this->add_simple_key(UPPER, "A", 0.0f, 1);
	this->add_simple_key(UPPER, "S", 1.0f, 1);
	this->add_simple_key(UPPER, "D", 2.0f, 1);
	this->add_simple_key(UPPER, "F", 3.0f, 1);
	this->add_simple_key(UPPER, "G", 4.0f, 1);
	this->add_simple_key(UPPER, "H", 5.0f, 1);
	this->add_simple_key(UPPER, "J", 6.0f, 1);
	this->add_simple_key(UPPER, "K", 7.0f, 1);
	this->add_simple_key(UPPER, "L", 8.0f, 1);
	this->add_key(UPPER, "Enter", [this]() { this->enter(); } , 9.0f, 1, 1.5f);
	
	this->add_key(UPPER, "Shift", [this]() { this->shift(); }, 0.0f, 2, 1.0f);
	this->add_simple_key(UPPER, "Z", 1.0f, 2);
	this->add_simple_key(UPPER, "X", 2.0f, 2);
	this->add_simple_key(UPPER, "C", 3.0f, 2);
	this->add_simple_key(UPPER, "V", 4.0f, 2);
	this->add_simple_key(UPPER, "B", 5.0f, 2);
	this->add_simple_key(UPPER, "N", 6.0f, 2);
	this->add_simple_key(UPPER, "M", 7.0f, 2);
	this->add_simple_key(UPPER, "!", 8.0f, 2);
	this->add_simple_key(UPPER,	"?", 9.0f, 2);
	this->add_key(UPPER, "Shift", [this]() { this->shift(); }, 10.0f, 2, 1.0f);
	
	this->add_key(UPPER, "Syms", [this]() { this->change(); }, 0.0f, 3, 1.5f);
	this->add_key(UPPER, "", [this]() { this->enter_char(" "); }, 1.5f, 3, 6.9f);
	this->add_key(UPPER, "Syms", [this]() { this->change(); }, 8.0f, 3, 1.5f);
	this->add_key(UPPER, "Cancel", [this]() { this->cancel(); }, 9.5f, 3, 1.5f);
	
	// buttons for symbols
	this->add_simple_key(SYMBOL, "1", 0.0f, 0);
	this->add_simple_key(SYMBOL, "2", 1.0f, 0);
	this->add_simple_key(SYMBOL, "3", 2.0f, 0);
	this->add_simple_key(SYMBOL, "4", 3.0f, 0);
	this->add_simple_key(SYMBOL, "5", 4.0f, 0);
	this->add_simple_key(SYMBOL, "6", 5.0f, 0);
	this->add_simple_key(SYMBOL, "7", 6.0f, 0);
	this->add_simple_key(SYMBOL, "8", 7.0f, 0);
	this->add_simple_key(SYMBOL, "9", 8.0f, 0);
	this->add_simple_key(SYMBOL, "0", 9.0f, 0);
	this->add_key(SYMBOL, "Backspace", [this]() { this->backspace(); }, 10.0f, 0, 1.0f);
	
	this->add_simple_key(SYMBOL, "-", 0.0f, 1);
	this->add_simple_key(SYMBOL, "/", 1.0f, 1);
	this->add_simple_key(SYMBOL, ":", 2.0f, 1);
	this->add_simple_key(SYMBOL, ";", 3.0f, 1);
	this->add_simple_key(SYMBOL, "(", 4.0f, 1);
	this->add_simple_key(SYMBOL, ")", 5.0f, 1);
	this->add_simple_key(SYMBOL, "$", 6.0f, 1);
	this->add_simple_key(SYMBOL, "&", 7.0f, 1);
	this->add_simple_key(SYMBOL, "@", 8.0f, 1);
	this->add_key(SYMBOL, "Enter", [this]() { this->enter(); } , 9.0f, 1, 1.5f);
	
	this->add_key(SYMBOL, "Shift", [this]() { this->shift(); }, 0.0f, 2, 1.0f);
	this->add_simple_key(SYMBOL, ".",  1.0f, 2);
	this->add_simple_key(SYMBOL, ",",  2.0f, 2);
	this->add_simple_key(SYMBOL, "?",  3.0f, 2);
	this->add_simple_key(SYMBOL, "!",  4.0f, 2);
	this->add_simple_key(SYMBOL, "'",  5.0f, 2);
	this->add_simple_key(SYMBOL, "\"" ,6.0f, 2);
	this->add_key(SYMBOL, "Shift", [this]() { this->shift(); }, 10.0f, 2, 1.0f);
	
	this->add_key(SYMBOL, "Syms", [this]() { this->change(); }, 0.0f, 3, 1.5f);
	this->add_key(SYMBOL, "Space", [this]() { this->enter_char(" "); }, 1.5f, 3, 6.9f);
	this->add_key(SYMBOL, "Syms", [this]() { this->change(); }, 8.0f, 3, 1.5f);
	this->add_key(SYMBOL, "Cancel", [this]() { this->cancel(); }, 9.5f, 3, 1.5f);
	
	// build complete scene
	trace("Completing onscreen keyboard...");
	this->m_window->set_scene(this->m_scene);
	this->m_case = 0;
	this->m_cat = 0;
	this->m_text_field = new label(renderer, 5, 5, 720, 70, this->m_buffer, 32);
	this->m_clear = new button(&((*this->m_model)["language.okbd.btn_clear"]), [this]() { this->clear(); }, this->m_window->get_renderer(), 730, 10, 60, 60);
	this->m_window->add(this->m_text_field);
	this->m_window->add(this->m_keys[this->m_cat][this->m_case]);
	this->m_window->add(this->m_clear);
	
	debug("Onscreen keyboard ready.");
}

onscreen_keyboard::~onscreen_keyboard()
{
}

std::function<void()> onscreen_keyboard::simple_key_func(const char* c)
{
	return [this, c]() { this->enter_char(c); };
}

void onscreen_keyboard::add_key(uint32_t type, std::string text, std::function<void()> func, float col, uint32_t row, float width)
{
	char tmp[64];
	snprintf(tmp, 64, "language.okbd.btn_%s", text.c_str());
	std::string btn_name(tmp);
	trace("debugging: checking for key in model...");
	if (!this->m_model->contains_key(btn_name)) {
	trace("debugging: key not found, adding it...");
		this->m_model->add(btn_name, text);
	}
	trace("debugging: key available");
	model::model_value &mval = (*this->m_model)[btn_name];
	button *btn = new button(&mval, func, this->m_window->get_renderer(), 5 + ((row == 1) * 32) + 72 * col, (float)(5 + 80 * row), 67 * width, 67);
	this->m_components.push_back(btn);
	this->m_keys[(type & 0x02) >> 1][type & 0x01]->add(btn);
}

void onscreen_keyboard::add_simple_key(uint32_t type, const char *text, float col, uint32_t row)
{
	this->add_key(type, text, this->simple_key_func(text), col, row, 1);
}

void onscreen_keyboard::shift()
{
	trace("Onscreen Keyboard: shift");
	this->m_window->remove_child(this->m_keys[0][0]);
	this->m_window->remove_child(this->m_keys[0][1]);
	this->m_window->remove_child(this->m_keys[1][0]);
	this->m_case = (1 - this->m_case) & 0x01;
	this->m_window->add(this->m_keys[this->m_cat][this->m_case]);
	this->m_window->draw(this->m_window->get_renderer());
}

void onscreen_keyboard::change()
{
	trace("Onscreen Keyboard: change");
	this->m_window->remove_child(this->m_keys[0][0]);
	this->m_window->remove_child(this->m_keys[0][1]);
	this->m_window->remove_child(this->m_keys[1][0]);
	this->m_cat = (1 - this->m_cat) & 0x01;
	this->m_window->add(this->m_keys[this->m_cat][this->m_case]);
	this->m_window->draw(this->m_window->get_renderer());
}


void onscreen_keyboard::backspace () {
	trace("Onscreen Keyboard: backspace");
	if (this->m_cursor == 0) return;
	this->m_buffer.erase(--this->m_cursor);
	this->m_text_field->set_text(this->m_buffer);
}

void onscreen_keyboard::enter_char(const char* c)
{
	trace("Onscreen Keyboard: key (%c)", c[0]);
	this->m_buffer.insert(this->m_cursor++, c);
	this->m_text_field->set_text(this->m_buffer);
	
	// if this was a capital char, release shift
	if (this->m_case && !this->m_cat) this->shift();
}

void onscreen_keyboard::enter()
{
	trace("Onscreen Keyboard: enter");
	*this->m_value = this->m_buffer;
	this->clear();
	this->m_value = nullptr;
	this->m_window->pop_scene();
}

void onscreen_keyboard::cancel()
{
	trace("Onscreen Keyboard: cancel");
	this->clear();
	this->m_value = nullptr;
	this->m_window->pop_scene();
}

void onscreen_keyboard::clear()
{
	this->m_buffer.clear();
	this->m_text_field->set_text(this->m_buffer);
	this->m_cursor = 0;
}

void onscreen_keyboard::show(const std::string &value_name)
{
	debug("Showing onscreen keyboard...");
	this->m_value = &((*this->m_model)[value_name]);
	this->m_buffer.clear();
	std::string tmp = *this->m_value;
	this->m_buffer.assign(tmp);
	this->m_cursor = (uint32_t)this->m_buffer.size();
	this->m_text_field->set_text(this->m_buffer);
	if (this->m_case) this->shift();
	if (this->m_cat) this->change();
	this->m_window->push_scene(this->m_scene);
}