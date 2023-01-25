#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include <common/utils/i_initialize_result.h>

namespace remotePortMapper {

/**
 * @brief   Factory function interface(normal).
 *
 * @tparam  Type    Class type.
 * @tparam  Args    Types of the arguments of the constructor.
 */
template<class Type, typename... Args>
class ICreateSharedAsyncFunc : virtual public IInitializeResult {
  protected:
    /**
     * @brief       Constructor.
     */
    ICreateSharedAsyncFunc() = default;

  public:
    /**
     * #brief   Destructor.
     */
    virtual ~ICreateSharedAsyncFunc() = default;

  public:
    /**
     * @brief       Create object.
     *
     * @param       args    Arguments of the constructor.
     *
     * @return      Object created or error infomation.
     */
    static inline Result<::std::shared_ptr<Type>, Error> create(Args... args);
};

#define CREATE_SHARED(Type, ...)                                             \
    friend class ::remotePortMapper::ICreateSharedAsyncFunc<Type,            \
                                                            ##__VA_ARGS__>;  \
                                                                             \
  public:                                                                    \
    using ::remotePortMapper::ICreateSharedAsyncFunc<Type,                   \
                                                     ##__VA_ARGS__>::create; \
                                                                             \
  private:

} // namespace remotePortMapper

#include <common/utils/i_create_shared_function.hpp>
