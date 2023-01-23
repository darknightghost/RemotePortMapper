#include <cstring>
#include <errno.h>

#include <common/utils/type_traits.h>

#include <common/os/error.h>

#include <common/os/memory.h>

#if defined(OS_LINUX)
    #include <sys/mman.h>
    #include <unistd.h>

#elif defined(OS_WINDOWS)
    #include <Memoryapi.h>
    #include <Windows.h>

#endif

namespace remotePortMapper {
namespace os {

/**
 * @brief   Get size of memory pages.
 */
::std::size_t pageSize()
{
#if defined(OS_LINUX)
    long sz = ::sysconf(_SC_PAGESIZE);

    return static_cast<::std::size_t>(sz);

#elif defined(OS_WINDOWS)
    SYSTEM_INFO info;
    ::GetSystemInfo(&info);

    return static_cast<::std::size_t>(info.dwPageSize);

#endif
}

/**
 * @brief       Allocate pages.
 */
Result<void *, Error> pageAlloc(::std::size_t size)
{
    if (size == 0) {
        return Result<void *, Error>::makeError(Error {
            ErrorCode::InvaledValue, "Size of pages should not be zero."});
    }
    size = alignSize(size, pageSize());
#if defined(OS_LINUX)
    void *ret = ::mmap(nullptr, size, PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (ret == nullptr) {
        int err = errno;
        return Result<void *, Error>::makeError(
            Error {ErrorCode::PageAlloc, sysError2Str(err)});

    } else {
        return Result<void *, Error>::makeOk(ret);
    }

#elif defined(OS_WINDOWS)
    void *ret = ::VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE,
                               PAGE_READWRITE);
    if (ret == nullptr) {
        DWORD err = ::GetLastError();
        return Result<void *, Error>::makeError(
            Error {ErrorCode::PageAlloc, sysError2Str(err)});

    } else {
        return Result<void *, Error>::makeOk(ret);
    }

#endif
}

/**
 * @brief       Free pages.
 */
void pageFree(void *addr, ::std::size_t size)
{
#if defined(OS_LINUX)
    ::munmap(addr, size);

#elif defined(OS_WINDOWS)
    ::VirtualFree(addr, 0, MEM_RELEASE);
    (void)(size);
#endif
}

} // namespace os
} // namespace remotePortMapper
