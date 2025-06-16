
#include <gtest/gtest.h>
#include "../src/SequenceMap.hpp"

#include <string>
#include <vector>
#include <memory>

TEST(SequenceMapTypes, StringToInt) {
    SequenceMap<std::string, int> map;
    map.emplace_back("one", 1);
    map.emplace_back("two", 2);

    EXPECT_EQ(map.atKey("one"), 1);
    EXPECT_EQ(map.atKey("two"), 2);
}

TEST(SequenceMapTypes, IntToDouble) {
    SequenceMap<int, double> map;
    map.emplace_back(10, 3.14);
    map.emplace_back(20, 2.718);

    EXPECT_DOUBLE_EQ(map.atKey(10), 3.14);
    EXPECT_DOUBLE_EQ(map.atKey(20), 2.718);
}

TEST(SequenceMapTypes, StringToVectorInt) {
    SequenceMap<std::string, std::vector<int>> map;
    map.emplace_back("list", std::vector<int>{1, 2, 3});
    map.emplace_back("empty", std::vector<int>{});

    EXPECT_EQ(map.atKey("list").size(), 3);
    EXPECT_TRUE(map.atKey("empty").empty());
}

struct CustomStruct {
    int id;
    std::string label;

    CustomStruct() = default;
    CustomStruct(int id, std::string label) : id(id), label(std::move(label)) {}
    bool operator==(const CustomStruct& other) const {
        return id == other.id && label == other.label;
    }
};

TEST(SequenceMapTypes, IntToCustomStruct) {
    SequenceMap<int, CustomStruct> map;
    map.emplace_back(1, CustomStruct(10, "apple"));
    map.emplace_back(2, CustomStruct(20, "banana"));

    EXPECT_EQ(map.atKey(1).label, "apple");
    EXPECT_EQ(map.atKey(2).id, 20);
}

struct MoveOnly {
    std::unique_ptr<int> data;

    MoveOnly() = default;
    explicit MoveOnly(int val) : data(std::make_unique<int>(val)) {}
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
};

TEST(SequenceMapTypes, StringToMoveOnly) {
    SequenceMap<std::string, MoveOnly> map;
    map.emplace_back("x", MoveOnly(42));
    map("y") = MoveOnly(99);

    EXPECT_EQ(*map.atKey("x").data, 42);
    EXPECT_EQ(*map.atKey("y").data, 99);
}

struct Person {
    std::string first;
    std::string last;

    bool operator==(const Person& other) const {
        return first == other.first && last == other.last;
    }
};

namespace std {
    template<>
    struct hash<Person> {
        std::size_t operator()(const Person& p) const noexcept {
            std::size_t h1 = std::hash<std::string>{}(p.first);
            std::size_t h2 = std::hash<std::string>{}(p.last);
            return h1 ^ (h2 << 1);  // combine hashes
        }
    };
}

TEST(SequenceMapCustomKey, SupportsCustomKeyWithHash) {
    SequenceMap<Person, int> map;
    map.emplace_back(Person{ "Ada", "Lovelace" }, 1815);
    map.emplace_back(Person{ "Alan", "Turing" }, 1912);

    EXPECT_EQ(map.size(), 2);

    int adaYear = map.atKey(Person{ "Ada", "Lovelace" });
    int alanYear = map.atKey(Person{ "Alan", "Turing" });

    EXPECT_EQ(adaYear, 1815);
    EXPECT_EQ(alanYear, 1912);
}

TEST(SequenceMapCustomKey, EraseAndFindWithCustomKey) {
    SequenceMap<Person, int> map;
    Person grace{ "Grace", "Hopper" };
    map(grace) = 1906;

    EXPECT_EQ(map.count(grace), 1);
    EXPECT_NE(map.find(grace), map.end());

    map.erase(grace);
    EXPECT_EQ(map.count(grace), 0);
    EXPECT_EQ(map.find(grace), map.end());
}