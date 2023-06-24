#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include <common/interfaces/i_initialize_result.h>

namespace remotePortMapper {

/**
 * @brief   Factory function interface(normal).
 *
 * @tparam  Type    Class type.
 * @tparam  Args    Types of the arguments of the constructor.
 */
template<class Type, typename... Args>
class ICreateSharedFunc : virtual public IInitializeResult {
  protected:
    /**
     * @brief       Constructor.
     */
    ICreateSharedFunc() = default;

  public:
    /**
     * #brief   Destructor.
     */
    virtual ~ICreateSharedFunc() = default;

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

#define CREATE_SHARED(Type, ...)                                              \
    friend class ::remotePortMapper::ICreateSharedFunc<Type, ##__VA_ARGS__>;  \
                                                                              \
  public:                                                                     \
    using ::remotePortMapper::ICreateSharedFunc<Type, ##__VA_ARGS__>::create; \
                                                                              \
  private:

} // namespace remotePortMapper

#include <common/interfaces/i_create_shared_function.hpp>
