#include <cstdint>

#include <gtest/gtest.h>

#include <test/common/TestClass.h>

#include <common/functional/move_only_function.h>

class FunctionalTestClass {
  public:
    int m_num = 0;
    int getNum(int a)
    {
        return a + m_num;
    }
};

TEST(Functional, functional)
{
    {
        volatile bool flag = flag;

        ::remotePortMapper::MoveOnlyFunction<void()> func([&]() -> void {
            flag = true;
            return;
        });
        ASSERT_TRUE(func);
        func();
        ASSERT_TRUE(flag);
    }

    {
        ::std::unique_ptr<FunctionalTestClass> a(new FunctionalTestClass);
        a->m_num = 123;
        ::remotePortMapper::MoveOnlyFunction<int(int)> func(
            [thisPtr = ::std::move(a)](int n) mutable -> int {
                return thisPtr->getNum(456 + n);
            });
        ASSERT_TRUE(func);
        ASSERT_EQ(func(789), 123 + 456 + 789);
    }
}
