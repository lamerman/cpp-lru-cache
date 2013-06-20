#include <lrucache.hpp>
#include <stdio.h>
#include <iostream>
#include <assert.h>
 
const int NUM_OF_TEST1_RECORDS = 1000000;
const int NUM_OF_TEST2_RECORDS = 1000000;
const int TEST2_CACHE_CAPACITY = 50000;

int main(int argc, char *argv[])
{
	{
		cache::lru_cache<int, int> cache(NUM_OF_TEST1_RECORDS);

		for (int i = 0; i < NUM_OF_TEST1_RECORDS; ++i) {
			cache.put(i, i);
		}

		for (int i = 0; i < NUM_OF_TEST1_RECORDS; ++i) {
			if (!cache.exists(i)) assert(false);
			assert(cache.get(i) == i);
		}

		size_t size = cache.size();
		assert(size == NUM_OF_TEST1_RECORDS);
	}
	
	{
		cache::lru_cache<int, int> cache(TEST2_CACHE_CAPACITY);

		for (int i = 0; i < NUM_OF_TEST2_RECORDS; ++i) {
			cache.put(i, i);
		}

		for (int i = 0; i < NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; ++i) {
			if (cache.exists(i)) assert(false);
		}
		
		for (int i = NUM_OF_TEST2_RECORDS - TEST2_CACHE_CAPACITY; i < NUM_OF_TEST2_RECORDS; ++i) {
			if (!cache.exists(i)) assert(false);
			assert(cache.get(i) == i);
		}

		size_t size = cache.size();
		assert(size == TEST2_CACHE_CAPACITY);
	}
	
	{
		cache::lru_cache<std::string, std::string> cache(3);
		cache.put("one", "one");
		cache.put("two", "two");
		cache.put("three", "three");
		
		assert(cache.exists("one"));
		assert(cache.exists("two"));
		assert(cache.exists("three"));
		
		assert(cache.get("one") == "one");
		assert(cache.get("two") == "two");
		assert(cache.get("three") == "three");
		
		cache.put("four", "four");
		assert(cache.exists("two"));
		assert(cache.exists("three"));
		assert(cache.exists("four"));
		
		assert(cache.get("four") == "four");
	}
	
	
	return 0;
}