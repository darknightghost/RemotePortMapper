#pragma once

#include <cstdint>
#include <mutex>

#include <common/utils/type_traits.h>

#include <common/logger/logger.h>

namespace remotePortMapper {

/**
 * @brief   Memory pool.
 *
 * @tparam  T   Type.
 */
template<typename T>
class MemoryPool {
  private:
    /**
     * @brief   List header.
     */
    struct ListHeader {
        ListHeader *prev; ///< Previous.
        ListHeader *next; ///< Next.
    };

    struct BlockHeader;

    /**
     * @brief   Group header.
     */
    struct GroupHeader {
        ListHeader   listHeader; ///< List header.
        uint64_t     allocOrder; ///< Allocate order.
        BlockHeader *usedList;   ///< Used block list.
        BlockHeader *freeList;   ///< Free block list.
        MemoryPool  *pool;       ///< Address of the pool.
    };

    /**
     * @brief   Block header.
     */
    struct BlockHeader {
        uint64_t     magicBlockBegin; ///< Magic of the begining of thge block..
        ListHeader   listHeader;      ///< List header.
        GroupHeader *group;           ///< Group.
        BlockHeader *usedList;        ///< Used block list.
        BlockHeader *freeList;        ///< Free block list.
    };

    /// Size of group header.
    inline static constexpr ::std::size_t groupHeaderSize
        = alignSize(sizeof(GroupHeader), alignof(BlockHeader));

    /// Size of block header.
    inline static constexpr ::std::size_t blockHeaderSize
        = alignSize(sizeof(BlockHeader), alignof(T));

    /// Size of data.
    inline static constexpr ::std::size_t dataSize
        = alignSize(sizeof(T), sizeof(uint64_t));

    /// Size of block.
    inline static constexpr ::std::size_t blockSize = alignSize(
        blockHeaderSize + dataSize + sizeof(uint64_t), alignof(BlockHeader));

  private:
    ::std::mutex   m_lock;            ///< Lock.
    ::std::size_t  m_nextGroupSize;   ///< Size of next allocated group.
    GroupHeader   *m_fullGroups;      ///< Full groups.
    GroupHeader   *m_availGroups;     ///< Available groups.
    const uint64_t m_magicBlockBegin; ///< Magic of the begining of the block.
    const uint64_t m_magicDataEnd;    ///< Magic of the end of the data.

  private:
    /**
     * @brief       Constructor.
     *
     * @param[in]   firstGroupSize      Size of first group.
     */
    MemoryPool(::std::size_t firstGroupSize);

  public:
    /**
     * @brief       Destructor.
     */
    virtual ~MemoryPool();
};

} // namespace remotePortMapper
