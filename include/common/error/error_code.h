#pragma once

#include <string>

namespace remotePortMapper {

/**
 * @brief   Error code.
 */
enum class ErrorCode : int32_t {
    // Error code begin.
    Success      = 0,           ///< Success.
    InvalidValue = -1,          ///< Invalid value.
    PageAlloc    = -2,          ///< Failed to allocate memory pages.
    Unknow       = -2147483648, ///< Unknow error.
    // Error code end.
};

} // namespace remotePortMapper
