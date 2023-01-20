#pragma once

#include <concepts>
#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>

#include <common/utils/type_traits.h>

namespace remotePortMapper {

/**
 * @brief       Result type.
 *
 * @param[in]   SuccessType     Type of the value on success.
 * @param[in]   FailType      Type of the value on failed.
 */
template<typename SuccessType, typename FailType>
class Result {
  private:
    /**
     * @brief   Status.
     */
    enum class Status : uint8_t { Bad, Success, Fail };

  public:
    /**
     * @brief   Reference type of the value.
     *
     * @tparam  Type    Type.
     */
    template<typename Type>
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
                    && (! ::std::is_same<void, Type>::value)
    using ConstValueReference = typename ::std::conditional<
        ::std::is_reference<Type>::value,
        typename ::std::add_const<Type>::type,
        typename ::std::add_const<
            typename ::std::add_lvalue_reference<Type>::type>::type>::type;

  private:
    /// Size of the data.
    static inline constexpr ::std::size_t dataSize
        = BufferSize<SuccessType, FailType>::value;

    /// Alignment of the data.
    static inline constexpr ::std::size_t dataAlignment
        = BufferAlignment<SuccessType, FailType>::value;

  private:
    Status m_status;                                 ///< Status.
    alignas(dataAlignment) uint8_t m_data[dataSize]; ///< Data.

  private:
    /**
     * @brief       Constructor.
     *
     * @tparam      Type        Value type.
     * @tparam      Args        Types of the arguments of the constructor.
     *
     * @param[in]   status      Status.
     * @param[in]   args        Arguments of the constructor.
     */
    template<typename Type, typename... Args>
        requires ::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value
    inline explicit Result(Status status, Args &&...args);

  public:
    /**
     * @brief       Constructor.
     */
    inline explicit Result();

    /**
     * @brief       Copy constructor.
     *
     * @param[in]   result      Result to copy.
     */
    inline explicit Result(const Result &result);

    /**
     * @brief       Move constructor.
     *
     * @param[in]   result      Result to move.
     */
    inline explicit Result(Result &&result);

    /**
     * @brief       Destructor.
     */
    inline ~Result();

  public:
    /**
     * @brief       Check if the result is good.
     *
     * @return      \c true if good, \c false if bad.
     */
    inline bool good() const;

    /**
     * @brief       Check if the result is success.
     *
     * @return      \c true if success, \c false if not.
     */
    inline bool success() const;

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
     * @brief       Get value.
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
     * @brief       Get value.
     *
     * @tparam      Type    Value type.
     *
     * @return      Reference to  the value.
     */
    template<typename Type>
        requires(::std::is_reference<Type>::value)
                && (! ::std::is_void<Type>::value)
    inline ConstValueReference<Type> value() const;

  public:
    /**
     * @brief       Operator bool.
     *
     * @return      \c true if success, \c false if not.
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && (! ::std::is_copy_assignable<Type>::value)
    inline void copyValue(const Result &result);

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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && (! ::std::is_move_assignable<Type>::value)
                && (! ::std::is_copy_assignable<Type>::value)
    inline void moveValue(Result &&result);

  private:
    // Construct.
    /**
     * @brief       Construct the value(void).
     *
     * @tparam      Type    Value type.
     */
    template<typename Type>
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
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
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && ::std::is_constructible<Type, Args &&...>::value
    inline void construct(Type ref, Args &&...args);

  private:
    /**
     * @brief       Clear the value.
     */
    inline void clear();

    /**
     * @brief       Desctuct the value.
     *
     * @tparam      Type    Value type.
     */
    template<typename Type>
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
    inline void destruct();

    /**
     * @brief       Desctuct the value at the position(void/reference).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   ptr     Pointer to the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
                && (::std::is_void<Type>::value
                    || ::std::is_reference<Type>::value)
    inline void destructAt(typename ::std::add_pointer<Type> ptr);

    /**
     * @brief       Desctuct the value at the position(array).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   ptr     Pointer to the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && ::std::is_array<Type>::value
    inline void destructAt(typename ::std::add_pointer<Type> ptr);

    /**
     * @brief       Desctuct the value at the position(trival).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   ptr     Pointer to the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && (! ::std::is_array<Type>::value)
                && ::std::is_trivial<Type>::value
    inline void destructAt(typename ::std::add_pointer<Type> ptr);

    /**
     * @brief       Desctuct the value at the position(non-trival).
     *
     * @tparam      Type    Value type.
     *
     * @param[in]   ptr     Pointer to the value.
     */
    template<typename Type>
        requires(::std::is_same<Type, SuccessType>::value
                 || ::std::is_same<Type, FailType>::value)
                && (! ::std::is_void<Type>::value)
                && (! ::std::is_reference<Type>::value)
                && (! ::std::is_array<Type>::value)
                && (! ::std::is_trivial<Type>::value)
    inline void destructAt(typename ::std::add_pointer<Type> ptr);
};

} // namespace remotePortMapper
