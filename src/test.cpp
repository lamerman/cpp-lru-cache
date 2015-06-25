#include "lrucache.hpp"
#include "gtest/gtest.h"

const int NUM_OF_TEST1_RECORDS = 100;
const int NUM_OF_TEST2_RECORDS = 100;
const int TEST2_CACHE_CAPACITY = 50;

TEST(CacheTest, SimplePut) {
    cache::lru_cache<int, int> cache(1);
    cache.put(7, 777);
    EXPECT_TRUE(cache.exists(7));
    EXPECT_EQ(777, cache.get(7));
    EXPECT_EQ(1, cache.size());
}

TEST(CacheTest, MissingValue) {
    cache::lru_cache<int, int> cache(1);
    EXPECT_THROW(cache.get(7), std::range_error);
}

TEST(CacheTest1, KeepsAllValuesWithinCapacity) {
    cache::lru_cache<int, int> cache(TEST2_CACHE_CAPACITY);

    for (int i = 0; i < NUM_OF_TEST2_RECORDS; ++i) {
        cache.put(i, i);
    }

    for (int i = 0; i < NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; ++i) {
        EXPECT_FALSE(cache.exists(i));
    }

    for (int i = NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; i < NUM_OF_TEST2_RECORDS; ++i) {
        EXPECT_TRUE(cache.exists(i));
        EXPECT_EQ(i, cache.get(i));
    }

    size_t size = cache.size();
    EXPECT_EQ(TEST2_CACHE_CAPACITY, size);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
