cpp-lru-cache
=============

Simple and reliable LRU (Least Recently Used) cache for c++ based on hashmap and linkedlist. The library is header only, simple test and example are included.
It includes standard components and very little own logics that guarantees reliability.

Example:

```
/**Creates cache with maximum size of three. When the 
   size in achieved every next element will replace the 
   least recently used one and returns it*/
cache::lru_cache<std::string, std::string> cache(2);
std::pair<std::string, std::string> lru_item;

cache.put("one", "one", lru_item);
cache.put("two", "two", lru_item);
cache.put("three", "three", lru_item); //lru_item here contains "one" --> "one"

const std::string& from_cache = cache.get("two")

```

How to run tests:

```
mkdir build
cd build
cmake ..
make check
```
