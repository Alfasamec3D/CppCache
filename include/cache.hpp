#ifndef CACHE_HPP
#define CACHE_HPP

#include <algorithm>
#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Cache {

template <typename T, typename KeyT = int>
struct Page {
  Page(KeyT key) : key_(key) {};
  KeyT get_key() const { return key_; }

 private:
  KeyT key_;
  T* content_ = nullptr;
};

// function for getting page slow
template <typename T, typename KeyT = int>
Page<T, KeyT> slow_get_page_t(const KeyT& key) {
  return Page<T, KeyT>{key};
}

template <typename T, typename KeyT = int>
class Cache_LFU {
  struct Object_freq_ {
    Page<T, KeyT> page_;
    int freq_;
  };

 private:
  // number of elements in cache
  size_t sz_ = 0;
  // cache capacity
  size_t capacity_;
  // minimal times of callings among cache elements
  int minFreq;

  //  hash table with pointers to pages in cache
  std::unordered_map<KeyT, typename std::list<Object_freq_>::iterator>
      cacheHash_;
  // hash table, matching times of callings and keys
  std::unordered_map<int, std::list<Object_freq_>> freqHash_;

 public:
  // constructor
  Cache_LFU(size_t capacity) : capacity_(capacity) {};

  // function-interface
  bool lookup_update(const KeyT& key, Page<T, KeyT> (*slow_get_page)(
                                          const KeyT&) = slow_get_page_t<int>) {
    // if required object is already in cache
    if (cacheHash_.find(key) != cacheHash_.end()) {
      // Iterator to the requested page in cache
      const auto inputIterator = cacheHash_[key];

      // if frequency of the object is minimal and it is the only one with such
      // frequency, then minimal frequency increases by 1
      if ((inputIterator->freq_ == minFreq) &&
          (freqHash_[minFreq].size() == 1)) {
        ++minFreq;
      }

      // increase frequency
      ++(inputIterator->freq_);

      // Move object to area of matching frequency
      freqHash_[inputIterator->freq_].splice(
          freqHash_[inputIterator->freq_].end(),
          freqHash_[inputIterator->freq_ - 1], inputIterator);
      return true;
    } else {
      // If required object is not in cache

      // if cache is full
      if (sz_ == capacity_) {
        // Delete object with minimal frequency from the cache
        cacheHash_.erase(freqHash_[minFreq].front().page_.get_key());
        // Delete iterator to this
        freqHash_[minFreq].pop_front();
        --sz_;
      }
      // Add required element to a list of objects with frequency 1
      freqHash_[1].push_back({slow_get_page(key), 1});
      ++sz_;
      // Minimal frequency is 1 again
      minFreq = 1;

      cacheHash_[key] = --(freqHash_[1].end());

      return false;
    }
  }
};

template <typename T, typename KeyT = int>
class Cache_Ideal {
 private:
  size_t capacity_;
  std::list<Page<T, KeyT>> cache_;
  std::unordered_map<KeyT, typename std::list<Page<T, KeyT>>::iterator>
      cache_keys_;
  std::set<std::pair<size_t, KeyT>> order_;
  std::unordered_map<KeyT, size_t> next_use_map_;

 public:
  Cache_Ideal(const size_t& capacity) : capacity_(capacity) {};

  int lookup_update_list(
      const std::vector<T>& keys,
      Page<T, KeyT> (*slow_get_page)(const KeyT&) = slow_get_page_t<int>) {
    int hits = 0;

    std::unordered_map<KeyT, std::queue<size_t>> future;

    for (size_t i = 0; i < keys.size(); ++i) future[keys[i]].push(i);

    for (size_t i = 0; i < keys.size(); ++i) {
      KeyT key = keys[i];

      future[key].pop();

      size_t next_use = future[key].empty() ? std::numeric_limits<size_t>::max()
                                            : future[key].front();
      // If object is in cache
      if (cache_keys_.find(key) != cache_keys_.end()) {
        ++hits;

        order_.erase({next_use_map_[key], key});
        next_use_map_[key] = next_use;
        order_.insert({next_use, key});
      }
      // If object not in cache
      else {
        // If there is free space in cache
        if (cache_.size() < capacity_) {
          cache_.push_back(slow_get_page(key));
          cache_keys_[key] = --cache_.end();
          next_use_map_[key] = next_use;
          order_.insert({next_use, key});
        }
        // if there is no free space in cache and next use of insert key is
        // closer than farthest next use of objects in cache
        else {
          auto it = std::prev(order_.end());
          if (next_use < it->first) {
            cache_.erase(cache_keys_[it->second]);
            cache_keys_.erase(it->second);
            next_use_map_.erase(it->second);
            order_.erase(it);

            cache_.push_back(slow_get_page(key));
            cache_keys_[key] = --cache_.end();
            next_use_map_[key] = next_use;
            order_.insert({next_use, key});
          }
        }
        // if there is no free space in cache and next use of insert key is
        // farther than farthest next use of objects in cache just go to
        // the next insert key
      }
    }

    return hits;
  }
};

void run_LFU();
void run_IDEAL();
}  // namespace Cache
#endif