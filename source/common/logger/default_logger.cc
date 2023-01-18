#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <common/logger/default_logger.h>

#ifdef OS_WINDOWS
    #define gmtime_r(timep, result) gmtime_s((result), (timep))
#endif

namespace remotePortMapper {

/**
 * @brief   Get default logger.
 */
Logger &DefaultLogger::getLogger()
{
    static DefaultLogger logger;
    return logger;
}

/**
 * @brief       Get current log level.
 */
LogLevel DefaultLogger::logLevel() const
{
    return LogLevel::Trace;
}

/**
 * @brief       Write log.
 */
void DefaultLogger::log(LogLevel                                level,
                        ::std::chrono::system_clock::time_point timestamp,
                        const ::std::source_location           &location,
                        ::std::string                           message)
{
    ::std::ostringstream ss;

    // Timestamp.
    time_t seconds = ::std::chrono::duration_cast<::std::chrono::seconds>(
                         timestamp.time_since_epoch())
                         .count();
    struct tm timeStructBuffer;
    auto      timeStruct = ::gmtime_r(&seconds, &timeStructBuffer);
    uint64_t  milliseconds
        = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
              timestamp.time_since_epoch())
              .count()
          % 1000;

    ss << "UTC " << ::std::put_time(timeStruct, "%Y-%m-%d %H:%M:%S") << "."
       << ::std::setw(3) << ::std::setfill('0') << milliseconds << ", \""
       << location.file_name() << ":" << location.line() << "\""
       << ", \"" << location.function_name() << "\""
       << ", ";

    // Log level.
    switch (level) {
        case LogLevel::Trace: {
            ss << "[TRACE]";
        } break;
        case LogLevel::Debug: {
            ss << "[DEBUG]";
        } break;
        case LogLevel::Info: {
            ss << "[INFO]";
        } break;
        case LogLevel::Warning: {
            ss << "[WARNING]";
        } break;
        case LogLevel::Error: {
            ss << "[ERROR]";
        } break;
        case LogLevel::Fatal: {
            ss << "[FATAL]";
        } break;
        default:
            ss << "[UNKNOW]";
    }
    ss << " : " << message << std::endl;

    ::std::cout << ss.str();
}

} // namespace remotePortMapper
