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

} // namespace remotePortMapper

#include <common/utils/type_traits.hpp>
