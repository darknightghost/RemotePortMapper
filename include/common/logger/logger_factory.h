#pragma once

#include <functional>
#include <memory>

#include <common/utils/i_singelton.h>

#include <common/logger/logger.h>

#include <common/logger/default_logger.h>

namespace remotePortMapper {

/**
 * @brief   Factory class of logger.
 */
class LoggerFactory : public ISingleton<LoggerFactory> {
    SINGLETON_OBJECT(LoggerFactory);

  private:
    ::std::function<Logger &()>
        m_createLoggerCallback; ///< Callback to create logger.

  private:
    /**
     * @brief       Constructor.
     *
     * @param[in]   createLoggerCallback    Callback to create logger.
     */
    LoggerFactory(::std::function<Logger &()> createLoggerCallback);

    LoggerFactory(const LoggerFactory &) = delete;
    LoggerFactory(LoggerFactory &&)      = delete;

  public:
    /**
     * @brief       Destructor.
     */
    ~LoggerFactory() = default;

  public:
    /**
     * @brief       Get logger.
     *
     * @return      Logger.
     */
    static Logger &logger();
};

} // namespace remotePortMapper
