#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>
#include <memory>
#include <fmt/core.h>

#include <spdlog/spdlog.h> // Forward declaration of spdlog classes

enum LogType {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntry {
    std::string message;
    LogType type;
};

class Logger {
public:
    static void Initialize();
    static void Shutdown();

    template<typename... Args>
    static void Log(const std::string& format, Args&&... args) {
        std::string formattedMessage = fmt::format(format, std::forward<Args>(args)...);
        spdlog::info(formattedMessage);
        AddToMessages(LOG_INFO, formattedMessage);
    }

    template<typename... Args>
    static void Warning(const std::string& format, Args&&... args) {
        std::string formattedMessage = fmt::format(format, std::forward<Args>(args)...);
        spdlog::warn(formattedMessage);
        AddToMessages(LOG_WARNING, formattedMessage);
    }

    template<typename... Args>
    static void Error(const std::string& format, Args&&... args) {
        std::string formattedMessage = fmt::format(format, std::forward<Args>(args)...);
        spdlog::error(formattedMessage);
        AddToMessages(LOG_ERROR, formattedMessage);
    }

    static std::vector<LogEntry> messages;

private:
    static std::shared_ptr<spdlog::logger> loggerInstance;
    static void AddToMessages(LogType type, const std::string& message);
};

#endif  // LOGGER_H
