#include "shell.hpp"

#define MODULE_NAME "shell"
#include <logging.hpp>

using namespace dim::sh;
using namespace dim::gui;
using namespace dim::model;


shell::shell(window &window)
	: m_window(window)
{
	ftrace();
	this->m_cmds.clear();
}


void shell::run(const std::string &command)
{
	ftrace();
	// parse command
	std::vector<std::string> args;
	size_t off = 0;
	size_t end = 0;
	while (off < command.size()) {
		// find next word ending
		while (++end < command.size() && (command.at(end) != ' ' && command.at(end) != '\t' && command.at(end) != '\r' && command.at(end) != '\n'));
		
		// add word to command line
		args.push_back(command.substr(off, end - off));
		
		// skipp following whitespace
		while (end < command.size() && (command.at(end) == ' ' || command.at(end) == '\t' || command.at(end) == '\r' || command.at(end) == '\n')) end++;
		off = end;
	}
	
	if (args.size() == 0) return;
	
	// run command
	if (this->m_cmds.find(args.at(0)) == this->m_cmds.end()) {
		error("Unknown command: %s", args.at(0).c_str());
		return;
	}
	
	this->m_cmds.at(args[0])(args);
}

void shell::add_command(const std::string &cmd_name, std::function<void(std::vector<std::string>&)> func)
{
	ftrace();
	if (this->m_cmds.find(cmd_name) != this->m_cmds.end()) {
		error("Shell command already registered: '%s'", cmd_name.c_str());
		return;
	}
	
	this->m_cmds.insert(std::make_pair(cmd_name, func));
}