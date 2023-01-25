#include <common/logger/logger.h>

#include <common/async_object/async_object.h>

namespace remotePortMapper {

/**
 * @brief       Constructor.
 */
AsyncObject::AsyncObject(::std::shared_ptr<ThreadPool> threadPool) :
    m_threadPool(threadPool)
{
    if (m_threadPool == nullptr) {
        panic("Argument \"threadPool\" must not be \"nullptr\"!");
    }
}

/**
 * @brief       Deleter of asynchronous object.
 */
void AsyncObject::deleter(AsyncObject *obj)
{
    obj->threadPool().addTask([obj]() -> void {
        obj->destructor();
    });
}

/**
 * @brief       Asynchronous destructor.
 */
void AsyncObject::destructor()
{
    delete this;
}

} // namespace remotePortMapper
