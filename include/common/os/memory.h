#pragma once

#include <cstdint>

#include <common/error/error.h>
#include <common/types/result.h>

namespace remotePortMapper {
namespace os {

/**
 * @brief   Get size of memory pages.
 *
 * @return  Page size.
 */
::std::size_t pageSize();

/**
 * @brief       Allocate pages.
 *
 * @param[in]   size    Size to allocate.
 *
 * @return      Start address of the pages or error infomation.
 */
Result<void *, Error> pageAlloc(::std::size_t size);

/**
 * @brief       Free pages.
 *
 * @param[in]   addr    Start address of the pages.
 * @param[in]   size    Size to allocate.
 */
void pageFree(void *addr, ::std::size_t size);

} // namespace os
} // namespace remotePortMapper
