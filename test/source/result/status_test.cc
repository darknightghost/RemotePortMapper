#include <gtest/gtest.h>

#include <common/types/result.h>

TEST(ResultDeathTest, status)
{
    ::remotePortMapper::Result<int, double> value;
    ASSERT_DEATH(value.value<int>(), ".*");
    ASSERT_DEATH(value.value<double>(), ".*");
    ASSERT_FALSE(value.good());
    ASSERT_FALSE(value.ok());
    ASSERT_FALSE(value);

    value = value.makeOk(100);
    ASSERT_EQ(value.value<int>(), 100);
    ASSERT_DEATH(value.value<double>(), ".*");
    ASSERT_TRUE(value.good());
    ASSERT_TRUE(value.ok());
    ASSERT_TRUE(value);

    double tmp = 999.0;
    value      = value.makeError(tmp);
    ASSERT_DEATH(value.value<int>(), ".*");
    ASSERT_EQ(value.value<double>(), tmp);
    ASSERT_TRUE(value.good());
    ASSERT_FALSE(value.ok());
    ASSERT_FALSE(value);
}
