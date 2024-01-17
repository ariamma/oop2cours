#include <gtest/gtest.h>
#include "BitArray.h"
#include "BitArray.cpp"

TEST(BitArray, CopyConstructorTest) {
    auto b = BitArray(200);
    auto a = BitArray(b);
    EXPECT_TRUE(a == b);
}

TEST(BitArray, Swap) {
    auto b = BitArray(10);
    auto a = ~b;
    BitArray old_b;
    old_b = b;
    a.swap(b);
    EXPECT_TRUE(~b == old_b);
}

TEST(BitArray, AnyNone) {
    auto b = BitArray(10, false);
    EXPECT_TRUE(b.none());
    b.set(9, true);
    EXPECT_TRUE(b.any());
    EXPECT_TRUE(!b.none());
}

TEST(BitArray, ResizeClear) {
    auto b = BitArray(100);
    b.resize(20);
    EXPECT_EQ(b.size, 20);
    b.clear();
    EXPECT_EQ(b.size, 0);
}

TEST(BitArray, BitwiseOperations) {
    BitArray b = BitArray(8);
    BitArray a = ~b;
    BitArray And = a & b;
    EXPECT_TRUE(And.to_string() == b.to_string());
    auto Or = a | b;
    EXPECT_TRUE(Or.to_string() == a.to_string());
    auto Xor = a ^ b;
    EXPECT_TRUE(Or == Xor);
    auto Not = ~b;
    EXPECT_TRUE(a == Not);
}

TEST(BitArray, ToString) {
    BitArray b(8);
    BitArray a(8);
    a.set(0, true);
    b = a >> 2;
    EXPECT_EQ(b.to_string(), "00000100");
    b = a << 2;
    EXPECT_TRUE(b.none());
    b = a >> 0;
    EXPECT_EQ(a, b);
}

TEST(BitArray, Count) {
    auto a = BitArray(100);
    for (int i = 0; i < a.size; i++) {
        if (i % 10 == 0) {
            a.set(i, true);
        }
    }
    EXPECT_EQ(a.count(), 10);
}



TEST(BitArray, EmptyReset) {
    BitArray a;
    EXPECT_TRUE(a.empty());
    a.resize(32);
    EXPECT_EQ(a.size, 32);
    a.reset();
    EXPECT_EQ(a.count(), 0);
}

TEST(BitArray, Exceptions) {
    BitArray a(10);
    BitArray b(100);
    EXPECT_ANY_THROW(b & a);
    EXPECT_ANY_THROW(b | a);
    EXPECT_ANY_THROW(b ^ a);
    EXPECT_ANY_THROW(b.resize(-10));
    EXPECT_ANY_THROW(b[-1]);
    EXPECT_ANY_THROW(a.set(-2));
    EXPECT_ANY_THROW(a[-2]);
    EXPECT_ANY_THROW(a[100]);
    a.clear();
    EXPECT_ANY_THROW(a[0]);
}

TEST(BitArray, PushTest) {
    BitArray b;
    for (int i = 0; i < 10000; i++) {
        b.push_back(true);
    }
    EXPECT_EQ(b.count(), 10000);
    b.resize(0);
    for (int i = 0; i < 100; i++) {
        b.push_back(true);
    }
    EXPECT_EQ(b.count(), 100);
    b.clear();
    for (int i = 0; i < 10; i++) {
        b.push_back(true);
    }
    EXPECT_EQ(b.count(), 10);
}

TEST(BitArray, Appeal) {
    BitArray b(8, false);
    b.set(2);
    EXPECT_TRUE(b[2]);
}
