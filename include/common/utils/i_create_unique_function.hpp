#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include <common/utils/i_create_unique_function.h>

namespace remotePortMapper {

/**
 * @brief       Create object.
 */
template<class Type, typename... Args>
inline Result<::std::unique_ptr<Type>, Error>
    ICreateUniqueFunc<Type, Args...>::create(Args... args)
{
    ::std::unique_ptr<Type> ret(new Type(::std::forward<Args>(args)...));
    auto                    result = ret->takeInitializeResult();
    if (result) {
        return Result<::std::shared_ptr<Type>, Error>::makeOk(::std::move(ret));
    } else {
        return Result<::std::shared_ptr<Type>, Error>::makeError(
            ::std::move(result.template value<Error>()));
    }
}

} // namespace remotePortMapper
