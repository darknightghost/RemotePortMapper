#pragma once

#include <common/async_object/async_object.h>

namespace remotePortMapper {

/**
 * @brief       Asynchronous destructor.
 */
inline ThreadPool &AsyncObject::threadPool()
{
    return *m_threadPool;
}

} // namespace remotePortMapper
