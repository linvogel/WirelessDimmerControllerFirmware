#pragma once

#include <string>

#include "event_system.hpp"

namespace dimmer {
	namespace event {
		
		class event {
			std::string m_name;
			event_system &evt_system;
			
			
		public:
			event(std::string evt_name);
			virtual ~event();
			
			virtual std::string get_name();
			virtual void skip() final;
			virtual void consume() final;
		};
		
	}
}