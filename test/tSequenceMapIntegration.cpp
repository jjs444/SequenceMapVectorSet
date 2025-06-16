
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <utility>
#include "../src/SequenceMap.hpp" 

// Integration-ish tests. Purpose is to simulate real-world 
// usage workflows � i.e., sequences of operations that interact 
// with the SequenceMap and might affect its internal structure 
// (data_, key_to_index_, pool_, etc.).


TEST(SequenceMapIntegration, InsertAccessEraseReinsert) {
    SequenceMap<std::string, int> map;

    map.push_back({ "a", 1 });
    map.push_back({ "b", 2 });
    map.push_back({ "c", 3 });

    EXPECT_EQ(map.atKey("a"), 1);
    EXPECT_EQ(map.atKey("b"), 2);
    EXPECT_EQ(map.atKey("c"), 3);

    EXPECT_EQ(map.erase("b"), 1);
    EXPECT_EQ(map.count("b"), 0);

    map.push_back({ "b", 42 });
    EXPECT_EQ(map.atKey("b"), 42);

    std::vector<std::string> keys;
    for (const auto& kv : map) {
        keys.push_back(kv.first);
    }

    EXPECT_EQ(keys, (std::vector<std::string>{"a", "c", "b"}));
}

TEST(SequenceMapIntegration, OperatorInsertUpdateAccessEraseViaIterator) {
    SequenceMap<int, std::string> map;

    map(10) = "ten";
    map(20) = "twenty";

    EXPECT_EQ(map.atKey(10), "ten");
    EXPECT_EQ(map.atKey(20), "twenty");

    map(20) = "updated_twenty";
    EXPECT_EQ(map.atKey(20), "updated_twenty");

    auto it = map.find(10);
    EXPECT_NE(it, map.end());
    map.erase(it);

    EXPECT_EQ(map.count(10), 0);
    EXPECT_EQ(map.count(20), 1);
}

TEST(SequenceMapIntegration, CopyModificationAndIndependence) {
    SequenceMap<std::string, double> original;
    original.push_back({ "pi", 3.14 });
    original.push_back({ "e", 2.71 });

    SequenceMap<std::string, double> copy = original;
    EXPECT_EQ(copy.atKey("pi"), 3.14);
    EXPECT_EQ(copy.atKey("e"), 2.71);

    copy("pi") = 3.14159;
    copy("e") = 2.71828;

    EXPECT_EQ(original("pi"), 3.14);
    EXPECT_EQ(original("e"), 2.71);
}

TEST(SequenceMapIntegration, EmplaceAndEraseRange) {
    SequenceMap<int, std::string> map;

    map.emplace_back(1, "one");
    map.emplace_back(2, "two");
    map.emplace_back(3, "three");
    map.emplace_back(4, "four");

    auto first = map.find(2);
    auto last = map.find(4);
    map.erase(first, last);

    EXPECT_EQ(map.count(2), 0);
    EXPECT_EQ(map.count(3), 0);

    std::vector<int> remainingKeys;
    for (const auto& [k, _] : map) {
        remainingKeys.push_back(k);
    }

    EXPECT_EQ(remainingKeys, (std::vector<int>{1, 4}));
}

TEST(SequenceMapIntegration, ClearAndReinsert) {
    SequenceMap<std::string, int> map;
    map.push_back({ "x", 100 });
    map.push_back({ "y", 200 });
    map.push_back({ "z", 300 });

    map.clear();
    ASSERT_TRUE(map.empty());

    map.emplace_back("a", 1);
    map.emplace_back("b", 2);

    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(map.atKey("a"), 1);
    EXPECT_EQ(map.atKey("b"), 2);

    std::vector<std::string> order;
    for (const auto& [k, _] : map) {
        order.push_back(k);
    }

    EXPECT_EQ(order, (std::vector<std::string>{"a", "b"}));
}

TEST(SequenceMapIntegration, MixedAccessPatterns) {
    SequenceMap<std::string, std::string> map;

    map.emplace_back("one", "first");
    map.emplace_back("two", "second");
    map.emplace_back("three", "third");

    EXPECT_EQ(map.atKey("two"), "second");
    EXPECT_EQ(map.atIdx(2).first, "three");
    EXPECT_EQ(map[2].second, "third");
    EXPECT_EQ(map("one"), "first");

    map[1].second = "modified_second";
    EXPECT_EQ(map.atKey("two"), "modified_second");
}
