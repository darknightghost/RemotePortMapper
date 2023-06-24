#include <cstdint>

#include <gtest/gtest.h>

#include <test/common/TestClass.h>

#include <common/types/type_traits.h>

class A {
  public:
    void voidMethod() {}
    void voidMethod(int, double) {}
    void voidMethod(int &, double &) {}
    void voidMethod(int &&, double &&) {}

    int intMethod()
    {
        return 0;
    }
    int intMethod(int, double)
    {
        return 0;
    }
    int intMethod(int &, double &)
    {
        return 0;
    }
    int intMethod(int &&, double &&)
    {
        return 0;
    }
};

class B : public A {};

class C {};

TEST(Type_trait, member_function_pointer_invocable_A)
{
    // void
    ASSERT_TRUE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                 A, decltype(static_cast<void (A::*)()>(&A::voidMethod)),
                 void>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            void>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            void, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            void, int, double>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            void, int &, double &>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            void, int &&, double &&>::value));
    ASSERT_TRUE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                 A,
                 decltype(static_cast<void (A::*)(int &&, double &&)>(
                     &A::voidMethod)),
                 void, int, double>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  A,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  void, int &, double &>::value));
    ASSERT_TRUE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                 A,
                 decltype(static_cast<void (A::*)(int &&, double &&)>(
                     &A::voidMethod)),
                 void, int &&, double &&>::value));

    // void int
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  A, decltype(static_cast<void (A::*)()>(&A::voidMethod)),
                  int>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            void>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            int, int &, double &>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            int, int &&, double &&>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  A,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  int, int, double>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  A,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  int, int &, double &>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  A,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  int, int &&, double &&>::value));

    // int
    ASSERT_TRUE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                 A, decltype(static_cast<int (A::*)()>(&A::intMethod)),
                 int>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A, decltype(static_cast<int (A::*)(int, double)>(&A::intMethod)),
            int>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A, decltype(static_cast<int (A::*)(int, double)>(&A::intMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<int (A::*)(int &, double &)>(&A::intMethod)),
            int, int, double>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<int (A::*)(int &, double &)>(&A::intMethod)),
            int, int &, double &>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<int (A::*)(int &, double &)>(&A::intMethod)),
            int, int &&, double &&>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<int (A::*)(int &&, double &&)>(&A::intMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<int (A::*)(int &&, double &&)>(&A::intMethod)),
            int, int &, double &>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            A,
            decltype(static_cast<int (A::*)(int &&, double &&)>(&A::intMethod)),
            int, int &&, double &&>::value));
}

TEST(Type_trait, member_function_pointer_invocable_B)
{
    // void
    ASSERT_TRUE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                 B, decltype(static_cast<void (A::*)()>(&A::voidMethod)),
                 void>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            void>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            void, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            void, int, double>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            void, int &, double &>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            void, int &&, double &&>::value));
    ASSERT_TRUE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                 B,
                 decltype(static_cast<void (A::*)(int &&, double &&)>(
                     &A::voidMethod)),
                 void, int, double>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  B,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  void, int &, double &>::value));
    ASSERT_TRUE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                 B,
                 decltype(static_cast<void (A::*)(int &&, double &&)>(
                     &A::voidMethod)),
                 void, int &&, double &&>::value));

    // void int
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  B, decltype(static_cast<void (A::*)()>(&A::voidMethod)),
                  int>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            void>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            int, int &, double &>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            int, int &&, double &&>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  B,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  int, int, double>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  B,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  int, int &, double &>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  B,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  int, int &&, double &&>::value));

    // int
    ASSERT_TRUE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                 B, decltype(static_cast<int (A::*)()>(&A::intMethod)),
                 int>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B, decltype(static_cast<int (A::*)(int, double)>(&A::intMethod)),
            int>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B, decltype(static_cast<int (A::*)(int, double)>(&A::intMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<int (A::*)(int &, double &)>(&A::intMethod)),
            int, int, double>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<int (A::*)(int &, double &)>(&A::intMethod)),
            int, int &, double &>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<int (A::*)(int &, double &)>(&A::intMethod)),
            int, int &&, double &&>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<int (A::*)(int &&, double &&)>(&A::intMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<int (A::*)(int &&, double &&)>(&A::intMethod)),
            int, int &, double &>::value));
    ASSERT_TRUE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            B,
            decltype(static_cast<int (A::*)(int &&, double &&)>(&A::intMethod)),
            int, int &&, double &&>::value));
}

TEST(Type_trait, member_function_pointer_invocable_C)
{
    // void
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  C, decltype(static_cast<void (A::*)()>(&A::voidMethod)),
                  void>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            void>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            void, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            void, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            void, int &, double &>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            void, int &&, double &&>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  C,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  void, int, double>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  C,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  void, int &, double &>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  C,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  void, int &&, double &&>::value));

    // void int
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  C, decltype(static_cast<void (A::*)()>(&A::voidMethod)),
                  int>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            void>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C, decltype(static_cast<void (A::*)(int, double)>(&A::voidMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            int, int &, double &>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<void (A::*)(int &, double &)>(&A::voidMethod)),
            int, int &&, double &&>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  C,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  int, int, double>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  C,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  int, int &, double &>::value));
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  C,
                  decltype(static_cast<void (A::*)(int &&, double &&)>(
                      &A::voidMethod)),
                  int, int &&, double &&>::value));

    // int
    ASSERT_FALSE((::remotePortMapper::IsMemberFunctionPointerInvocable<
                  C, decltype(static_cast<int (A::*)()>(&A::intMethod)),
                  int>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C, decltype(static_cast<int (A::*)(int, double)>(&A::intMethod)),
            int>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C, decltype(static_cast<int (A::*)(int, double)>(&A::intMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<int (A::*)(int &, double &)>(&A::intMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<int (A::*)(int &, double &)>(&A::intMethod)),
            int, int &, double &>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<int (A::*)(int &, double &)>(&A::intMethod)),
            int, int &&, double &&>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<int (A::*)(int &&, double &&)>(&A::intMethod)),
            int, int, double>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<int (A::*)(int &&, double &&)>(&A::intMethod)),
            int, int &, double &>::value));
    ASSERT_FALSE(
        (::remotePortMapper::IsMemberFunctionPointerInvocable<
            C,
            decltype(static_cast<int (A::*)(int &&, double &&)>(&A::intMethod)),
            int, int &&, double &&>::value));
}
