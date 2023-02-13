#pragma once

#include <string>
#include <vector>

namespace dim {
	namespace event {
		
		class event;
		/**
		 * @brief The event system is responsible for receiving, directing and passing events from producers to consumers.
		 * Any consumer may chose to handle and consume an event, or to skip it and leave it for any potential consumer
		 * of lower priority to the event system.
		 */
		class event_system {
			
			
		public:
			event_system();
			virtual ~event_system();
			
			virtual void register_event_handler();
			virtual void throw_event(event evt);
		};
	}
}