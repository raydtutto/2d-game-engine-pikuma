#include "Logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> // Colored console output

std::vector<LogEntry> Logger::messages;
std::shared_ptr<spdlog::logger> Logger::loggerInstance = nullptr;

void Logger::Initialize() {
    loggerInstance = std::make_shared<spdlog::logger>("console", spdlog::sink_ptr(new spdlog::sinks::stdout_color_sink_mt()));
    spdlog::set_default_logger(loggerInstance);
    spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
}

void Logger::Shutdown() {
    spdlog::shutdown();
}

void Logger::AddToMessages(LogType type, const std::string& message) {
    messages.push_back({message, type});
}