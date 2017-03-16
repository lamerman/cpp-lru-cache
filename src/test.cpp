#include "lrucache.hpp"
#include "gtest/gtest.h"

const int NUM_OF_TEST1_RECORDS = 100;
const int NUM_OF_TEST2_RECORDS = 100;
const int TEST2_CACHE_CAPACITY = 50;

TEST(CacheTest, SimplePut) {
    cache::lru_cache<int, int> cache_lru(1);
    std::pair<int, int> lru_item;
    cache_lru.put(7, 777, lru_item);
    EXPECT_TRUE(cache_lru.exists(7));
    EXPECT_EQ(777, cache_lru.get(7));
    EXPECT_EQ(1, cache_lru.size());
}

TEST(CacheTest, MissingValue) {
    cache::lru_cache<int, int> cache_lru(1);
    EXPECT_THROW(cache_lru.get(7), std::range_error);
}

TEST(CacheTest1, KeepsAllValuesWithinCapacity) {
    cache::lru_cache<int, int> cache_lru(TEST2_CACHE_CAPACITY);
    std::pair<int, int> lru_item;
    for (int i = 0; i < NUM_OF_TEST2_RECORDS; ++i) {
        cache_lru.put(i, i, lru_item);
    }

    for (int i = 0; i < NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; ++i) {
        EXPECT_FALSE(cache_lru.exists(i));
    }

    for (int i = NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; i < NUM_OF_TEST2_RECORDS; ++i) {
        EXPECT_TRUE(cache_lru.exists(i));
        EXPECT_EQ(i, cache_lru.get(i));
    }

    size_t size = cache_lru.size();
    EXPECT_EQ(TEST2_CACHE_CAPACITY, size);
}

TEST(CacheTest2, CapturesTheLRUItem) {
    cache::lru_cache<int, int> cache_lru(3);
    std::pair<int, int> lru_item;

    EXPECT_FALSE(cache_lru.put(1, 111, lru_item));
    EXPECT_FALSE(cache_lru.put(2, 222, lru_item));
    EXPECT_FALSE(cache_lru.put(3, 333, lru_item));
    EXPECT_TRUE(cache_lru.put(4, 444, lru_item));

    EXPECT_EQ(1, lru_item.first);
    EXPECT_EQ(111, lru_item.second);

    EXPECT_TRUE(cache_lru.exists(2));
    EXPECT_TRUE(cache_lru.exists(3));
    EXPECT_TRUE(cache_lru.exists(4));

    EXPECT_EQ(222, cache_lru.get(2));
    EXPECT_EQ(333, cache_lru.get(3));
    EXPECT_EQ(444, cache_lru.get(4));

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
