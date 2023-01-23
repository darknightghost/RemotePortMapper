#pragma once

#include <string>

#include <common/error/error_code.h>

namespace remotePortMapper {

/**
 * @brief   Error infomation.
 */
struct Error {
    ErrorCode     errCode; ///< Error code.
    ::std::string message; ///< Error message.
};

} // namespace remotePortMapper
