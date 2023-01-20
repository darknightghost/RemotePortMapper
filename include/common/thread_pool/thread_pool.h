#pragma once

#include <functional>
#include <memory>

#include <common/logger/logger.h>

namespace remotePortMapper {

/**
 * @brief   Thread pool.
 */
class ThreadPool {
  public:
    /**
     * @brief   Task to call.
     */
    using Task = ::std::function<void()>;

  private:
  private:
};

} // namespace remotePortMapper
