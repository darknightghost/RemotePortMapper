#include <common/logger/logger_factory.h>

namespace remotePortMapper {

/**
 * @brief       Set factory function of logger.
 */
void LoggerFactory::setFactoryFunc(::std::function<Logger &()> factoryFunc)
{
    LoggerFactory::callback() = ::std::move(factoryFunc);
}

/**
 * @brief       Get logger.
 *
 * @return      Logger.
 */
Logger &LoggerFactory::logger()
{
    if (LoggerFactory::callback()) {
        return LoggerFactory::callback()();

    } else {
        return DefaultLogger::getLogger();
    }
}

/**
 * @brief       Get logger callback.
 */
::std::function<Logger &()> &LoggerFactory::callback()
{
    static ::std::function<Logger &()> ret;
    return ret;
}

} // namespace remotePortMapper
