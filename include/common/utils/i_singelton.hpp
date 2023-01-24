#pragma once

#include <memory>
#include <mutex>
#include <utility>

#include <common/logger/logger.h>

#include <common/utils/i_singelton.h>

namespace remotePortMapper {

/**
 * @brief       Initialize instance.
 */
template<class Type>
template<typename... Args>
inline Result<::std::shared_ptr<Type>, Error>
    ISingleton<Type>::initialize(Args &&...args)
{
    ::std::unique_lock<::std::mutex> lock(_instanceLock);
    if (_instance == nullptr) {
        auto newInstance
            = ::std::shared_ptr<Type>(new Type(::std::forward<Args>(args)...));
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
 */
template<class Type>
inline bool ISingleton<Type>::instanceInitialized()
{
    ::std::unique_lock<::std::mutex> lock(_instanceLock);
    return _instance != nullptr;
}

/**
 * @brief       Get instance.
 */
template<class Type>
inline ::std::shared_ptr<Type> ISingleton<Type>::instance()
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

template<class Type>
::std::shared_ptr<Type> ISingleton<Type>::_instance;

template<class Type>
::std::mutex ISingleton<Type>::_instanceLock;

} // namespace remotePortMapper
