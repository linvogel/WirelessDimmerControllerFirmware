#include <gui/view.hpp>

#include <gui/label.hpp>

namespace dim {
	namespace gui {
		/**
		 * @brief The keyboard view handles keyboard inputs from the touch screen.
		 * It displays a keyboard as well as a text field which displays the current entry.
		 * It allows to accept an entry, updating the corresponding model, or cancel.
		 * 
		 */
		class keyboard_view : public virtual view {
			std::string m_buffer;
			component *m_lower;
			component *m_upper;
			component *m_symbols;
			label *m_text_field;
		public:
			keyboard_view(window *window);
			~keyboard_view();
			
			void update_component() override;
			void update_model() override;
			void attach(model::model *model, const std::string &name) override;
		};
	}
}