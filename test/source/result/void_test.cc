#include <gtest/gtest.h>

#include <common/types/result.h>

TEST(Result, status)
{
    {
        ::remotePortMapper::Result<void, double> value;
        ASSERT_DEATH(value.value<double>(), ".*");
        ASSERT_FALSE(value.good());
        ASSERT_FALSE(value.ok());
        ASSERT_FALSE(value);

        value = value.makeOk();
        ASSERT_DEATH(value.value<double>(), ".*");
        ASSERT_TRUE(value.good());
        ASSERT_TRUE(value.ok());
        ASSERT_TRUE(value);

        double tmp = 999.0;
        value      = value.makeError(tmp);
        ASSERT_EQ(value.value<double>(), tmp);
        ASSERT_TRUE(value.good());
        ASSERT_FALSE(value.ok());
        ASSERT_FALSE(value);
    }

    {
        ::remotePortMapper::Result<int, void> value;
        ASSERT_DEATH(value.value<int>(), ".*");
        ASSERT_FALSE(value.good());
        ASSERT_FALSE(value.ok());
        ASSERT_FALSE(value);

        value = value.makeOk(100);
        ASSERT_EQ(value.value<int>(), 100);
        ASSERT_TRUE(value.good());
        ASSERT_TRUE(value.ok());
        ASSERT_TRUE(value);

        value = value.makeError();
        ASSERT_DEATH(value.value<int>(), ".*");
        ASSERT_TRUE(value.good());
        ASSERT_FALSE(value.ok());
        ASSERT_FALSE(value);
    }
}
