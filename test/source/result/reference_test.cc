#include <gtest/gtest.h>

#include <gtest/gtest.h>

#include <common/types/result.h>

TEST(Result, reference)
{
    {
        int                                     a = 100;
        ::remotePortMapper::Result<int &, void> value;
        ASSERT_DEATH(value.value<int &>(), ".*");
        ASSERT_FALSE(value.good());
        ASSERT_FALSE(value.ok());
        ASSERT_FALSE(value);

        value = value.makeOk(a);
        ASSERT_TRUE(value.good());
        ASSERT_TRUE(value.ok());
        ASSERT_TRUE(value);
        ASSERT_EQ(value.value<int &>(), 100);
        ASSERT_TRUE(
            ::std::is_lvalue_reference<decltype(value.value<int &>())>::value);

        value.value<int &>() = 200;
        ASSERT_EQ(a, 200);
    }
    {
        int                                      a = 100;
        ::remotePortMapper::Result<int &&, void> value;
        ASSERT_DEATH(value.value<int &&>(), ".*");
        ASSERT_FALSE(value.good());
        ASSERT_FALSE(value.ok());
        ASSERT_FALSE(value);

        value = value.makeOk(::std::move(a));
        ASSERT_TRUE(value.good());
        ASSERT_TRUE(value.ok());
        ASSERT_TRUE(value);
        ASSERT_EQ(value.value<int &&>(), 100);
        ASSERT_TRUE(
            ::std::is_rvalue_reference<decltype(value.value<int &&>())>::value);
    }
}
