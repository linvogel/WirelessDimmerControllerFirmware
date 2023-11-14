#pragma once

#include <functional>
#include <map>
#include <vector>
#include <string>

#include <gui/window.hpp>

namespace dim {
	namespace sh {
		
		class shell {
			dim::gui::window &m_window;
			std::map<std::string, std::function<void(std::vector<std::string>&)>> m_cmds;
			
		public:
			shell(dim::gui::window &window);
			
			void run(const std::string &command);
		private:
			void add_command(const std::string &cmd_name, std::function<void(std::vector<std::string>&)> func);
		};
		
	}
}