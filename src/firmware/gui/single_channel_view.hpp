#pragma once

#include <gui/view.hpp>

namespace dim {
	namespace gui {
		
		class single_channel_view : public virtual view {
			
		public:
			single_channel_view();
			
			void update_component() override;
			void update_model() override;
			void attach(model::model *model, const std::string &name) override;
		};
	}
}