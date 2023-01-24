#pragma once

#include <functional>
#include <memory>

#include <common/logger/logger.h>

#include <common/logger/default_logger.h>

namespace remotePortMapper {

/**
 * @brief   Factory class of logger.
 */
class LoggerFactory {
  private:
    LoggerFactory()                      = delete;
    LoggerFactory(const LoggerFactory &) = delete;
    LoggerFactory(LoggerFactory &&)      = delete;

  public:
    /**
     * @brief       Destructor.
     */
    ~LoggerFactory() = delete;

  public:
    /**
     * @brief       Set factory function of logger.
     *
     * @param[in]   factoryFunc     Factory function to create logger.
     */
    static void setFactoryFunc(::std::function<Logger &()> factoryFunc);

    /**
     * @brief       Get logger.
     *
     * @return      Logger.
     */
    static Logger &logger();

  private:
    /**
     * @brief       Get logger callback.
     *
     * @return      callback.
     */
    static ::std::function<Logger &()> &callback();
};

} // namespace remotePortMapper
