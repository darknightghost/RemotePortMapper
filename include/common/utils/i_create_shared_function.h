#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include <common/utils/i_initialized.h>

namespace remotePortMapper {

/**
 * @brief   Singleton interface.
 *
 * @tparam  Type    Class type.
 * @tparam  Args    Types of the arguments of the constructor.
 */
template<class Type, typename... Args>
    requires ::std::is_constructible<Type, Args...>::value
class ICreateSharedFunc : virtual public IInitialized {
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
     * @brief       Initialize instance.
     *
     *
     * @param       args    Arguments of the constructor.
     *
     * @return      If the instance has not been initialized, the method
     *              will try to initialize a new instance and return the new
     *              instance on success, or \c nullptr on failed. . If the
     *              instance has already been initialized, the method returns
     *              the existing instance.
     */
    static ::std::shared_ptr<Type> initialize(Args &&...args)
    {
        ::std::shared_ptr<Type> ret(new Type(::std::forward<Args>(args)...));
    }
};

#define SINGLETON_OBJECT(Type) \
    friend class remotePortMapper::ICreateSharedFunc<Type>;

} // namespace remotePortMapper
