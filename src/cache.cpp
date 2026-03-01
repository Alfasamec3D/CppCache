#include "cache.hpp"

#include <limits>
#include <queue>
#include <set>

namespace Cache {

template <typename T>
bool Cache_LFU<T>::require(T input) {
  // if required object is already in cache
  if (cacheHash_.find(input) != cacheHash_.end()) {
    // Oterator to the input object in cache
    const auto inputIterator = cacheHash_[input];

    // if frequency of the object is minimal and it is the only one with such
    // frequency, then minimal frequency increases by 1
    if ((inputIterator->freq == minFreq) && (freqHash_[minFreq].size() == 1)) {
      ++minFreq;
    }

    // increase frequency
    ++(inputIterator->freq);

    // Move object to area of matching frequency
    freqHash_[inputIterator->freq].splice(freqHash_[inputIterator->freq].end(),
                                          freqHash_[inputIterator->freq - 1],
                                          inputIterator);
    return true;
  } else {
    // If required object is not in cache

    // if cache is full
    if (sz_ == capacity_) {
      // Delete object with minimal frequency
      cacheHash_.erase(freqHash_[minFreq].front().object_);
      freqHash_[minFreq].pop_front();
      --sz_;
    }
    // Add required element to a list of objects with frequency 1
    freqHash_[1].push_back({input, 1});
    ++sz_;
    // Minimal frequency is 1 again
    minFreq = 1;

    cacheHash_[input] = --(freqHash_[1].end());

    return false;
  }
}

template <typename T>
int runIDEAL(const size_t& capacity, const std::vector<T>& inputs) {
  const size_t INF = std::numeric_limits<size_t>::max();

  int hits = 0;

  std::unordered_map<T, std::queue<size_t>> future;

  for (size_t i = 0; i < inputs.size(); ++i) future[inputs[i]].push(i);

  std::unordered_set<T> cache;
  std::set<std::pair<size_t, T>> order;
  std::unordered_map<T, size_t> next_use_map;

  for (size_t i = 0; i < inputs.size(); ++i) {
    T value = inputs[i];

    future[value].pop();

    size_t next_use = future[value].empty() ? std::numeric_limits<size_t>::max()
                                            : future[value].front();

    if (cache.count(value)) {
      ++hits;

      order.erase({next_use_map[value], value});
    } else {
      if (cache.size() == capacity) {
        auto it = std::prev(order.end());
        cache.erase(it->second);
        next_use_map.erase(it->second);
        order.erase(it);
      }

      cache.insert(value);
    }

    next_use_map[value] = next_use;
    order.insert({next_use, value});
  }

  return hits;
}

void run_LFU() {
  size_t cacheCapacity;
  int elementsQuantity;
  int element;
  int hitsCount = 0;

  std::cin >> cacheCapacity;

  std::cin >> elementsQuantity;

  Cache::Cache_LFU<int> cache{cacheCapacity};

  for (int i = 0; i <= elementsQuantity - 1; ++i) {
    std::cin >> element;
    hitsCount += cache.require(element);
  }

  std::cout << hitsCount << std::endl;
}

void run_IDEAL() {
  size_t cacheCapacity;
  int elementsQuantity;
  int element;
  int hitsCount = 0;
  std::vector<int> input_elem;

  std::cin >> cacheCapacity;

  std::cin >> elementsQuantity;

  for (int i = 0; i <= elementsQuantity - 1; ++i) {
    std::cin >> element;
    input_elem.push_back(element);
  }

  std::cout << runIDEAL(cacheCapacity, input_elem) << std::endl;
}
}  // namespace Cache