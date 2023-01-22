#include <test/common/TestClass.h>

#include <gtest/gtest.h>

#include <common/types/result.h>

TEST(Result, class)
{
    {
        int64_t constructCount = 0;
        int64_t destructCount  = 0;
        int64_t copyCount      = 0;
        int64_t moveCount      = 0;

        {
            TestClass v(
                [&](TestClass *) -> void {
                    ++constructCount;
                },
                [&](TestClass *) -> void {
                    ++destructCount;
                },
                [&](TestClass *) -> void {
                    ++copyCount;
                },
                [&](TestClass *) -> void {
                    ++moveCount;
                });

            ::remotePortMapper::Result<TestClass, void> value;
            ASSERT_DEATH(value.value<TestClass>(), ".*");
            ASSERT_FALSE(value.good());
            ASSERT_FALSE(value.ok());
            ASSERT_FALSE(value);
            ASSERT_EQ(constructCount, 1);
            ASSERT_EQ(destructCount, 0);
            ASSERT_EQ(copyCount, 0);
            ASSERT_EQ(moveCount, 0);

            value = value.makeOk(v);
            ASSERT_TRUE(value.good());
            ASSERT_TRUE(value.ok());
            ASSERT_TRUE(value);
            ASSERT_EQ(constructCount, 3);
            ASSERT_EQ(destructCount, 1);
            ASSERT_EQ(copyCount, 1);
            ASSERT_EQ(moveCount, 1);

            // Copy elision
            auto value1 = ::remotePortMapper::Result<TestClass, void>::makeOk(
                value.value<TestClass>());
            ASSERT_TRUE(value1.good());
            ASSERT_TRUE(value1.ok());
            ASSERT_TRUE(value1);
            ASSERT_EQ(constructCount, 4);
            ASSERT_EQ(destructCount, 1);
            ASSERT_EQ(copyCount, 2);
            ASSERT_EQ(moveCount, 1);
        }

        ASSERT_EQ(destructCount, 4);
    }
    {
        int64_t constructCount = 0;
        int64_t destructCount  = 0;
        int64_t copyCount      = 0;
        int64_t moveCount      = 0;

        {
            TestClass v(
                [&](TestClass *) -> void {
                    ++constructCount;
                },
                [&](TestClass *) -> void {
                    ++destructCount;
                },
                [&](TestClass *) -> void {
                    ++copyCount;
                },
                [&](TestClass *) -> void {
                    ++moveCount;
                });

            ::remotePortMapper::Result<void, TestClass> value;
            ASSERT_DEATH(value.value<TestClass>(), ".*");
            ASSERT_FALSE(value.good());
            ASSERT_FALSE(value.ok());
            ASSERT_FALSE(value);
            ASSERT_EQ(constructCount, 1);
            ASSERT_EQ(destructCount, 0);
            ASSERT_EQ(copyCount, 0);
            ASSERT_EQ(moveCount, 0);

            value = value.makeError(v);
            ASSERT_TRUE(value.good());
            ASSERT_FALSE(value.ok());
            ASSERT_FALSE(value);
            ASSERT_EQ(constructCount, 3);
            ASSERT_EQ(destructCount, 1);
            ASSERT_EQ(copyCount, 1);
            ASSERT_EQ(moveCount, 1);

            // Copy elision
            auto value1
                = ::remotePortMapper::Result<void, TestClass>::makeError(
                    value.value<TestClass>());
            ASSERT_TRUE(value1.good());
            ASSERT_FALSE(value1.ok());
            ASSERT_FALSE(value1);
            ASSERT_EQ(constructCount, 4);
            ASSERT_EQ(destructCount, 1);
            ASSERT_EQ(copyCount, 2);
            ASSERT_EQ(moveCount, 1);
        }

        ASSERT_EQ(destructCount, 4);
    }
}
