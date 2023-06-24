#pragma once

#include <algorithm>

#include <common/types/type_traits.h>

namespace remotePortMapper {

/**
 * @brief       Align size of the type to the value.
 */
inline constexpr ::std::size_t alignSize(::std::size_t size,
                                         ::std::size_t align)
{
    return size / align * align + (size % align ? align : 0);
}

/**
 * @brief       Implement of BufferSize.
 *
 * @tparam      Types   Other types.
 */
template<typename... Types>
struct __BufferSizeImpl;

/**
 * @brief       Implement of BufferSize.
 */
template<typename Type, typename... Types>
    requires ::std::is_void<Type>::value
struct __BufferSizeImpl<Type, Types...> {
    static inline constexpr ::std::size_t value
        = ::std::max(static_cast<size_t>(0), __BufferSizeImpl<Types...>::value);
};

/**
 * @brief       Implement of BufferSize.
 */
template<typename Type, typename... Types>
    requires ::std::is_reference<Type>::value
struct __BufferSizeImpl<Type, Types...> {
    static inline constexpr ::std::size_t value = ::std::max(
        sizeof(typename ::std::add_pointer<
               typename ::std::remove_reference<Type>::type>::type),
        __BufferSizeImpl<Types...>::value);
};

/**
 * @brief       Implement of BufferSize.
 */
template<typename Type, typename... Types>
    requires(! ::std::is_reference<Type>::value)
            && (! ::std::is_void<Type>::value)
struct __BufferSizeImpl<Type, Types...> {
    static inline constexpr ::std::size_t value
        = ::std::max(sizeof(Type), __BufferSizeImpl<Types...>::value);
};

/**
 * @brief       Implement of BufferSize.
 */
template<>
struct __BufferSizeImpl<> {
    static inline constexpr ::std::size_t value = 0;
};

/**
 * @brief       Compute the size of Buffer.
 *
 * @tparam      Types       Types.
 */
template<typename... Types>
struct BufferSize {
    /// Value.
    static inline constexpr ::std::size_t value
        = __BufferSizeImpl<Types...>::value;
};

/**
 * @brief       Calculate alignment.
 */
constexpr inline ::std::size_t __calculateAlignemnt(::std::size_t alignment1,
                                                    ::std::size_t alignment2)
{
    // Ignore 0.
    if (alignment1 == 1) {
        return alignment2;
    } else if (alignment2 == 1) {
        return alignment1;
    } else if (alignment1 == alignment2) {
        return alignment1;
    }

    // Get GCD.
    ::std::size_t m   = ::std::max(alignment1, alignment2);
    ::std::size_t gcd = ::std::min(alignment1, alignment2);
    while (m % gcd != 0) {
        if (m < gcd) {
            ::std::swap(m, gcd);
        }
        m = m - gcd;
    }

    return alignment1 * alignment2 / gcd;
}

/**
 * @brief       Implement of BufferAlignment.
 *
 * @tparam      Types   Other types.
 */
template<typename... Types>
struct __BufferAlignmentImpl;

/**
 * @brief       Implement of BufferAlignment.
 */
template<typename Type, typename... Types>
    requires ::std::is_void<Type>::value
struct __BufferAlignmentImpl<Type, Types...> {
    static inline constexpr ::std::size_t value
        = __calculateAlignemnt(1, __BufferAlignmentImpl<Types...>::value);
};

/**
 * @brief       Implement of BufferAlignment.
 */
template<typename Type, typename... Types>
    requires ::std::is_reference<Type>::value
struct __BufferAlignmentImpl<Type, Types...> {
    static inline constexpr ::std::size_t value = __calculateAlignemnt(
        alignof(typename ::std::add_pointer<
                typename ::std::remove_reference<Type>::type>::type),
        __BufferAlignmentImpl<Types...>::value);
};

/**
 * @brief       Implement of BufferAlignment.
 */
template<typename Type, typename... Types>
    requires(! ::std::is_reference<Type>::value)
            && (! ::std::is_void<Type>::value)
struct __BufferAlignmentImpl<Type, Types...> {
    static inline constexpr ::std::size_t value = __calculateAlignemnt(
        alignof(Type), __BufferAlignmentImpl<Types...>::value);
};

/**
 * @brief       Implement of BufferAlignment.
 */
template<>
struct __BufferAlignmentImpl<> {
    static inline constexpr ::std::size_t value = 1;
};

/**
 * @brief       Compute the alignment of Buffer.
 */
template<typename... Types>
struct BufferAlignment {
    /// Value.
    static inline constexpr ::std::size_t value
        = __BufferAlignmentImpl<Types...>::value;
};

/**
 * @brief       Checks if a pointer type can be invoked as a member function
 *              pointer with the given argument types.
 */
template<class ClassType,
         typename PointerType,
         typename ReturnType,
         typename... ArgTypes>
struct IsMemberFunctionPointerInvocable : public ::std::false_type {};

/**
 * @brief       Checks if a pointer type can be invoked as a member function
 *              pointer with the given argument types.
 */
template<class ClassType,
         typename PointerType,
         typename ReturnType,
         typename... ArgTypes>
    requires ::std::is_member_function_pointer<PointerType>::value
             && ::std::is_same<decltype((::std::declval<ClassType>()
                                         .*(::std::declval<PointerType>()))(
                                   ::std::declval<ArgTypes>()...)),
                               ReturnType>::value
struct IsMemberFunctionPointerInvocable<ClassType,
                                        PointerType,
                                        ReturnType,
                                        ArgTypes...> :
    public ::std::true_type {};

} // namespace remotePortMapper
