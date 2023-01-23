#include <common/logger/logger_factory.h>

namespace remotePortMapper {

/**
 * @brief       Constructor.
 */
LoggerFactory::LoggerFactory(::std::function<Logger &()> createLoggerCallback) :
    m_createLoggerCallback(::std::move(createLoggerCallback))
{
    this->setInitializeResult(Result<void, Error>::makeOk());
}

/**
 * @brief       Get logger.
 */
Logger &LoggerFactory::logger()
{
    if (LoggerFactory::instanceInitialized()) {
        return DefaultLogger::getLogger();

    } else {
        auto instance = LoggerFactory::instance();
        return instance->m_createLoggerCallback();
    }
}

} // namespace remotePortMapper
