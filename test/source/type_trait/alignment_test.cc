#include <cstdint>

#include <gtest/gtest.h>

#include <test/common/TestClass.h>

#include <common/types/type_traits.h>

TEST(Type_trait, alignment)
{
    ASSERT_EQ(4,
              (::remotePortMapper::BufferAlignment<uint32_t, uint8_t>::value));
    ASSERT_EQ(8,
              (::remotePortMapper::BufferAlignment<uint32_t, uint64_t>::value));
    ASSERT_EQ(alignof(TestClass),
              (::remotePortMapper::BufferAlignment<TestClass, uint8_t>::value));
}
