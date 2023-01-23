#include <chrono>
#include <source_location>
#include <string_view>

#include <common/logger/logger.h>

namespace remotePortMapper {

/**
 * @brief       Do panic.
 */
void __doPanic(::std::chrono::system_clock::time_point timestamp,
               const ::std::source_location           &location,
               ::std::string                           message)
{
    auto  logLevel = LogLevel::Fatal;
    auto &logger   = ::remotePortMapper::LoggerFactory::logger();
    if (static_cast<typename ::std::underlying_type<
            ::remotePortMapper::LogLevel>::type>(logger.logLevel())
        <= static_cast<typename ::std::underlying_type<
            ::remotePortMapper::LogLevel>::type>(logLevel)) {
        ::std::ostringstream messageSs;
        messageSs << "PANIC : " << message;
        logger.log(logLevel, timestamp, location, messageSs.str());
    }
}

} // namespace remotePortMapper
