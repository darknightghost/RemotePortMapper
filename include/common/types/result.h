#pragma once

#include <concepts>
#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>

#include <common/types/type_traits.h>

namespace remotePortMapper {

/**
 * @brief       Result type.
 *
 * @param[in]   OkType     Type of the value on ok.
 * @param[in]   ErrorType      Type of the value on erred.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
class Result {
  private:
    /**
     * @brief   Status.
     */
    enum class Status : uint8_t { Bad, Ok, Error };

  public:
    /**
     * @brief   Reference type of the value.
     *
     * @tparam  Type    Type.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                    && (! ::std::is_same<void, Type>::value)
    using ValueReference = typename ::std::conditional<
        ::std::is_reference<Type>::value,
        Type,
        typename ::std::add_lvalue_reference<Type>::type>::type;

    /**
     * @brief   Constant reference type of the value.
     *
     * @tparam  Type    Type.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                    && (! ::std::is_same<void, Type>::value)
    using ConstValueReference = typename ::std::conditional<
        ::std::is_reference<Type>::value,
        typename ::std::add_const<Type>::type,
        typename ::std::add_lvalue_reference<
            typename ::std::add_const<Type>::type>::type>::type;

  private:
    /// Size of the data.
    static inline constexpr ::std::size_t dataSize
        = BufferSize<OkType, ErrorType>::value;

    /// Alignment of the data.
    static inline constexpr ::std::size_t dataAlignment
        = BufferAlignment<OkType, ErrorType>::value;

  private:
    Status m_status;                                 ///< Status.
    alignas(dataAlignment) uint8_t m_data[dataSize]; ///< Data.

  public:
    /**
     * @brief       Constructor.
     */
    inline Result();

    /**
     * @brief       Copy constructor.
     *
     * @param[in]   result      Result to copy.
     */
    inline Result(const Result &result);

    /**
     * @brief       Move constructor.
     *
     * @param[in]   result      Result to move.
     */
    inline Result(Result &&result);

    /**
     * @brief       Destructor.
     */
    inline ~Result();

  public:
    /**
     * @brief       Make an ok result.
     *
     * @tparam      Args        Types of the arguments of the constructor.
     *
     * @param[in]   args        Arguments of the constructor of the value.
     *
     * @return      Result.
     */
    template<typename... Args>
    static Result makeOk(Args &&...args);

    /**
     * @brief       Make a error result.
     *
     * @tparam      Args        Types of the arguments of the constructor.
     *
     * @param[in]   args        Arguments of the constructor of the value.
     *
     * @return      Result.
     */
    template<typename... Args>
    static Result makeError(Args &&...args);

  public:
    /**
     * @brief       Check if the result is good.
     *
     * @return      \c true if good, \c false if bad.
     */
    inline bool good() const;

    /**
     * @brief       Check if the result is ok.
     *
     * @return      \c true if ok, \c false if not.
     */
    inline bool ok() const;

    /**
     * @brief       Get value.
     *
     * @tparam      Type    Value type.
     *
     * @return      Reference to  the value.
     */
    template<typename Type>
        requires(! ::std::is_reference<Type>::value)
                && (! ::std::is_void<Type>::value)
    inline ValueReference<Type> value();

    /**
     * @brief       Get value(reference).
     *
     * @tparam      Type    Value type.
     *
     * @return      Reference to  the value.
     */
    template<typename Type>
        requires(::std::is_reference<Type>::value)
                && (! ::std::is_void<Type>::value)
    inline ValueReference<Type> value();

    /**
     * @brief       Get value.
     *
     * @tparam      Type    Value type.
     *
     * @return      Reference to  the value.
     */
    template<typename Type>
        requires(! ::std::is_reference<Type>::value)
                && (! ::std::is_void<Type>::value)
    inline ConstValueReference<Type> value() const;

    /**
     * @brief       Get value(reference).
     *
     * @tparam      Type    Value type.
     *
     * @return      Reference to  the value.
     */
    template<typename Type>
        requires(::std::is_reference<Type>::value)
                && (! ::std::is_void<Type>::value)
    inline ConstValueReference<Type> value() const;

    /**
     * @brief       Clear the value.
     */
    inline void clear();

  public:
    /**
     * @brief       Operator bool.
     *
     * @return      \c true if ok, \c false if not.
     */
    inline operator bool() const;

    /**
     * @brief       Operator=.
     *
     * @param[in]   result      Result to copy.
     *
     * @return      *this.
     */
    inline Result &operator=(const Result &result);

    /**
     * @brief       Operator=.
     *
     * @param[in]   result      Result to move.
     *
     * @return      *this.
     */
    inline Result &operator=(Result &&result);

  private:
    // Copy.
    /**
     * @brief       Copy value.
     *
     * @param[in]   result      Result to copy.
     */
    inline void copy(const Result &result);

    /**
     * @brief       Copy value(void).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to copy the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && ::std::is_void<Type>::value
    inline void copyValue(const Result &result);

    /**
     * @brief       Copy value(reference).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to copy the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
                && ::std::is_reference<Type>::value
    inline void copyValue(const Result &result);

    /**
     * @brief       Copy value(copy assignable).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to copy the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && ::std::is_copy_assignable<Type>::value
    inline void copyValue(const Result &result);

    /**
     * @brief       Copy value(else).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to copy the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && (! ::std::is_copy_assignable<Type>::value)
    inline void copyValue(const Result &result);

    /**
     * @brief       Copy construct value(void).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to copy the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && ::std::is_void<Type>::value
    inline void copyConstructValue(const Result &result);

    /**
     * @brief       Copy construct value(else).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to copy the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
    inline void copyConstructValue(const Result &result);

  private:
    // Move.
    /**
     * @brief       Move value.
     *
     * @param[in]   result      Result to move.
     */
    inline void move(Result &&result);

    /**
     * @brief       Move value(void).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to move the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && ::std::is_void<Type>::value
    inline void moveValue(Result &&result);

    /**
     * @brief       Move value(reference).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to move the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
                && ::std::is_reference<Type>::value
    inline void moveValue(Result &&result);

    /**
     * @brief       Move value(move assignable).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to move the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && ::std::is_move_assignable<Type>::value
    inline void moveValue(Result &&result);

    /**
     * @brief       Move value(copy assignable).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to move the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && (! ::std::is_move_assignable<Type>::value)
                && ::std::is_copy_assignable<Type>::value
    inline void moveValue(Result &&result);

    /**
     * @brief       Move value(else).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to move the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && (! ::std::is_move_assignable<Type>::value)
                && (! ::std::is_copy_assignable<Type>::value)
    inline void moveValue(Result &&result);

    /**
     * @brief       Move construct value(void).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to move the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && ::std::is_void<Type>::value
    inline void moveConstructValue(Result &&result);

    /**
     * @brief       Move construct value(else).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   result  Result to move the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
    inline void moveConstructValue(Result &&result);

  private:
    // Construct.
    /**
     * @brief       Construct the value(void).
     *
     * @tparam      Type    Value type.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && ::std::is_void<Type>::value
    inline void construct();

    /**
     * @brief       Construct the value(reference).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   ref     Reference to copy.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
                && ::std::is_reference<Type>::value
    inline void construct(Type ref);

    /**
     * @brief       Construct the value(others).
     *
     * @tparam      Type    Value type.
     * @tparam      Args    Types of the arguments
     *
     * @param[in]   args    Arguments of the constructor.
     */
    template<typename Type, typename... Args>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && ::std::is_constructible<Type, Args &&...>::value
    inline void construct(Args &&...args);

  private:
    /**
     * @brief       Desctuct the value.
     *
     * @tparam      Type    Value type.
     */
    template<typename Type>
        requires(::std::is_same<Type, OkType>::value
                 || ::std::is_same<Type, ErrorType>::value)
    inline void destruct();

    /**
     * @brief       Desctuct the value at the position(trival).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   ptr     Pointer to the value.
     */
    template<typename Type>
        requires ::std::is_void<Type>::value || ::std::is_reference<Type>::value
                 || ((! ::std::is_array<Type>::value)
                     && ::std::is_trivial<Type>::value)
    inline void destructAt(typename ::std::add_pointer<Type>::type ptr);

    /**
     * @brief       Desctuct the value at the position(non-trival).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   ptr     Pointer to the value.
     */
    template<typename Type>
        requires(! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && (! ::std::is_array<Type>::value)
                && (! ::std::is_trivial<Type>::value)
    inline void destructAt(typename ::std::add_pointer<Type>::type ptr);
};

} // namespace remotePortMapper

#include <common/types/result.hpp>
