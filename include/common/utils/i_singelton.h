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
    static Result<::std::shared_ptr<Type>, Error> initialize(Args &&...args)
    {
        ::std::unique_lock<::std::mutex> lock(_instanceLock);
        if (_instance == nullptr) {
            auto newInstance = ::std::shared_ptr<Type>(
                new Type(::std::forward<Args>(args)...));
            auto result = newInstance->takeInitializeResul();
            if (result) {
                _instance = newInstance;
            } else {
                return Result<::std::shared_ptr<Type>, Error>::makeError(
                    ::std::move(result.template value<Error>()));
            }
        }
        return Result<::std::shared_ptr<Type>, Error>::makeOk(_instance);
    }

    /**
     * @brief       Check if the instance is initialized.
     *
     * @return      Result.
     */
    static bool instanceInitialized()
    {
        ::std::unique_lock<::std::mutex> lock(_instanceLock);
        return _instance != nullptr;
    }

    /**
     * @brief       Get instance.
     *
     * @return      Instance.
     */
    static ::std::shared_ptr<Type> instance()
    {
        ::std::shared_ptr<Type> ret;
        {
            ::std::unique_lock<::std::mutex> lock(_instanceLock);
            ret = _instance;
        }
        if (ret == nullptr) {
            panic("Try to get instance before initialized.");
        }
        return ret;
    }
};

template<class Type>
::std::shared_ptr<Type> ISingleton<Type>::_instance;

template<class Type>
::std::mutex ISingleton<Type>::_instanceLock;

#define SINGLETON_OBJECT(Type) friend class remotePortMapper::ISingleton<Type>;

} // namespace remotePortMapper
