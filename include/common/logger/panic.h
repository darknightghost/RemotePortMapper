#pragma once

#include <chrono>
#include <source_location>
#include <string_view>

#define panic(message)                                                 \
    {                                                                  \
        ::std::ostringstream __messageSs;                              \
        __messageSs << message;                                        \
        __doPanic(::std::chrono::system_clock::now(),                  \
                  std::source_location::current(), __messageSs.str()); \
    }

namespace remotePortMapper {

/**
 * @brief       Do panic.
 *
 * @param[in]   timestamp   Timestamp.
 * @param[in]   location    Source location.
 * @param[in]   message     Log message.
 */
void __doPanic(::std::chrono::system_clock::time_point timestamp,
               const ::std::source_location           &location,
               ::std::string                           message);

} // namespace remotePortMapper
