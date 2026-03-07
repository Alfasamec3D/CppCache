#include "cache.hpp"

#include <limits>
#include <queue>
#include <set>

namespace Cache {

template <typename T>
int runIDEAL(const size_t& capacity, const std::vector<T>& inputs) {
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
    // If object is in cache
    if (cache.count(value)) {
      ++hits;

      order.erase({next_use_map[value], value});
      next_use_map[value] = next_use;
      order.insert({next_use, value});
    }
    // If object not in cache
    else {
      // If there is free space in cache
      if (cache.size() < capacity) {
        cache.insert(value);
        next_use_map[value] = next_use;
        order.insert({next_use, value});
      }
      // if there is no free space in cache and next use of insert value is
      // closer than farthest next use of objects in cache
      else {
        auto it = std::prev(order.end());
        if (next_use < it->first) {
          cache.erase(it->second);
          next_use_map.erase(it->second);
          order.erase(it);

          cache.insert(value);
          next_use_map[value] = next_use;
          order.insert({next_use, value});
        }
      }
      // if there is no free space in cache and next use of insert value is
      // farther than farthest next use of objects in cache which just go to the
      // next insert value
    }
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
    hitsCount += cache.lookup_update(element);
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