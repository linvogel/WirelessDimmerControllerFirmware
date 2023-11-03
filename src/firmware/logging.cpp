
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <string.h>

#define MODULE_NAME "logger"
#include "logging.hpp"

static std::vector<std::ostream*> out_streams;
static std::vector<std::ostream*> err_streams;

static std::vector<std::string> level_names(128, std::string{});

static dim::log::log_level current_level = dim::log::TRACE;

void dim::log::init()
{
	out_streams.push_back(&std::cout);
	err_streams.push_back(&std::cerr);
	
	level_names[dim::log::FATAL] = "FATAL";
	level_names[dim::log::ERROR] = "ERROR";
	level_names[dim::log::WARNING] = "WARNING";
	level_names[dim::log::INFO] = "INFO";
	level_names[dim::log::DEBUG] = "DEBUG";
	level_names[dim::log::VERBOSE] = "VERBOSE";
	level_names[dim::log::TRACE] = "TRACE";
}

void dim::log::set_log_level(dim::log::log_level level)
{
	current_level = level;
}

void dim::log::add_out_stream(std::ostream out)
{
	out_streams.push_back(&out);
}

void dim::log::add_err_stream(std::ostream out)
{
	err_streams.push_back(&out);
}

void dim::log::_log(std::string mod_name, dim::log::log_level level, std::string &fmt, va_list args)
{
	if (level > current_level) return;
	char date_time[256];
	std::chrono::time_point now = std::chrono::system_clock::now();
	std::chrono::milliseconds millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
	time_t raw_time = std::chrono::system_clock::to_time_t(now);
	struct tm *timeinfo = localtime(&raw_time);
	strftime(date_time, 255, "%Y-%m-%d %H:%M:%S", timeinfo);
	
	std::transform(mod_name.begin(), mod_name.end(), mod_name.begin(), static_cast<int (*)(int)>(&std::toupper));
	// if this is not enough, then i don't know anymore
	char buf_msg[64*1024];
	char buf_pre[256];
	size_t printed_msg = vsnprintf(buf_msg, 64*1024, fmt.c_str(), args);
	size_t printed_pre = snprintf(buf_pre, 256, "[%s.%03d][%s][%s] ", date_time, (int)(millis.count() % 1000), mod_name.c_str(), level_names[level].c_str());
	if (printed_msg <= 0 || printed_msg >= 64*1024) {
		error("Log message exceeded 64KB! Please enuse that log messages are strictly smaller in size than 64KB!");
		return;
	}
	if (printed_pre <= 0 || printed_pre >= 256) {
		fatal("failed to print prefix size!");
		return;
	}
	
	for (int i = 0; i < out_streams.size(); i++) *out_streams[i] << buf_pre << buf_msg << std::endl;
	if (level <= dim::log::WARNING) for (int i = 1; i < err_streams.size(); i++) *err_streams[i] << buf_pre << buf_msg << std::endl;
}

bool dim::log::register_log_level(int level, std::string name)
{
	if (level_names[level].length() > 0) return false;
	level_names[level] = name;
	return true;
}