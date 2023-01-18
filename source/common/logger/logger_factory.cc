#include <common/logger/logger_factory.h>

namespace remotePortMapper {

/**
 * @brief       Constructor.
 */
LoggerFactory::LoggerFactory(::std::function<Logger &()> createLoggerCallback) :
    m_createLoggerCallback(::std::move(createLoggerCallback))
{}

/**
 * @brief       Get logger.
 */
Logger &LoggerFactory::logger()
{
    auto instance = LoggerFactory::instance();
    if (instance == nullptr) {
        return DefaultLogger::getLogger();

    } else {
        return instance->m_createLoggerCallback();
    }
}

} // namespace remotePortMapper
