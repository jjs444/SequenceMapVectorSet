
#include "../src/VectorSet.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>

TEST(VectorSetTest, DefaultConstructor) {
    VectorSet<int> s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0);
}

TEST(VectorSetTest, InitializerListDeduplicatesAndSorts) {
    VectorSet<int> s = { 4, 2, 2, 1, 3 };
    std::vector<int> expected = { 1, 2, 3, 4 };
    EXPECT_EQ(std::vector<int>(s.begin(), s.end()), expected);
}

TEST(VectorSetTest, InsertUniqueSorted) {
    VectorSet<int> s;
    s.insert(3);
    s.insert(1);
    s.insert(2);
    s.insert(3); // duplicate

    std::vector<int> expected = { 1, 2, 3 };
    EXPECT_EQ(std::vector<int>(s.begin(), s.end()), expected);
    EXPECT_EQ(s.size(), 3);
}

TEST(VectorSetTest, EraseByValue) {
    VectorSet<int> s = { 1, 2, 3, 4 };
    EXPECT_EQ(s.erase(3), 1);
    EXPECT_EQ(s.count(3), 0);
    EXPECT_EQ(s.erase(99), 0);
}

TEST(VectorSetTest, EraseByIterator) {
    VectorSet<int> s = { 1, 2, 3 };
    auto it = s.find(2);
    s.erase(it);
    EXPECT_EQ(s.count(2), 0);
}

TEST(VectorSetTest, EraseRange) {
    VectorSet<int> s = { 1, 2, 3, 4, 5 };
    auto it1 = s.find(2);
    auto it2 = s.find(4);
    s.erase(it1, it2);
    std::vector<int> expected = { 1, 4, 5 };
    EXPECT_EQ(std::vector<int>(s.begin(), s.end()), expected);
}

TEST(VectorSetTest, ContainsFindCount) {
    VectorSet<int> s = { 10, 20, 30 };
    EXPECT_TRUE(s.contains(20));
    EXPECT_FALSE(s.contains(25));
    EXPECT_EQ(s.count(10), 1);
    EXPECT_EQ(s.count(99), 0);

    auto it = s.find(30);
    EXPECT_NE(it, s.end());
    EXPECT_EQ(*it, 30);
}

TEST(VectorSetTest, LowerAndUpperBound) {
    VectorSet<int> s = { 10, 20, 30, 40 };

    auto lb = s.lower_bound(25);
    EXPECT_EQ(*lb, 30);

    auto ub = s.upper_bound(30);
    EXPECT_EQ(*ub, 40);
}

TEST(VectorSetTest, EqualRangeTest) {
    VectorSet<int> s = { 5, 10, 15 };

    auto [first, last] = s.equal_range(10);
    ASSERT_NE(first, last);
    EXPECT_EQ(*first, 10);

    auto [nf, nl] = s.equal_range(12);
    EXPECT_EQ(nf, nl);
}

TEST(VectorSetTest, SupportsMoveOnlyTypes) {
    VectorSet<std::unique_ptr<int>, std::function<bool(const std::unique_ptr<int>&, const std::unique_ptr<int>&)>> s(
        [](const auto& a, const auto& b) { return *a < *b; });

    s.insert(std::make_unique<int>(5));
    s.insert(std::make_unique<int>(2));
    s.insert(std::make_unique<int>(5)); // Duplicate value

    std::vector<int> values;
    for (const auto& ptr : s) {
        values.push_back(*ptr);
    }

    EXPECT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], 2);
    EXPECT_EQ(values[1], 5);
}

TEST(VectorSetTest, IteratorTraversal) {
    VectorSet<int> s = { 1, 2, 3 };
    std::vector<int> forward(s.begin(), s.end());
    std::vector<int> reverse(s.rbegin(), s.rend());

    EXPECT_EQ(forward, (std::vector<int>{1, 2, 3}));
    EXPECT_EQ(reverse, (std::vector<int>{3, 2, 1}));
}

TEST(VectorSetTest, Clear) {
    VectorSet<int> s = { 1, 2, 3 };
    s.clear();
    EXPECT_TRUE(s.empty());
}

TEST(VectorSetTest, SwapWorksCorrectly) {
    VectorSet<int> a = { 1, 2 };
    VectorSet<int> b = { 3, 4 };

    swap(a, b);

    EXPECT_EQ(std::vector<int>(a.begin(), a.end()), (std::vector<int>{3, 4}));
    EXPECT_EQ(std::vector<int>(b.begin(), b.end()), (std::vector<int>{1, 2}));
}

TEST(VectorSetTest, CustomComparatorDescendingOrder) {
    VectorSet<int, std::greater<>> s = { 5, 2, 8, 1 };
    std::vector<int> expected = { 8, 5, 2, 1 };
    EXPECT_EQ(std::vector<int>(s.begin(), s.end()), expected);
}