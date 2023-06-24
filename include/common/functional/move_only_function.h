#pragma once

#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>

#include <common/types/type_traits.h>

namespace remotePortMapper {

/**
 * @brief   Invoker of move-only function.
 */
template<typename>
class MoveOnlyFunctionInvokerBase;

/**
 * @brief   Invoker of move-only function.
 *
 * @tparam  ReturnType  Type of return value.
 * @tparam  ArgTypes    Type of arguments.
 */
template<typename ReturnType, typename... ArgTypes>
class MoveOnlyFunctionInvokerBase<ReturnType(ArgTypes...)> {
  public:
    /**
     * @brief       Constructor.
     */
    MoveOnlyFunctionInvokerBase()                                    = default;
    MoveOnlyFunctionInvokerBase(const MoveOnlyFunctionInvokerBase &) = delete;
    MoveOnlyFunctionInvokerBase(MoveOnlyFunctionInvokerBase &&)      = delete;

    /**
     * @brief       Invoke.
     *
     * @param[in]   args        Arguments.
     *
     * @return      Return value of the function.
     */
    virtual ReturnType invoke(ArgTypes &&...args) = 0;

    /**
     * @brief       Destructor.
     */
    virtual ~MoveOnlyFunctionInvokerBase() = default;
};

/**
 * @brief   Invoker for move-only function(\c ReturnType is not \c void).
 *
 * @tparam  CallableType    Type of the callable.
 * @tparam  ReturnType      Type of return value.
 * @tparam  ArgTypes        Type of arguments.
 */
template<typename CallableType, typename ReturnType, typename... ArgTypes>
    requires(
        ::std::is_invocable_r<ReturnType, CallableType, ArgTypes...>::value)
class MoveOnlyFunctionInvoker :
    public MoveOnlyFunctionInvokerBase<ReturnType(ArgTypes...)> {
  private:
    CallableType m_callable; ///< Callable object.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   callable        Callable object.
     */
    inline MoveOnlyFunctionInvoker(CallableType &&callable) :
        m_callable(::std::move(callable))
    {}

    MoveOnlyFunctionInvoker(const MoveOnlyFunctionInvoker &) = delete;
    MoveOnlyFunctionInvoker(MoveOnlyFunctionInvoker &&)      = delete;

    /**
     * @brief       Invoke.
     *
     * @param[in]   args        Arguments.
     *
     * @return      Return value of the function.
     */
    virtual ReturnType invoke(ArgTypes &&...args) override
    {
        return m_callable(::std::forward<ArgTypes &&>(args)...);
    }

    /**
     * @brief       Destructor.
     */
    virtual ~MoveOnlyFunctionInvoker() = default;
};

/**
 * @brief   Invoker for move-only function(\c ReturnType is \c void).
 *
 * @tparam  CallableType    Type of the callable.
 * @tparam  ArgTypes        Type of arguments.
 */
template<typename CallableType, typename... ArgTypes>
    requires(::std::is_invocable_r<void, CallableType, ArgTypes...>::value)
class MoveOnlyFunctionInvoker<CallableType, void, ArgTypes...> :
    public MoveOnlyFunctionInvokerBase<void(ArgTypes...)> {
  private:
    CallableType m_callable; ///< Callable object.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   callable        Callable object.
     */
    inline MoveOnlyFunctionInvoker(CallableType &&callable) :
        m_callable(::std::move(callable))
    {}

    MoveOnlyFunctionInvoker(const MoveOnlyFunctionInvoker &) = delete;
    MoveOnlyFunctionInvoker(MoveOnlyFunctionInvoker &&)      = delete;

    /**
     * @brief       Invoke.
     *
     * @param[in]   args        Arguments.
     *
     * @return      Return value of the function.
     */
    virtual void invoke(ArgTypes &&...args) override
    {
        m_callable(::std::forward<ArgTypes &&>(args)...);
        return;
    }

    /**
     * @brief       Destructor.
     */
    virtual ~MoveOnlyFunctionInvoker() = default;
};

/**
 * @brief   Move-only function.
 */
template<typename>
class MoveOnlyFunction;

/**
 * @brief   Move-only function(not void).
 *
 * @tparam  ReturnType  Type of return value.
 * @tparam  ArgTypes    Type of arguments.
 */
template<typename ReturnType, typename... ArgTypes>
class MoveOnlyFunction<ReturnType(ArgTypes...)> {
  private:
    ::std::unique_ptr<MoveOnlyFunctionInvokerBase<ReturnType(ArgTypes...)>>
        m_invoker; ///< Invoker.

  public:
    /**
     * @brief       Constructor.
     */
    inline MoveOnlyFunction() {}

    /**
     * @brief       Constructor.
     *
     * @param[in]   callable        Callable object.
     */
    template<typename CallableType>
    inline MoveOnlyFunction(CallableType &&callable) :
        m_invoker(
            new MoveOnlyFunctionInvoker<CallableType, ReturnType, ArgTypes...>(
                ::std::move(callable)))
    {}

    /**
     * @brief       Move constructor.
     *
     * @param[in]   func            Function to move.
     */
    inline MoveOnlyFunction(MoveOnlyFunction &&func) :
        m_invoker(::std::move(func.m_invoker))
    {}

    MoveOnlyFunction(const MoveOnlyFunction &) = delete;

    /**
     * @brief       Destructor.
     */
    ~MoveOnlyFunction() = default;

  public:
    /**
     * @brief       Check if \c *this stores a callable function target.
     *
     * @return  \c true if \c *this stores a callable function target.
     */
    inline operator bool() const
    {
        return m_invoker != nullptr;
    }

    /**
     * @brief       Call the stored function object(not void).
     *
     * @param[in]   args        Arguments.
     *
     * @return  Return value of the stored callable function object.
     */
    template<typename R = ReturnType>
        requires(! ::std::is_void<R>::value)
    inline ReturnType operator()(ArgTypes &&...args) const
    {
        if (*this) {
            return m_invoker->invoke(::std::forward<ArgTypes &&>(args)...);
        } else {
            throw std::bad_function_call();
        }
    }

    /**
     * @brief       Call the stored function object(not void).
     *
     * @param[in]   args        Arguments.
     *
     * @return  Return value of the stored callable function object.
     */
    template<typename R = ReturnType>
        requires ::std::is_void<R>::value
    inline ReturnType operator()(ArgTypes &&...args) const
    {
        if (*this) {
            m_invoker->invoke(::std::forward<ArgTypes &&>(args)...);
            return;
        } else {
            throw std::bad_function_call();
        }
    }
};

} // namespace remotePortMapper
