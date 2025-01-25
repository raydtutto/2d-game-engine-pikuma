#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <vector>

enum LogType {
    LOG_INFO, // 0
    LOG_WARNING, // 1
    LOG_ERROR // 2
};

struct LogEntry {
    LogType type;
    std::string message;
};

class Logger {
public:
    static std::vector<LogEntry> messages;
    static void Log(const std::string& message);
    static void Error(const std::string& message);
    static void Warning(const std::string& message);
};

#endif  // LOGGER_H
