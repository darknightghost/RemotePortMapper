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
        Read,  ///< Read event.
        Write, ///< Write event.
    };
};

} // namespace remotePortMapper

#include <common/event_loop/i_event_source.h>
