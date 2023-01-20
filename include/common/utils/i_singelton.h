#pragma once

#include <memory>
#include <mutex>
#include <utility>

#include <common/utils/i_initialized.h>

namespace remotePortMapper {

/**
 * @brief   Singleton interface.
 *
 * @tparam  Type    Class type.
 */
template<class Type>
class ISingleton : virtual public IInitialized {
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
    static ::std::shared_ptr<Type> initialize(Args &&...args)
    {
        ::std::unique_lock<::std::mutex> lock(_instanceLock);
        if (_instance == nullptr) {
            auto newInstance = ::std::shared_ptr<Type>(
                new Type(::std::forward<Args>(args)...));
            if (newInstance->initialized()) {
                _instance = newInstance;
            }
        }
        return _instance;
    }

    /**
     * @brief       Get instance.
     *
     * @return      Instance.
     */
    static ::std::shared_ptr<Type> instance()
    {
        ::std::unique_lock<::std::mutex> lock(_instanceLock);
        return _instance;
    }
};

template<class Type>
::std::shared_ptr<Type> ISingleton<Type>::_instance;

template<class Type>
::std::mutex ISingleton<Type>::_instanceLock;

#define SINGLETON_OBJECT(Type) friend class remotePortMapper::ISingleton<Type>;

} // namespace remotePortMapper
