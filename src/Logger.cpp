#include "Logger.h"

#include "iostream"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Define the vector
std::vector<LogEntry> Logger::messages;

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
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "LOG: [" + getTimeStamp() + "] - " + message;

    // ANSI escape code for green color
    const std::string greenText = "\033[32m";
    const std::string resetText = "\033[0m";

    // Output text in color
    std::cout << greenText << logEntry.message << resetText << std::endl;

    // Add to messages
    messages.push_back(logEntry);
}

void Logger::Warning(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_WARNING;
    logEntry.message = "WRN: [" + getTimeStamp() + "] - " + message;

    // ANSI escape code for yellow color
    const std::string yellowText = "\033[33m";
    const std::string resetText = "\033[0m";

    // Output text in color
    std::cout << yellowText << logEntry.message << resetText << std::endl;

    // Add to messages
    messages.push_back(logEntry);
}

void Logger::Error(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERR: [" + getTimeStamp() + "] - " + message;

    // ANSI escape code for red color
    const std::string redText = "\033[31m";
    const std::string resetText = "\033[0m";

    // Output text in color
    std::cout << redText << logEntry.message << resetText << std::endl;

    // Add to messages
    messages.push_back(logEntry);
}