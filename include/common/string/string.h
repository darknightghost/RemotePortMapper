#pragma once

#include <memory>
#include <string>
#include <type_traits>

#if defined(OS_WINDOWS)
    #include <windows.h>

#endif

#include <common/types/result.h>

namespace remotePortMapper {
#if defined(OS_WINDOWS)

// Type of windows string.
using WindowsString
    = ::std::unique_ptr<typename ::std::remove_pointer<LPWSTR>::type[]>;

/**
 * @brief       Convert local string to windows string.
 *
 * @param[in]   localString     Local string.
 *
 * @return      Windows string or error.
 */
Result<WindowsString, Error> toWindowsString(const ::std::string &localString);

/**
 * @brief       Convert windows string to local string.
 *
 * @param[in]   windowsString   Windows string.
 *
 * @return      Local string.
 */
::std::string toLocalString(const WindowsString &localString);

#endif
} // namespace remotePortMapper
