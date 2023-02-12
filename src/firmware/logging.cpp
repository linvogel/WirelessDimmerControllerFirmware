#include "logging.hpp"

#include <vector>
#include <ctime>
#include <algorithm>
#include <string.h>

static std::vector<std::ostream*> out_streams;
static std::vector<std::ostream*> err_streams;

static std::vector<std::string> level_names(128, std::string{});

static dimmer::logging::log_level current_level = dimmer::logging::TRACE;

void dimmer::logging::init()
{
	
	out_streams.push_back(&std::cout);
	err_streams.push_back(&std::cerr);
	
	level_names[dimmer::logging::FATAL] = "FATAL";
	level_names[dimmer::logging::ERROR] = "ERROR";
	level_names[dimmer::logging::WARNING] = "WARNING";
	level_names[dimmer::logging::INFO] = "INFO";
	level_names[dimmer::logging::DEBUG] = "DEBUG";
	level_names[dimmer::logging::VERBOSE] = "VERBOSE";
	level_names[dimmer::logging::TRACE] = "TRACE";
}

void dimmer::logging::set_log_level(dimmer::logging::log_level level)
{
	current_level = level;
}

void dimmer::logging::add_out_stream(std::ostream out)
{
	out_streams.push_back(&out);
}

void dimmer::logging::add_err_stream(std::ostream out)
{
	err_streams.push_back(&out);
}

void dimmer::logging::_log(std::string mod_name, dimmer::logging::log_level level, std::string &fmt, va_list args)
{
	if (level > current_level) return;
	char date_time[256];
	time_t raw_time;
	time(&raw_time);
	struct tm *timeinfo = localtime(&raw_time);
	strftime(date_time, 255, "%Y-%m-%d %H:%M:%S", timeinfo);
	
	std::transform(mod_name.begin(), mod_name.end(), mod_name.begin(), static_cast<int (*)(int)>(&std::toupper));
	// if this is not enough, then i don't know anymore
	char buf_msg[64*1024];
	char buf_pre[256];
	size_t printed_msg = snprintf(buf_msg, 64*1024, fmt.c_str(), args);
	size_t printed_pre = snprintf(buf_pre, 256, "[%s][%s][%s] ", date_time, mod_name.c_str(), level_names[level].c_str());
	if (printed_msg <= 0 || printed_msg >= 64*1024) error("Log message exceeded 64KB! Please enuse that log messages are strictly smaller in size than 64KB!");
	if (printed_pre <= 0 || printed_pre >= 256) fatal("failed to print prefix size!");
	
	for (int i = 0; i < out_streams.size(); i++) *out_streams[i] << buf_pre << buf_msg << std::endl;
	if (level <= dimmer::logging::WARNING) for (int i = 1; i < err_streams.size(); i++) *err_streams[i] << buf_pre << buf_msg << std::endl;
}

bool dimmer::logging::register_log_level(int level, std::string name)
{
	if (level_names[level].length() > 0) return false;
	level_names[level] = name;
	return true;
}