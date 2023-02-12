#pragma once

#include <iostream>
#include <string>
#include <stdarg.h>

#define fatal(...) dimmer::logging::_fatal(__VA_ARGS__)
#define error(...) dimmer::logging::_error(__VA_ARGS__)
#define warn(...) dimmer::logging::_warn(__VA_ARGS__)
#define info(...) dimmer::logging::_info(__VA_ARGS__)
#define debug(...) dimmer::logging::_debug(__VA_ARGS__)
#define verbose(...) dimmer::logging::_verbose(__VA_ARGS__)
#define trace(...) dimmer::logging::_trace(__VA_ARGS__)

namespace dimmer
{
	namespace logging {
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
		
		
		inline void log(log_level level, std::string &format, va_list args) { _log(_DIMMER_FILE_NAME_, level, format, args); }
		
		inline void _fatal(std::string msg, ...) { va_list args; __va_start(&args, msg); log(log_level::FATAL, msg, args); }
		inline void _error(std::string msg, ...) { va_list args; __va_start(&args, msg); log(log_level::ERROR, msg, args); }
		inline void _warn(std::string msg, ...) { va_list args; __va_start(&args, msg); log(log_level::WARNING, msg, args); }
		inline void _info(std::string msg, ...) { va_list args; __va_start(&args, msg); log(log_level::INFO, msg, args); }
		inline void _debug(std::string msg, ...) { va_list args; __va_start(&args, msg); log(log_level::DEBUG, msg, args); }
		inline void _verbose(std::string msg, ...) { va_list args; __va_start(&args, msg); log(log_level::VERBOSE, msg, args); }
		inline void _trace(std::string msg, ...) { va_list args; __va_start(&args, msg); log(log_level::TRACE, msg, args); }
	}
} // namespace dimmer
