#pragma once

#include <memory>
#include <mutex>
#include <utility>

#include <common/logger/logger.h>

#include <common/utils/i_initialize_result.h>

namespace remotePortMapper {

/**
 * @brief   Singleton interface.
 *
 * @tparam  Type    Class type.
 */
template<class Type>
class ISingleton : virtual public IInitializeResult {
  private:
    static ::std::shared_ptr<Type> _instance;     ///< Instance.
    static ::std::mutex            _instanceLock; ///< Instance lock.

  protected:
    /**
     * @brief       Constructor.
     */
    ISingleton() = default;

  public:
    /**
     * #brief   Destructor.
     */
    virtual ~ISingleton() = default;

  public:
    /**
     * @brief       Initialize instance.
     *
     * @tparam      Args    Types of the arguments of the constructor.
     *
     * @param       args    Arguments of the constructor.
     *
     * @return      If the instance has not been initialized, the method
     *              will try to initialize a new instance and return the new
     *              instance on success, or \c nullptr on failed. . If the
     *              instance has already been initialized, the method returns
     *              the existing instance.
     */
    template<typename... Args>
    static inline Result<::std::shared_ptr<Type>, Error>
        initialize(Args &&...args);

    /**
     * @brief       Check if the instance is initialized.
     *
     * @return      Result.
     */
    static inline bool instanceInitialized();

    /**
     * @brief       Get instance.
     *
     * @return      Instance.
     */
    static inline ::std::shared_ptr<Type> instance();
};

#define SINGLETON_OBJECT(Type) friend class remotePortMapper::ISingleton<Type>;

} // namespace remotePortMapper

#include <common/utils/i_singelton.hpp>
