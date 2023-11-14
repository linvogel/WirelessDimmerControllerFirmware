#pragma once

#include "device.hpp"

namespace dim {
	namespace hw {
		
		class spi_device : public virtual device {
			int m_pin;
		};
		
	}
}