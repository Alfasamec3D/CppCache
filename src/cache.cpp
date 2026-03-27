#include "cache.hpp"

namespace Cache {

void run_LFU() {
  size_t cacheCapacity;
  int elementsQuantity;
  int element;
  int hitsCount = 0;

  std::cin >> cacheCapacity;

  std::cin >> elementsQuantity;

  Cache_LFU<int> cache{cacheCapacity};

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

  Cache_Ideal<int> cache{cacheCapacity};

  for (int i = 0; i <= elementsQuantity - 1; ++i) {
    std::cin >> element;
    input_elem.push_back(element);
  }

  std::cout << cache.lookup_update_list(input_elem) << std::endl;
}
}  // namespace Cache