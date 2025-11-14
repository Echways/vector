#include "gtest/gtest.h"
#include "../include/vector.h"
#include <stdexcept>

struct TestStruct {
    int value;
    static int constructions;
    static int destructions;

    TestStruct(int v = 0) : value(v) { ++constructions; }
    TestStruct(const TestStruct& other) : value(other.value) { ++constructions; }
    ~TestStruct() { ++destructions; }
};

int TestStruct::constructions = 0;
int TestStruct::destructions  = 0;


TEST(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
}

TEST(VectorTest, SizeConstructor) {
    Vector<int> v(5);
    EXPECT_EQ(v.size(), 5u);
    EXPECT_GE(v.capacity(), 5u);

    v[0] = 42;
    EXPECT_EQ(v[0], 42);
}


TEST(VectorTest, ReserveDoesNotShrink) {
    Vector<int> v(5);
    size_t oldCapacity = v.capacity();
    v.reserve(2);           
    EXPECT_EQ(v.capacity(), oldCapacity);
}

TEST(VectorTest, ReserveIncreasesCapacityAndKeepsElements) {
    Vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    size_t oldCapacity = v.capacity();
    v.reserve(oldCapacity + 10);

    EXPECT_GE(v.capacity(), oldCapacity + 10);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}


TEST(VectorTest, ResizeGrowWithDefaultValues) {
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);

    v.resize(5);

    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);

    EXPECT_EQ(v[2], int());
    EXPECT_EQ(v[3], int());
    EXPECT_EQ(v[4], int());
}

TEST(VectorTest, ResizeGrowWithGivenValue) {
    Vector<int> v;
    v.resize(3, 7);

    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 7);
    EXPECT_EQ(v[1], 7);
    EXPECT_EQ(v[2], 7);
}

TEST(VectorTest, ResizeShrink) {
    Vector<int> v;
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }

    v.resize(3);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(v[2], 2);
}

TEST(VectorTest, PushBackIncreasesSize) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0u);

    v.push_back(5);
    EXPECT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], 5);

    v.push_back(7);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[1], 7);
}

TEST(VectorTest, PushBackHandlesReallocation) {
    Vector<int> v;

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    EXPECT_EQ(v.size(), 100u);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(v[i], i);
    }
}

TEST(VectorTest, PopBackOnNonEmpty) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);

    bool ok1 = v.pop_back();
    EXPECT_TRUE(ok1);
    EXPECT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], 1);

    bool ok2 = v.pop_back();
    EXPECT_TRUE(ok2);
    EXPECT_EQ(v.size(), 0u);
}

TEST(VectorTest, PopBackOnEmpty) {
    Vector<int> v;
    bool ok = v.pop_back();
    EXPECT_FALSE(ok);
    EXPECT_EQ(v.size(), 0u);
}


TEST(VectorTest, IndexOperatorReadWrite) {
    Vector<int> v(3);
    v[0] = 10;
    v[1] = 20;
    v[2] = 30;

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorTest, AtThrowsOnOutOfRange) {
    Vector<int> v;
    v.push_back(1);

    EXPECT_NO_THROW(v.at(0));
    EXPECT_THROW(v.at(1), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}


TEST(VectorTest, NonTrivialTypeLifetimeSimple) {
    TestStruct::constructions = 0;
    TestStruct::destructions  = 0;

    {
        Vector<TestStruct> v;
        v.push_back(TestStruct(1));
        v.push_back(TestStruct(2));
        v.push_back(TestStruct(3));

        EXPECT_EQ(v.size(), 3u);
        EXPECT_EQ(v[0].value, 1);
        EXPECT_EQ(v[1].value, 2);
        EXPECT_EQ(v[2].value, 3);
    }

    EXPECT_EQ(TestStruct::constructions, TestStruct::destructions);
}

TEST(VectorTest, NonTrivialTypeLifetimeWithReserve) {
    TestStruct::constructions = 0;
    TestStruct::destructions  = 0;

    {
        Vector<TestStruct> v;
        v.reserve(3);
        v.push_back(TestStruct(7));
        v.push_back(TestStruct(8));

        EXPECT_EQ(v.size(), 2u);
        EXPECT_EQ(v[0].value, 7);
        EXPECT_EQ(v[1].value, 8);
    }

    EXPECT_EQ(TestStruct::constructions, TestStruct::destructions);
}
