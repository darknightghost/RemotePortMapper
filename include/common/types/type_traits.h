#pragma once

#include <type_traits>

namespace remotePortMapper {

/**
 * @brief       Align size of the type to the value.
 *
 * @param       size    Size to align.
 * @param       align   Value to align the size.
 */
inline constexpr ::std::size_t alignSize(::std::size_t size,
                                         ::std::size_t align);

/**
 * @brief       Compute the size of buffer.
 *
 * @tparam      Types       Types.
 */
template<typename... Types>
struct BufferSize;

/**
 * @brief       Compute the alignment of buffer.
 *
 * @tparam      Types       Types.
 */
template<typename... Types>
struct BufferAlignment;

/**
 * @brief       Checks if a pointer type can be invoked as a member function
 *              pointer with the given argument types.
 *
 * @tparam      ClassType       Type of the class.
 * @tparam      PointerType     Type of the pointer.
 * @tparam      ReturnType      Type of the return value.
 * @tparam      ArgTypes        Types of the arguments.
 */
template<class ClassType,
         typename PointerType,
         typename ReturnType,
         typename... ArgTypes>
struct IsMemberFunctionPointerInvocable;

} // namespace remotePortMapper

#include <common/types/type_traits.hpp>
