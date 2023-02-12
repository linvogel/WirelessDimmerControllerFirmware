#include "logging.hpp"

#include <format>
#include <vector>
#include <ctime>
#include <algorithm>

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
	struct tm timeinfo;
	time(&raw_time);
	localtime_s(&timeinfo, &raw_time);
	strftime(date_time, 255, "%Y-%m-%d %H:%M:%S", &timeinfo);
	
	std::transform(mod_name.begin(), mod_name.end(), mod_name.begin(), static_cast<int (*)(int)>(&std::toupper));
	std::format_args fargs = std::make_format_args(args);
	std::string msg = std::vformat(fmt, fargs);
	std::string prefix = std::format("[{}][{}][{}] ", date_time, mod_name, level_names[level]);
	
	for (int i = 0; i < out_streams.size(); i++) *out_streams[i] << prefix << msg << std::endl;
	if (level <= dimmer::logging::WARNING) for (int i = 1; i < err_streams.size(); i++) *err_streams[i] << prefix << msg << std::endl;
}

bool dimmer::logging::register_log_level(int level, std::string name)
{
	if (level_names[level].length() > 0) return false;
	level_names[level] = name;
	return true;
}