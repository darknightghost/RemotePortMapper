#pragma once

namespace remotePortMapper {

/// Event source interface.
class IEventSource;

/**
 * @brief   Event loop.
 */
class EventLoop {
  public:
    /**
     * @brief   Event.
     */
    enum class Event {
#if defined(OS_WINDOWS)
        ReadCompleted, ///< Read completed event.
        WriteCompled,  ///< Write completed event.

#elif defined(OS_LINUX)
        Readable,  ///< Readable event.
        Writeable, ///< Writebale event.

#else
    #error Target platform not supported.

#endif
    };
};

} // namespace remotePortMapper

#include <common/event_loop/i_event_source.h>
