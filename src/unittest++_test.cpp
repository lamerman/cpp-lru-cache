#include "../include/lrucache.hpp"
#include <unittest++/UnitTest++.h>

const int NUM_OF_TEST1_RECORDS = 100;
const int NUM_OF_TEST2_RECORDS = 100;
const int TEST2_CACHE_CAPACITY = 50;

SUITE(CacheTest) {

TEST(SimplePut) {
    cache::lru_cache<int, int> cache_lru(1);
    cache_lru.put(7, 777);
    CHECK(cache_lru.exists(7));
    CHECK_EQUAL(777, cache_lru.get(7));
    CHECK_EQUAL(1, cache_lru.size());
}

TEST(MissingValue) {
    cache::lru_cache<int, int> cache_lru(1);
    CHECK_THROW(cache_lru.get(7), std::range_error);
}

TEST(KeepsAllValuesWithinCapacity) {
    cache::lru_cache<int, int> cache_lru(TEST2_CACHE_CAPACITY);

    for (int i = 0; i < NUM_OF_TEST2_RECORDS; ++i) {
        cache_lru.put(i, i);
    }

    for (int i = 0; i < NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; ++i) {
        CHECK(not cache_lru.exists(i));
    }

    for (int i = NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; i < NUM_OF_TEST2_RECORDS; ++i) {
        CHECK(cache_lru.exists(i));
        CHECK_EQUAL(i, cache_lru.get(i));
    }

    size_t size = cache_lru.size();
    CHECK_EQUAL(TEST2_CACHE_CAPACITY, size);
}
}

int main(int argc, char **argv) {
    return UnitTest::RunAllTests();
}
