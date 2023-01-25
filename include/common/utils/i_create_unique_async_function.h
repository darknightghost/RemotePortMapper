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
class ICreateUniqueAsyncFunc : virtual public IInitializeResult {
  protected:
    /**
     * @brief       Constructor.
     */
    ICreateUniqueAsyncFunc() = default;

  public:
    /**
     * #brief   Destructor.
     */
    virtual ~ICreateUniqueAsyncFunc() = default;

  public:
    /**
     * @brief       Create object.
     *
     * @param       args    Arguments of the constructor.
     *
     * @return      Object created or error infomation.
     */
    static inline Result<::std::unique_ptr<Type>, Error> create(Args... args);
};

#define CREATE_UNIQUE(Type, ...)                                             \
    friend class remotePortMapper::ICreateUniqueAsyncFunc<Type,              \
                                                          ##__VA_ARGS__>;    \
                                                                             \
  public:                                                                    \
    using ::remotePortMapper::ICreateUniqueAsyncFunc<Type,                   \
                                                     ##__VA_ARGS__>::create; \
                                                                             \
  private:

} // namespace remotePortMapper

#include <common/utils/i_create_unique_function.hpp>
