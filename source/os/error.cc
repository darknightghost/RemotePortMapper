#include <common/os/error.h>

#if defined(OS_LINUX)
    #ifndef _GNU_SOURCE
        #define _GNU_SOURCE 1
    #endif

    #include <cstring>
    #include <errno.h>

#elif defined(OS_WINDOWS)
    #include <Winnls.h.>

#endif

namespace remotePortMapper {
namespace os {

/**
 * @brief       Parse system error to string.
 */
::std::string sysError2Str(SystemErrno err)
{
#if defined(OS_LINUX)
    char buffer[512];
    return ::std::string(::strerror_r(err, buffer, sizeof(buffer)));

#elif defined(OS_WINDOWS)
    // Parse message.
    char *message;
    if (::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, err,
                         LANG_USER_DEFAULT, reinterpret_cast<LPCSTR>(&message),
                         0, nullptr)
        == 0) {
        return "Unknown error.";
    }

    ::std::string ret(message);
    ::LocalFree(message);

    return ret;

#endif
}

} // namespace os
} // namespace remotePortMapper
