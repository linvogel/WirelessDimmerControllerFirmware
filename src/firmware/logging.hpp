#pragma once

#include <iostream>
#include <string>
#include <cstdarg>
#include <array>
#include <assert.h>

#define _CUSTOM_LOGGING_INCLUDED

static constexpr const char* _log_base_file_name(const char *full_name) {
	const char* best = full_name;
	const char* current = full_name;
	while (*current) {
		if (*current == '/' || *current == '\\') {
			best = current + 1;
		}
		current++;
	}
	return best;
}

#if !defined(MODULE_NAME)
#define MODULE_NAME "N/A"
#endif

#if defined(WIN32)
#define __CUSTOM_FUNCTION_NAME__ __FUNCTION__
#elif defined(__GNUG__)
#define __CUSTOM_FUNCTION_NAME__ __func__
#endif

#define fatal(...) dim::log::_fatal(__VA_ARGS__)
#define error(...) dim::log::_error(__VA_ARGS__)
#define warn(...) dim::log::_warn(__VA_ARGS__)
#define info(...) dim::log::_info(__VA_ARGS__)
#define debug(...) dim::log::_debug(__VA_ARGS__)
#define verbose(...) dim::log::_verbose(__VA_ARGS__)
#define trace(...) dim::log::_trace(__VA_ARGS__)

#define ftrace() dim::log::_trace("Function Trace: %s", __CUSTOM_FUNCTION_NAME__)


namespace dim{
	namespace log {
		enum log_level {
			FATAL=0,
			ERROR=10,
			WARNING=20,
			INFO=30,
			DEBUG=40,
			VERBOSE=50,
			TRACE=60
		};
		
		
		void init();
		void set_log_level(log_level level);
		void add_out_stream(std::ostream out);
		void add_err_stream(std::ostream out);
		bool register_log_level(int priority, std::string name);
		void _log(std::string mod_name, log_level level, std::string &format, va_list args);
		
		inline void log(log_level level, std::string &format, va_list args) { _log(std::string(MODULE_NAME), level, format, args); }
		
		inline void _fatal(std::string msg, ...) { va_list args; va_start(args, msg); log(log_level::FATAL, msg, args); }
		inline void _error(std::string msg, ...) { va_list args; va_start(args, msg); log(log_level::ERROR, msg, args); }
		inline void _warn(std::string msg, ...) { va_list args; va_start(args, msg); log(log_level::WARNING, msg, args); }
		inline void _info(std::string msg, ...) { va_list args; va_start(args, msg); log(log_level::INFO, msg, args); }
		inline void _debug(std::string msg, ...) { va_list args; va_start(args, msg); log(log_level::DEBUG, msg, args); }
		inline void _verbose(std::string msg, ...) { va_list args; va_start(args, msg); log(log_level::VERBOSE, msg, args); }
		inline void _trace(std::string msg, ...) { va_list args; va_start(args, msg); log(log_level::TRACE, msg, args); }
	}
} // namespace dimmer
