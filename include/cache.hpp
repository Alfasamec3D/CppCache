#ifndef CACHE_HPP
#define CACHE_HPP

#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Cache {

template <typename T>

class Cache_LFU {
  struct Object_freq_ {
    T object_;
    int freq;
  };

 private:
  // number of elements in cache
  size_t sz_ = 0;
  // cache capacity
  size_t capacity_;
  // minimal times of callings among cache elements
  int minFreq;

  //  hash table wit pointers to objects in cache
  std::unordered_map<T, typename std::list<Object_freq_>::iterator> cacheHash_;
  // hash table, matching times of callings and elements
  std::unordered_map<int, std::list<Object_freq_>> freqHash_;

 public:
  // constructor
  Cache_LFU(size_t capacity) : capacity_(capacity) {};

  // function-interface
  bool require(T input);
};

template <typename T>
class Cache_IDEAL {
 private:
  size_t capacity_;
  std::unordered_set<T> cache_;

  void delete_latest_elem(
      const std::vector<T>& inputs,
      const typename std::vector<T>::iterator& inputIterator);

 public:
  Cache_IDEAL(size_t capacity) : capacity_(capacity) {};

  int run(const std::vector<T>& inputs);
};
void run_LFU();
void run_IDEAL();
}  // namespace Cache
#endif