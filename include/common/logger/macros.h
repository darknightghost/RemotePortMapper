#pragma once

#include <sstream>
#include <type_traits>

#include <common/logger/logger.h>
#include <common/logger/logger_factory.h>

#define __write_log(level, message)                                       \
    {                                                                     \
        auto  __logLevel = (level);                                       \
        auto &__logger   = ::remotePortMapper::LoggerFactory::logger();   \
        if (static_cast<typename ::std::underlying_type<                  \
                ::remotePortMapper::LogLevel>::type>(__logger.logLevel()) \
            <= static_cast<typename ::std::underlying_type<               \
                ::remotePortMapper::LogLevel>::type>(__logLevel)) {       \
            ::std::ostringstream __messageSs;                             \
            __messageSs << message;                                       \
            __logger.log(__logLevel, ::std::chrono::system_clock::now(),  \
                         std::source_location::current(),                 \
                         ::std::move(__messageSs.str()));                 \
        }                                                                 \
    }

#define log_trace(message) \
    __write_log(::remotePortMapper::LogLevel::Trace, message);

#define log_debug(message) \
    __write_log(::remotePortMapper::LogLevel::Debug, message);

#define log_info(message) \
    __write_log(::remotePortMapper::LogLevel::Info, message);

#define log_warning(message) \
    __write_log(::remotePortMapper::LogLevel::Warning, message);

#define log_error(message) \
    __write_log(::remotePortMapper::LogLevel::Error, message);

#define log_fatal(message) \
    __write_log(::remotePortMapper::LogLevel::Fatal, message);
