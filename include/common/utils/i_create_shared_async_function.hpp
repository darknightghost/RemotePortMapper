#pragma once

#include <common/async_object/async_object.h>

#include <common/utils/i_create_shared_async_function.h>

namespace remotePortMapper {

/**
 * @brief       Create object.
 */
template<class Type, typename... Args>
inline Result<::std::shared_ptr<Type>, Error>
    ICreateSharedAsyncFunc<Type, Args...>::create(Args... args)
{
    static_assert(
        ::std::is_base_of<AsyncObject, Type>::value,
        "Interface \"ICreateSharedAsyncFunc\" must be implemented by an "
        "asynchronous object.");
    ::std::shared_ptr<Type> ret(
        new Type(::std::forward<Args>(args)...),
        [](typename ::std::add_pointer<Type>::type ptr) -> void {
            Type::deleter(static_cast<AsyncObject *>(ptr));
        });
    auto result = ret->takeInitializeResult();
    if (result) {
        return Result<::std::shared_ptr<Type>, Error>::makeOk(ret);
    } else {
        return Result<::std::shared_ptr<Type>, Error>::makeError(
            ::std::move(result.template value<Error>()));
    }
}

} // namespace remotePortMapper
