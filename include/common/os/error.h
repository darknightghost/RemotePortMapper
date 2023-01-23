#pragma once

#include <string>

#if defined(OS_WINDOWS)
    #include <Windows.h>

#endif

namespace remotePortMapper {
namespace os {

#if defined(OS_LINUX)
using SystemErrno = int;

#elif defined(OS_WINDOWS)
using SystemErrno = DWORD;

#endif

/**
 * @brief       Parse system error to string.
 *
 * @param[in]   err     System error code.
 *
 * @return      Error message.
 */
::std::string sysError2Str(SystemErrno err);

} // namespace os
} // namespace remotePortMapper
