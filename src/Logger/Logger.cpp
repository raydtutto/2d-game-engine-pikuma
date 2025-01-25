#include "Logger.h"

#include "iostream"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <spdlog/spdlog.h>
#include <sstream>

// Define the vector
std::vector<LogEntry> Logger::messages;

// TODO: Make singleton
// TODO: std::vector<LogEntry> change to pointer
// TODO: add singleton destructor to clean up messages std::vector<LogEntry*>
// TODO: Study Alexandresku singleton pattern


// Get the current timestamp
std::string getTimeStamp() {
    // Get current time as a time_point
    const auto now = std::chrono::system_clock::now();

    // Convert to std::time_t
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    // Convert to local time (std::tm)
    const std::tm local_time = *std::localtime(&now_time_t);

    // Format the timestamp
    std::ostringstream timestamp;
    timestamp << std::put_time(&local_time, "%d/%b/%Y %H:%M:%S");  // Format string for desired style
    return timestamp.str();
}

void Logger::Log(const std::string& message) {
    spdlog::info(message);

    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "[" + getTimeStamp() + "] [info] " + message;

    // Add to messages
    messages.push_back(logEntry);
}

void Logger::Warning(const std::string& message) {
    spdlog::warn(message);

    LogEntry logEntry;
    logEntry.type = LOG_WARNING;
    logEntry.message = "[" + getTimeStamp() + "] [warn] " + message;

    // Add to messages
    messages.push_back(logEntry);
}

void Logger::Error(const std::string& message) {
    spdlog::error(message);

    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "[" + getTimeStamp() + "] [error] " + message;

    // Add to messages
    messages.push_back(logEntry);
}