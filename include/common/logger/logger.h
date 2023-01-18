#pragma once

#include <chrono>
#include <source_location>
#include <string_view>

namespace remotePortMapper {

/**
 * @brief   Log level.
 */
enum class LogLevel {
    Trace   = 0,
    Debug   = 10000,
    Info    = 20000,
    Warning = 30000,
    Error   = 40000,
    Fatal   = 50000
};

/**
 * @brief       Base class of loggers.
 */
class Logger {
  public:
    /**
     * @brief   Destructor.
     */
    virtual ~Logger() = default;

  public:
    /**
     * @brief       Get current log level.
     */
    virtual LogLevel logLevel() const = 0;

    /**
     * @brief       Write log.
     *
     * @param[in]   level       Log level.
     * @param[in]   timestamp   Timestamp.
     * @param[in]   location    Source location.
     * @param[in]   message     Log message.
     */
    virtual void log(LogLevel                                level,
                     ::std::chrono::system_clock::time_point timestamp,
                     const ::std::source_location           &location,
                     ::std::string                           message)
        = 0;
};

} // namespace remotePortMapper

#include <common/logger/macros.h>
