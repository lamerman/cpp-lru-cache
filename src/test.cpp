#include "lrucache.hpp"
#include "gtest/gtest.h"
#ifdef _WIN32
#include <stdint.h>
#endif
#include <functional>

const int NUM_OF_TEST_RECORDS = 100;
const int TEST2_CACHE_CAPACITY = 50;


struct MyKeyPair { uint64_t partial_key_a; int partial_key_b; };
typedef double MyValue;

struct MyKeyPairMapper {
	template <typename TSeed>
	inline void HashCombine(TSeed value, TSeed *seed) const {
		*seed ^= value + 0x9e3779b9 + (*seed << 6) + (*seed >> 2);
	}
	inline size_t operator()(const MyKeyPair& p) const {
		size_t hash = std::hash<uint64_t>()(p.partial_key_a);
		size_t hash_2 = std::hash<int>()(p.partial_key_b);

		HashCombine<size_t>(hash_2, &hash);
		return hash;
	}
	inline bool operator()(const MyKeyPair &p, const MyKeyPair &q) const {
		return p.partial_key_a == q.partial_key_a && p.partial_key_b == q.partial_key_b;
	}
};


TEST(CacheTest, SimplePut) {
    cache::lru_cache<MyKeyPair, MyValue, MyKeyPairMapper, MyKeyPairMapper> cache(1);
	cache.put({ 7,3 }, 777.0);
    EXPECT_TRUE(cache.exists({ 7,3 }));
    EXPECT_EQ(777.0, cache.get({ 7,3 }));
    EXPECT_EQ(1, cache.size());
}

TEST(CacheTest, MissingValue) {
    cache::lru_cache<MyKeyPair, MyValue, MyKeyPairMapper, MyKeyPairMapper> cache(1);
	EXPECT_THROW(cache.get({ 7,3 } ) , std::range_error);
}

TEST(CacheTest1, KeepsAllValuesWithinCapacity) {
    cache::lru_cache<MyKeyPair, MyValue, MyKeyPairMapper, MyKeyPairMapper> cache(TEST2_CACHE_CAPACITY);

    for (int i = 0; i < NUM_OF_TEST_RECORDS; ++i) {
		cache.put( { i, i*2 }, i+1);
    }

    for (int i = 0; i < NUM_OF_TEST_RECORDS - TEST2_CACHE_CAPACITY; ++i) {
        EXPECT_FALSE(cache.exists({ i, i * 2 }));
    }

    for (int i = NUM_OF_TEST_RECORDS - TEST2_CACHE_CAPACITY; i < NUM_OF_TEST_RECORDS; ++i) {
        EXPECT_TRUE(cache.exists({ i, i * 2 }));
        EXPECT_EQ(  i+1, cache.get ( { i, i * 2 } )  );
    }

    size_t size = cache.size();
    EXPECT_EQ(TEST2_CACHE_CAPACITY, size);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
