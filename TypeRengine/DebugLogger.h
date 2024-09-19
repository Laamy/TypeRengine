#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <iostream>
#include <chrono>
#include <format>
#include <string>

// temp
enum class LogType {
	Info = 0,
	Warning,
	Error,
	Debug,
	LOG_COUNT
};

namespace DebugLogger
{
	__forceinline std::string to_string(LogType type)
	{
		switch (type) {
			case LogType::Info: return "Info";
			case LogType::Warning: return "Warning";
			case LogType::Error: return "Error";
			case LogType::Debug: return "Debug";
			default: return "Unknown";
		}
	}

	// good enough
	__forceinline std::string get_current_time() 
	{
		auto now = std::chrono::system_clock::now();
		auto time_t = std::chrono::system_clock::to_time_t(now);
		auto tm = *std::localtime(&time_t);

		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		std::ostringstream oss;
		oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");

		// handle milliseconds too
		oss << "." << std::setfill('0') << std::setw(3) << ms.count();

		return oss.str();
	}

	inline void Print(LogType type, const std::string& message)
	{
		std::string curTime = get_current_time();

		std::cout << std::format("[{}, {}] {}\n", curTime, to_string(type), message);
	}

	__forceinline void Debug(const std::string& message)
	{
		Print(LogType::Debug, message);
	}
}

#endif