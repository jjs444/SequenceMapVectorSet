#include <gtest/gtest.h>
#include <algorithm>
#include "../src/SequenceMap.hpp"

class SequenceMapFunctionalTest : public ::testing::Test {
protected:
    SequenceMap<std::string, int> map;

    void SetUp() override {
        map.emplace_back("one", 1);
        map.emplace_back("two", 2);
        map.emplace_back("three", 3);
    }
};

TEST_F(SequenceMapFunctionalTest, SizeAndEmpty) {
    EXPECT_EQ(map.size(), 3);
    EXPECT_FALSE(map.empty());
}

TEST_F(SequenceMapFunctionalTest, AtIdxReturnsCorrectValue) {
    EXPECT_EQ(map.atIdx(0).first, "one");
    EXPECT_EQ(map.atIdx(1).second, 2);
}

TEST_F(SequenceMapFunctionalTest, AtIdxThrowsOnOutOfBounds) {
    EXPECT_THROW(map.atIdx(3), std::out_of_range);
}

TEST_F(SequenceMapFunctionalTest, AtKeyReturnsCorrectValue) {
    EXPECT_EQ(map.atKey("two"), 2);
}

TEST_F(SequenceMapFunctionalTest, AtKeyThrowsIfKeyMissing) {
    EXPECT_THROW(map.atKey("missing"), std::out_of_range);
}

TEST_F(SequenceMapFunctionalTest, OperatorSquareBracketsIndex) {
    EXPECT_EQ(map[0].first, "one");
    EXPECT_EQ(map[2].second, 3);
}

TEST_F(SequenceMapFunctionalTest, OperatorCallInsertsIfMissing) {
    EXPECT_EQ(map.size(), 3);
    map("four") = 4;
    EXPECT_EQ(map.size(), 4);
    EXPECT_EQ(map("four"), 4);
}

TEST_F(SequenceMapFunctionalTest, PushBackRejectsDuplicateKeys) {
    auto [it, inserted] = map.push_back({ "one", 10 });
    EXPECT_FALSE(inserted);
    EXPECT_EQ(it->second, 1); // Original value remains
}

TEST_F(SequenceMapFunctionalTest, PushBackAddsNewElement) {
    auto [it, inserted] = map.push_back({ "four", 4 });
    EXPECT_TRUE(inserted);
    EXPECT_EQ(it->first, "four");
    EXPECT_EQ(map.size(), 4);
}

TEST_F(SequenceMapFunctionalTest, EraseByIterator) {
    auto it = map.begin();
    ++it; // points to "two"
    map.erase(it);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(map.atIdx(1).first, "three");
    EXPECT_THROW(map.atKey("two"), std::out_of_range);
}

TEST_F(SequenceMapFunctionalTest, EraseByKey) {
    size_t count = map.erase("two");
    EXPECT_EQ(count, 1);
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(map.count("two"), 0);
}

TEST_F(SequenceMapFunctionalTest, EraseByRange) {
    map.erase(map.begin(), std::next(map.begin(), 2));
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(map.atIdx(0).first, "three");
}

TEST_F(SequenceMapFunctionalTest, PopBack) {
    map.pop_back();
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(map.back().first, "two");

    map.pop_back();
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(map.back().first, "one");
}

TEST_F(SequenceMapFunctionalTest, BackFront) {
    EXPECT_EQ(map.back().first, "three");
    EXPECT_EQ(map.back().second, 3);

    EXPECT_EQ(map.front().first, "one");
    EXPECT_EQ(map.front().second, 1);
}

TEST_F(SequenceMapFunctionalTest, ClearEmptiesContainer) {
    map.clear();
    EXPECT_TRUE(map.empty());
    EXPECT_EQ(map.size(), 0);
}

TEST_F(SequenceMapFunctionalTest, IterationOrderIsInsertionOrder) {
    std::vector<std::string> keys;
    for (const auto& kv : map) {
        keys.push_back(kv.first);
    }
    EXPECT_EQ(keys, (std::vector<std::string>{"one", "two", "three"}));
}

TEST_F(SequenceMapFunctionalTest, CopyConstructorCreatesDeepCopy) {
    SequenceMap<std::string, int> copy(map);
    EXPECT_EQ(copy.size(), map.size());
    EXPECT_EQ(copy.atKey("two"), 2);
    copy("two") = 20;
    EXPECT_NE(copy.atKey("two"), map.atKey("two"));
}

TEST(SequenceMapFunctionalTest_Move, MoveConstructorTransfersOwnership) {
    SequenceMap<int, std::unique_ptr<int>> map1;
    map1.push_back({ 1, std::make_unique<int>(1) });

    SequenceMap<int, std::unique_ptr<int>> moved(std::move(map1));
    EXPECT_EQ(moved.size(), 1);
    EXPECT_EQ(*moved.atKey(1), 1);
}

TEST_F(SequenceMapFunctionalTest, FindWorksAsExpected) {
    auto it = map.find("two");
    ASSERT_NE(it, map.end());
    EXPECT_EQ(it->second, 2);

    auto not_found = map.find("missing");
    EXPECT_EQ(not_found, map.end());
}


TEST_F(SequenceMapFunctionalTest, IteratorTraversalMatchesIndexAccess) {
    size_t i = 0;
    for (auto it = map.begin(); it != map.end(); ++it, ++i) {
        EXPECT_EQ(it->first, map[i].first);
        EXPECT_EQ(it->second, map[i].second);
    }
}

TEST_F(SequenceMapFunctionalTest, ConstIteratorTraversal) {
    const auto& const_map = map;
    size_t i = 0;
    for (auto it = const_map.cbegin(); it != const_map.cend(); ++it, ++i) {
        EXPECT_EQ(it->first, const_map[i].first);
        EXPECT_EQ(it->second, const_map[i].second);
    }
}

TEST_F(SequenceMapFunctionalTest, ReverseIteratorTraversal) {
    std::vector<std::string> reversed_keys;
    for (auto it = map.rbegin(); it != map.rend(); ++it) {
        reversed_keys.push_back(it->first);
    }
    EXPECT_EQ(reversed_keys, (std::vector<std::string>{"three", "two", "one"}));
}

TEST_F(SequenceMapFunctionalTest, ConstReverseIteratorTraversal) {
    const auto& const_map = map;
    std::vector<std::string> reversed_keys;
    for (auto it = const_map.crbegin(); it != const_map.crend(); ++it) {
        reversed_keys.push_back(it->first);
    }
    EXPECT_EQ(reversed_keys, (std::vector<std::string>{"three", "two", "one"}));
}

TEST_F(SequenceMapFunctionalTest, IteratorEqualityAndInequality) {
    auto begin = map.begin();
    auto same = map.begin();
    auto end = map.end();

    EXPECT_TRUE(begin == same);
    EXPECT_FALSE(begin != same);
    EXPECT_TRUE(begin != end);
    EXPECT_FALSE(begin == end);
}

TEST_F(SequenceMapFunctionalTest, IteratorAlgorithmsWork) {
    auto it = std::find_if(map.begin(), map.end(),
        [](const auto& pair) { return pair.second == 2; });

    ASSERT_NE(it, map.end());
    EXPECT_EQ(it->first, "two");
}

TEST_F(SequenceMapFunctionalTest, IteratorMutability) {
    auto beg = map.begin();
    beg->second = 8;

    EXPECT_EQ(map("one"), 8);
}

TEST_F(SequenceMapFunctionalTest, IteratorConversion) {
    auto cBeg = SequenceMap<std::string, int>::const_iterator(map.begin());

    EXPECT_EQ(cBeg, map.begin());
}

TEST_F(SequenceMapFunctionalTest, IteratorPlusMinusOperator) {
    auto it = map.begin();

    auto it2 = it + 2;
    EXPECT_EQ(it2->first, "three");

    auto it3 = it2 - 2;
    EXPECT_EQ(it3->first, "one");
}

TEST_F(SequenceMapFunctionalTest, IteratorPlusMinusEqualsOperator) {
    auto it = map.end() - 1;
    EXPECT_EQ(it->first, "three");

    it -= 2;
    EXPECT_EQ(it->first, "one");

    it += 2;
    EXPECT_EQ(it->first, "three");
}

TEST_F(SequenceMapFunctionalTest, IteratorDifferenceComputesCorrectDistance) {
    auto begin = map.begin();
    auto end = map.end();

    EXPECT_EQ(end - begin, map.size());
    EXPECT_EQ((begin + 1) - begin, 1);
    EXPECT_EQ((begin + 2) - (begin + 1), 1);
}


TEST_F(SequenceMapFunctionalTest, InsertSingleAtPositionInsertsNewKey) {
    auto it = map.find("two");
    ASSERT_NE(it, map.end());
    auto pos = ++map.begin();

    auto [inserted_it, inserted] = map.insert(pos, std::make_pair<const std::string, int>(std::string("four"), 4));
    EXPECT_TRUE(inserted);
    EXPECT_EQ(inserted_it->first, "four");
    EXPECT_EQ(inserted_it->second, 4);
    EXPECT_EQ(map.size(), 4);

    // "four" should be at index 1 now, shifting "two" and "three" right
    EXPECT_EQ(map.atIdx(1).first, "four");
    EXPECT_EQ(map.atIdx(2).first, "two");
    EXPECT_EQ(map.atIdx(3).first, "three");
}

TEST_F(SequenceMapFunctionalTest, InsertSingleAtPositionRejectsDuplicateKey) {
    auto pos = ++map.begin();
    auto [it, inserted] = map.insert(pos, std::make_pair(std::string("two"), 22));
    EXPECT_FALSE(inserted);
    EXPECT_EQ(it->first, "two");
    EXPECT_EQ(it->second, 2); // original value unchanged
    EXPECT_EQ(map.size(), 3);
}

TEST_F(SequenceMapFunctionalTest, InsertRangeInsertsOnlyUniqueKeys) {
    std::vector<std::pair<std::string, int>> to_insert{
        {"zero", 0},
        {"two", 22},   // duplicate, should be skipped
        {"four", 4},
        {"five", 5},
    };

    auto pos = ++map.begin();
    auto [first_inserted_it, inserted_count] = map.insert(pos, to_insert.begin(), to_insert.end());

    EXPECT_EQ(inserted_count, 3);
    EXPECT_EQ(first_inserted_it->first, "zero");
    EXPECT_EQ(map.size(), 6);

    // Check insertion order at indices 1, 2, 3 matches inserted unique keys
    EXPECT_EQ(map.atIdx(1).first, "zero");
    EXPECT_EQ(map.atIdx(2).first, "four");
    EXPECT_EQ(map.atIdx(3).first, "five");
}

TEST_F(SequenceMapFunctionalTest, InsertRangeReturnsEndIteratorIfNoInsertions) {
    std::vector<std::pair<std::string, int>> duplicates{
        {"one", 10},
        {"two", 20},
        {"three", 30},
    };

    auto pos = map.begin();
    auto [it, count] = map.insert(pos, duplicates.begin(), duplicates.end());

    EXPECT_EQ(count, 0);
    EXPECT_EQ(it, map.end());
    EXPECT_EQ(map.size(), 3);
}