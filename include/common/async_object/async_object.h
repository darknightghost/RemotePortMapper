#pragma once

#include <functional>
#include <memory>

namespace remotePortMapper {

/**
 * @brief   Thread pool.
 */
class ThreadPool;

/**
 * @brief       Asynchronous object.
 */
class AsyncObject {
  private:
    ::std::shared_ptr<ThreadPool> m_threadPool; ///< Thread pool.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   threadPool      Thread pool.
     */
    AsyncObject(::std::shared_ptr<ThreadPool> threadPool);

    /**
     * @brief     Destructor.
     */
    virtual ~AsyncObject() = default;

  public:
    /**
     * @brief   Get thread pool.
     *
     * @return  Thread pool.
     */
    inline ThreadPool &threadPool();

  public:
    /**
     * @brief       Deleter of asynchronous object.
     *
     * @param[in]   obj     Object to delete.
     */
    static void deleter(AsyncObject *obj);

  protected:
    /**
     * @brief       Asynchronous destructor.
     */
    virtual void destructor();
};

} // namespace remotePortMapper

#include <common/thread_pool/thread_pool.h>

#include <common/async_object/async_object.hpp>
