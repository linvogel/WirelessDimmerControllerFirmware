#pragma once

#include <gui/window.hpp>
#include <model/model.hpp>

#include <string>


namespace dim {
	namespace gui {
		/**
		 * @brief Views combine a component and a model to be displayed.
		 * The view attaches the data from the model to the component and updates the model
		 * when changes are made in the component.
		 * 
		 * Additionally, views build the component GUI and are responsible for displaying it.
		 * 
		 */
		class view {
		protected:
			window* m_window;
			size_t m_scene;
			
		public:
			view(window *window);
			
			void show();
			virtual void update_component() = 0;
			virtual void update_model() = 0;
			virtual void attach(model::model *model, const std::string &name) = 0;
		};
	}
}