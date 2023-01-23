#pragma once

#include <cstdint>
#include <mutex>

#include <common/utils/i_create_shared_function.h>
#include <common/utils/type_traits.h>

#include <common/logger/logger.h>

namespace remotePortMapper {

/**
 * @brief   Memory pool.
 *
 * @tparam  T   Type.
 */
template<typename T>
class MemoryPool :
    virtual public ICreateSharedFunc<MemoryPool<T>>,
    virtual public ICreateSharedFunc<MemoryPool<T>, ::std::size_t> {
    CREATE_SHARED(MemoryPool<T>);
    CREATE_SHARED(MemoryPool<T>, ::std::size_t);

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
        ListHeader    listHeader; ///< List header.
        ::std::size_t size;       ///< Size.
        BlockHeader  *usedList;   ///< Used block list.
        BlockHeader  *freeList;   ///< Free block list.
        MemoryPool   *pool;       ///< Address of the pool.
    };

    /**
     * @brief   Block.
     */
    struct Block {
        uint64_t     magicBlockBegin; ///< Magic of the begining of thge block..
        ListHeader   listHeader;      ///< List header.
        GroupHeader *group;           ///< Group.
        BlockHeader *usedList;        ///< Used block list.
        BlockHeader *freeList;        ///< Free block list.
        alignas(alignof(T)) uint64_t data[sizeof(T)]; ///< Data.
        uint64_t magicBlockEnd; ///< Magic of the end of the block.
    };

    /// Size of group header.
    inline static constexpr ::std::size_t groupHeaderSize
        = alignSize(sizeof(GroupHeader), alignof(BlockHeader));

    /// Size of block.
    inline static constexpr ::std::size_t blockSize = sizeof(Block);

  private:
    ::std::mutex   m_lock;            ///< Lock.
    ::std::size_t  m_nextGroupSize;   ///< Size of next allocated group.
    GroupHeader   *m_fullGroups;      ///< Full groups.
    GroupHeader   *m_availGroups;     ///< Available groups.
    const uint64_t m_magicBlockBegin; ///< Magic of the begining of the block.
    const uint64_t m_magicBlockEnd;   ///< Magic of the end of the block.

  private:
    /**
     * @brief       Constructor.
     *
     * @param[in]   firstGroupSize      Size of first group.
     */
    MemoryPool(::std::size_t firstGroupSize = 0);

  public:
    /**
     * @brief       Destructor.
     */
    virtual ~MemoryPool();

  private:
};

} // namespace remotePortMapper
