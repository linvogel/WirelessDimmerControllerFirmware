#include "keyboard_view.hpp"

#include <gui/label.hpp>
#include <gui/button.hpp>
#include <gui/panel.hpp>


using namespace dim::gui;

#define ADD_KEY_TEXT(type, name, text, parent, index, offset, y) \
button *btn_##type##_##name = new button(text, [&] { this->m_buffer.append(text, 1); this->m_text_field->set_text(this->m_buffer); }, renderer, 5 + offset + index*72, y, 67, 67);\
parent->add(btn_##type##_##name);

#define ADD_KEY(type, name, parent, index, offset, y) ADD_KEY_TEXT(type, name, #name, parent, index, offset, y)

#define ADD_BACKSPACE(type, parent) \
button *btn_##type##_backspace = new button("<-", [&] { this->m_buffer.pop_back(); this->m_text_field->set_text(this->m_buffer); }, renderer, 725, 80, 67, 67);\
parent->add(btn_##type##_backspace);

#define ADD_SHIFT(type, parent, target) \
button *btn_##type##_lshift = new button("^", [&] { this->m_window->remove_child(parent); this->m_window->add(target); }, renderer, 5, 240, 67, 67);\
parent->add(btn_##type##_lshift);\
button *btn_##type##_rshift = new button("^", [&] { this->m_window->remove_child(parent); this->m_window->add(target); }, renderer, 725, 240, 67, 67);\
parent->add(btn_##type##_rshift);

#define ADD_ENTER(type, parent) \
button *btn_##type##_enter = new button("Enter", [&] { /* TODO: apply change and update model */ }, renderer, 685, 160, 108, 67);\
parent->add(btn_##type##_enter);

#define ADD_CHANGE(type, parent, target, text) \
button *btn_##type##_lchange = new button(text, [&] { this->m_window->remove_child(parent); this->m_window->add(target); }, renderer, 5, 320, 108, 67);\
parent->add(btn_##type##_lchange);\
button *btn_##type##_rchange = new button(text, [&] { this->m_window->remove_child(parent); this->m_window->add(target); }, renderer, 581, 320, 108, 67);\
parent->add(btn_##type##_rchange);

#define ADD_SPACE(type, parent) \
button *btn_##type##_space = new button("", [&] { this->m_buffer.append(" ", 1); this->m_text_field->set_text(this->m_buffer); }, renderer, 113, 320, 458, 67);\
parent->add(btn_##type##_space);

#define ADD_CANCEL(type, parent) \
button *btn_##type##_cancel = new button("Cancel", [&] { /* TODO: cancel change */ }, renderer, 685, 320, 108, 67);\
parent->add(btn_##type##_cancel);


keyboard_view::keyboard_view(window *window) : view(window)
{
	renderer &renderer = this->m_window->get_renderer();
	// create the two content panes for letters and symbols
	this->m_lower = new panel(renderer, 0, 80, 800, 400);
	this->m_upper = new panel(renderer, 0, 80, 800, 400);
	this->m_symbols = new panel(renderer, 0, 80, 800, 400);
	
	// setup buffer
	this->m_buffer = std::string();
	this->m_buffer.reserve(256);
	this->m_buffer.clear();
	
	// buttons for lower letters
	ADD_KEY(l, q, this->m_lower, 0, 0, 80);
	ADD_KEY(l, w, this->m_lower, 1, 0, 80);
	ADD_KEY(l, e, this->m_lower, 2, 0, 80);
	ADD_KEY(l, r, this->m_lower, 3, 0, 80);
	ADD_KEY(l, t, this->m_lower, 4, 0, 80);
	ADD_KEY(l, y, this->m_lower, 5, 0, 80);
	ADD_KEY(l, u, this->m_lower, 6, 0, 80);
	ADD_KEY(l, i, this->m_lower, 7, 0, 80);
	ADD_KEY(l, o, this->m_lower, 8, 0, 80);
	ADD_KEY(l, p, this->m_lower, 9, 0, 80);
	ADD_BACKSPACE(l, this->m_lower);
	
	ADD_KEY(l, a, this->m_lower, 0, 32, 160);
	ADD_KEY(l, s, this->m_lower, 1, 32, 160);
	ADD_KEY(l, d, this->m_lower, 2, 32, 160);
	ADD_KEY(l, f, this->m_lower, 3, 32, 160);
	ADD_KEY(l, g, this->m_lower, 4, 32, 160);
	ADD_KEY(l, h, this->m_lower, 5, 32, 160);
	ADD_KEY(l, j, this->m_lower, 6, 32, 160);
	ADD_KEY(l, k, this->m_lower, 7, 32, 160);
	ADD_KEY(l, l, this->m_lower, 8, 32, 160);
	ADD_ENTER(l, this->m_lower);
	
	ADD_SHIFT(l, this->m_lower, this->m_upper);
	ADD_KEY(l, z,					this->m_lower, 1, 0, 240);
	ADD_KEY(l, x,					this->m_lower, 2, 0, 240);
	ADD_KEY(l, c,					this->m_lower, 3, 0, 240);
	ADD_KEY(l, v,					this->m_lower, 4, 0, 240);
	ADD_KEY(l, b,					this->m_lower, 5, 0, 240);
	ADD_KEY(l, n,					this->m_lower, 6, 0, 240);
	ADD_KEY(l, m,					this->m_lower, 7, 0, 240);
	ADD_KEY_TEXT(l, COMMA,	",",	this->m_lower, 8, 0, 240);
	ADD_KEY_TEXT(l, PERIOD,	".",	this->m_lower, 9, 0, 240);
	
	ADD_CHANGE(l, this->m_lower, this->m_symbols, ".?123");
	ADD_SPACE(l, this->m_lower);
	ADD_CANCEL(l, this->m_lower);
	
	// buttons for upper letters
	ADD_KEY(u, Q, this->m_upper, 0, 0, 80);
	ADD_KEY(u, W, this->m_upper, 1, 0, 80);
	ADD_KEY(u, E, this->m_upper, 2, 0, 80);
	ADD_KEY(u, R, this->m_upper, 3, 0, 80);
	ADD_KEY(u, T, this->m_upper, 4, 0, 80);
	ADD_KEY(u, Y, this->m_upper, 5, 0, 80);
	ADD_KEY(u, U, this->m_upper, 6, 0, 80);
	ADD_KEY(u, I, this->m_upper, 7, 0, 80);
	ADD_KEY(u, O, this->m_upper, 8, 0, 80);
	ADD_KEY(u, P, this->m_upper, 9, 0, 80);
	ADD_BACKSPACE(u, this->m_upper);
	
	ADD_KEY(u, A, this->m_upper, 0, 32, 160);
	ADD_KEY(u, S, this->m_upper, 1, 32, 160);
	ADD_KEY(u, D, this->m_upper, 2, 32, 160);
	ADD_KEY(u, F, this->m_upper, 3, 32, 160);
	ADD_KEY(u, G, this->m_upper, 4, 32, 160);
	ADD_KEY(u, H, this->m_upper, 5, 32, 160);
	ADD_KEY(u, J, this->m_upper, 6, 32, 160);
	ADD_KEY(u, K, this->m_upper, 7, 32, 160);
	ADD_KEY(u, L, this->m_upper, 8, 32, 160);
	ADD_ENTER(u, this->m_upper);
	
	ADD_SHIFT(u, this->m_upper, this->m_lower);
	ADD_KEY(u, Z,					this->m_upper, 1, 0, 240);
	ADD_KEY(u, X,					this->m_upper, 2, 0, 240);
	ADD_KEY(u, C,					this->m_upper, 3, 0, 240);
	ADD_KEY(u, V,					this->m_upper, 4, 0, 240);
	ADD_KEY(u, B,					this->m_upper, 5, 0, 240);
	ADD_KEY(u, N,					this->m_upper, 6, 0, 240);
	ADD_KEY(u, M,					this->m_upper, 7, 0, 240);
	ADD_KEY_TEXT(u, BANG,	"!",	this->m_upper, 8, 0, 240);
	ADD_KEY_TEXT(u, QUES,	"?",	this->m_upper, 9, 0, 240);
	
	ADD_CHANGE(u, this->m_upper, this->m_symbols, ".?123");
	ADD_SPACE(u, this->m_upper);
	ADD_CANCEL(u, this->m_upper);
	
	// buttons for symbols
	ADD_KEY(s, 1, this->m_symbols, 0, 0, 80);
	ADD_KEY(s, 2, this->m_symbols, 1, 0, 80);
	ADD_KEY(s, 3, this->m_symbols, 2, 0, 80);
	ADD_KEY(s, 4, this->m_symbols, 3, 0, 80);
	ADD_KEY(s, 5, this->m_symbols, 4, 0, 80);
	ADD_KEY(s, 6, this->m_symbols, 5, 0, 80);
	ADD_KEY(s, 7, this->m_symbols, 6, 0, 80);
	ADD_KEY(s, 8, this->m_symbols, 7, 0, 80);
	ADD_KEY(s, 9, this->m_symbols, 8, 0, 80);
	ADD_KEY(s, 0, this->m_symbols, 9, 0, 80);
	ADD_BACKSPACE(s, this->m_symbols);
	
	ADD_KEY_TEXT(s, DASH,	"-", this->m_symbols, 0, 32, 160);
	ADD_KEY_TEXT(s, SLASH,	"/", this->m_symbols, 1, 32, 160);
	ADD_KEY_TEXT(s, COLON,	":", this->m_symbols, 2, 32, 160);
	ADD_KEY_TEXT(s, SEMI,	";", this->m_symbols, 3, 32, 160);
	ADD_KEY_TEXT(s, LPAREN,	"(", this->m_symbols, 4, 32, 160);
	ADD_KEY_TEXT(s, RPAREN,	")", this->m_symbols, 5, 32, 160);
	ADD_KEY_TEXT(s, DOLLAR,	"$", this->m_symbols, 6, 32, 160);
	ADD_KEY_TEXT(s, AND,	"&", this->m_symbols, 7, 32, 160);
	ADD_KEY_TEXT(s, AT,		"@", this->m_symbols, 8, 32, 160);
	ADD_ENTER(s, this->m_symbols);
	
	ADD_KEY_TEXT(s, PERIOD,	".", this->m_symbols, 1, 0, 240);
	ADD_KEY_TEXT(s, COMMA,	",", this->m_symbols, 2, 0, 240);
	ADD_KEY_TEXT(s, QUEST,	"?", this->m_symbols, 3, 0, 240);
	ADD_KEY_TEXT(s, BANG,	"!", this->m_symbols, 4, 0, 240);
	ADD_KEY_TEXT(s, APOSTR,	"'", this->m_symbols, 5, 0, 240);
	ADD_KEY_TEXT(s, QUOTE,	"\"",this->m_symbols, 6, 0, 240);
	
	ADD_CHANGE(s, this->m_symbols, this->m_lower, "ABC");
	ADD_SPACE(s, this->m_symbols);
	ADD_CANCEL(s, this->m_symbols);
	
	// build complete scene
	this->m_text_field = new label(renderer, 5, 5, 720, 70, this->m_buffer, 32);
	this->m_window->add(this->m_text_field);
	this->m_window->add(this->m_lower);
}

keyboard_view::~keyboard_view()
{	
	//for (component* c : this->m_lower->get_children()) {
	//	delete c;
	//}
	//for (component* c : this->m_upper->get_children()) {
	//	delete c;
	//}
	//for (component* c : this->m_symbols->get_children()) {
	//	delete c;
	//}
	//delete this->m_lower;
	//delete this->m_upper;
	//delete this->m_symbols;
	//delete this->m_text_field;
}


void keyboard_view::update_component() {}
void keyboard_view::update_model() {}
void keyboard_view::attach(model::model *model, const std::string &name) {}