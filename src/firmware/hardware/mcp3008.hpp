#pragma once

#include "device.hpp"

namespace dim {
	namespace hw {
		
		class mcp3008 : public virtual device {
		public:
			mcp3008();
			
			virtual void sync() override;
		};
	}
}