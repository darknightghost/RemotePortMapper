#pragma once

#include <chrono>
#include <source_location>
#include <string_view>

#include <common/logger/logger.h>

namespace remotePortMapper {
/**
 * @brief       Default logger.
 */
class DefaultLogger : public Logger {
  public:
    /**
     * @brief   Destructor.
     */
    virtual ~DefaultLogger() = default;

  public:
    /**
     * @brief   Get default logger.
     *
     * @return  Default logger.
     */
    static Logger &getLogger();

  public:
    /**
     * @brief       Get current log level.
     */
    virtual LogLevel logLevel() const override;

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
                     ::std::string                           message) override;
};

} // namespace remotePortMapper
