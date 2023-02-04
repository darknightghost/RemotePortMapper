#include <cstdint>
#include <future>

#include <gtest/gtest.h>

#include <common/utils/flags.h>

enum class TestFlag { A = 0x01, B = 0x02, C = 0x04 };

FLAGS_TYPE(TestFlag);

TEST(Flags, all)
{
    {
        TestFlags f;
        ASSERT_FALSE(f & TestFlag::A);
        ASSERT_FALSE(f & TestFlag::B);
        ASSERT_FALSE(f & TestFlag::C);
        ASSERT_EQ(f.value(), 0);
    }
    {
        TestFlags f(TestFlag::A);
        ASSERT_EQ(f.value(), static_cast<int>(TestFlag::A));
    }
    {
        TestFlags tmp(TestFlag::B);
        TestFlags f = tmp;
        ASSERT_EQ(f.value(), static_cast<int>(TestFlag::B));
    }
    {
        TestFlags f(TestFlag::A);
        ASSERT_TRUE(f.mask() & static_cast<int>(TestFlag::A));
        ASSERT_FALSE(f.mask() & static_cast<int>(TestFlag::B));
        ASSERT_FALSE(f.mask() & static_cast<int>(TestFlag::C));
    }
    {
        TestFlags tmp(TestFlag::B);
        TestFlags f;
        f = tmp;
        ASSERT_EQ(f.value(), static_cast<int>(TestFlag::B));

        f = TestFlag::A;
        ASSERT_EQ(f.value(), static_cast<int>(TestFlag::A));
    }
    {
        TestFlags f(TestFlag::A);

        ASSERT_TRUE(f == TestFlags(TestFlag::A));
        ASSERT_TRUE(f == TestFlag::A);

        ASSERT_TRUE(f != TestFlags(TestFlag::B));
        ASSERT_TRUE(f != TestFlag::B);
    }

    {
        TestFlags f(TestFlag::A);

        ASSERT_TRUE((f & TestFlag::A).value() == static_cast<int>(TestFlag::A));
        ASSERT_TRUE((f & TestFlags(TestFlag::B)).value()
                    == static_cast<int>(TestFlag::A));

        ASSERT_TRUE((f & TestFlag::B).value() == 0);
        ASSERT_TRUE((f & TestFlags(TestFlag::B)).value() == 0);
    }

    {
        TestFlags f(TestFlag::A);

        ASSERT_TRUE((f | TestFlag::B).value() == 0x03);
        ASSERT_TRUE((f | TestFlags(TestFlag::B)).value() == 0x03);

        ASSERT_TRUE(f.value() == static_cast<int>(TestFlag::A));
    }

    {
        TestFlags f(TestFlag::A);

        ASSERT_TRUE(&(f &= TestFlag::A) == &f);
        ASSERT_TRUE(f.value() == static_cast<int>(TestFlag::A));

        f = TestFlag::A;
        ASSERT_TRUE(&(f &= TestFlags(TestFlag::A)) == &f);
        ASSERT_TRUE(f.value() == static_cast<int>(TestFlag::A));

        f = TestFlag::A;
        ASSERT_TRUE(&(f &= TestFlag::B) == &f);
        ASSERT_TRUE(f.value() == 0);

        f = TestFlag::A;
        ASSERT_TRUE(&(f &= TestFlags(TestFlag::B)) == &f);
        ASSERT_TRUE(f.value() == 0);
    }
    {
        TestFlags f(TestFlag::A);
        ASSERT_TRUE(&(f |= TestFlag::B) == &f);
        ASSERT_TRUE(f.value() == 0x03);

        f = TestFlag::A;
        ASSERT_TRUE(&(f |= TestFlags(TestFlag::B)) == &f);
        ASSERT_TRUE((f | TestFlags(TestFlag::B)).value() == 0x03);
    }
}
