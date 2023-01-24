#pragma once

#include <common/utils/i_create_shared_function.h>

namespace remotePortMapper {

/**
 * @brief       Create object.
 */
template<class Type, typename... Args>
inline Result<::std::shared_ptr<Type>, Error>
    ICreateSharedFunc<Type, Args...>::create(Args... args)
{
    ::std::shared_ptr<Type> ret(new Type(::std::forward<Args>(args)...));
    auto                    result = ret->takeInitializeResult();
    if (result) {
        return Result<::std::shared_ptr<Type>, Error>::makeOk(ret);
    } else {
        return Result<::std::shared_ptr<Type>, Error>::makeError(
            ::std::move(result.template value<Error>()));
    }
}

} // namespace remotePortMapper
