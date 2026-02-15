#include "cache.hpp"

int main() {
  size_t cacheCapacity;
  int elementsQuantity;

  std::cout << "Input cache capacity:" << std::endl;

  std::cin >> cacheCapacity;

  std::cout << std::endl << "Input quantity of the elements:" << std::endl;

  std::cin >> elementsQuantity;

  std::cout << std::endl << "Input the elements: " << std::endl;

  Cache::Cache_LFU<int> cache{cacheCapacity};
  Cache::Cache_IDEAL<int> ideal_cache{cacheCapacity};
  int element;
  int hitsCount = 0;
  std::vector<int> input_data;

  for (int i = 0; i <= elementsQuantity - 1; ++i) {
    std::cin >> element;
    input_data.push_back(element);
    hitsCount += cache.require(element);
#ifndef NDEBUG
    std::cout << std::endl << "I counted " << hitsCount << " hits" << std::endl;
#endif
  }

  std::cout << std::endl << "Number of hits is: ";
  std::cout << std::endl << hitsCount;

  std::cout << std::endl << "Number of hits for ideal cache is: ";
  std::cout << std::endl << ideal_cache.run(input_data) << std::endl;
}