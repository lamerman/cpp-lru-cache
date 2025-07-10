/* 
 * File:   lrucache.hpp
 * Author: Alexander Ponomarev, Lars H. Rohwedder
 *
 * Created on June 20, 2013, 5:09 PM, edited 2024-03-02 by LR
 */

#ifndef LRU_CACHE_HPP_INCLUDED_
#define LRU_CACHE_HPP_INCLUDED_

#include <unordered_map>
#include <list>
#include <cstddef>
#include <stdexcept>

namespace cache {

template<typename key_t, typename value_t>
class lru_cache
{
public:
	typedef typename std::pair<key_t, value_t> key_value_pair_t;
	typedef typename std::list<key_value_pair_t>::iterator list_iterator_t;

	explicit lru_cache(size_t max_size = 10)
	: _max_size(max_size)
	{ }
	
	lru_cache(const lru_cache<key_t, value_t>&) = delete;
	void operator=(const lru_cache<key_t, value_t>&) = delete;
	
	void put(const key_t& key, const value_t& value)
	{
		list.push_front(key_value_pair_t(key, value));
		remove(key);
		map[key] = list.begin();
		trim(_max_size);
	}

	void put(const key_t& key, value_t&& value)
	{
		list.push_front( key_value_pair_t{key, std::move(value)} );
		remove(key);
		map[key] = list.begin();
		trim(_max_size);
	}
	
	// just move the key's element to front of list
	void touch(const key_t& key)
	{
		auto it = map.find(key);
		if(it != map.end() && it->second != list.begin())
		{
			list.splice(list.begin(), list, it->second);
			map.insert(it, key_value_pair_t{key, list.begin()} );
		}
	}
	
	const value_t& get(const key_t& key)
	{
		auto it = map.find(key);
		if (it == map.end())
		{
			throw std::range_error("There is no such key in cache");
		} else {
			list.splice(list.begin(), list, it->second);
			return it->second->second;
		}
	}
	
	void remove(const key_t& key)
	{
		auto it = map.find(key);
		if( it!= map.end() )
		{
			list.erase(it->second);
			map.erase(it);
		}
	}
	
	void clear()
	{
		map.clear();
		list.clear();
	}
	
	bool exists(const key_t& key) const noexcept
	{
		return map.find(key) != map.end();
	}
	
	size_t size() const noexcept { return map.size(); }
	
	size_t max_size() const noexcept { return max_size(); }
	
	void max_size(size_t new_max_size)
	{
		if(new_max_size<_max_size)
		{
			trim(new_max_size);
		}
		_max_size = new_max_size;
	}
	
private:
	std::list<key_value_pair_t> list;
	std::unordered_map<key_t, list_iterator_t> map;
	size_t _max_size;
	
	void trim(size_t allowed_size)
	{
		while(map.size() > allowed_size)
		{
			auto last = list.rbegin();
			map.erase(last->first);
			list.pop_back();
		}
	}
};

} // namespace cache

#endif	/* LRU_CACHE_HPP_INCLUDED_ */

