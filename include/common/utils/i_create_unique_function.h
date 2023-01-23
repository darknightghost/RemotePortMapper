#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include <common/utils/i_initialize_result.h>

namespace remotePortMapper {

/**
 * @brief   Factory function interface.
 *
 * @tparam  Type    Class type.
 * @tparam  Args    Types of the arguments of the constructor.
 */
template<class Type, typename... Args>
    requires ::std::is_constructible<Type, Args...>::value
class ICreateUniqueFunc : virtual public IInitializeResult {
  protected:
    /**
     * @brief       Constructor.
     */
    ICreateUniqueFunc() = default;

  public:
    /**
     * #brief   Destructor.
     */
    virtual ~ICreateUniqueFunc() = default;

  public:
    /**
     * @brief       Initialize instance.
     *
     * @param       args    Arguments of the constructor.
     *
     * @return      Object created or error infomation.
     */
    static Result<::std::unique_ptr<Type>, Error> initialize(Args &&...args)
    {
        ::std::unique_ptr<Type> ret(new Type(::std::forward<Args>(args)...));
        auto                    result = ret->takeInitializeResult();
        if (result) {
            return Result<::std::shared_ptr<Type>, Error>::makeOk(
                ::std::move(ret));
        } else {
            return Result<::std::shared_ptr<Type>, Error>::makeError(
                ::std::move(result.template value<Error>()));
        }
    }
};

#define CREATE_UNIQUE(Type, ...) \
    friend class remotePortMapper::ICreateUniqueFunc<Type, ##__VA_ARGS__>;

} // namespace remotePortMapper
