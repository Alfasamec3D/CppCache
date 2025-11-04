#include "cache.hpp"
#include<iostream>
int main() { 
  size_t cacheCapacity;
  int elementsQuantity;

  
  std::cout << "Input cache capacity:" << std::endl;
  
  std::cin >> cacheCapacity;
  
  
  std::cout <<std::endl<< "Input quantity of the elements:" << std::endl;
  
  std::cin >> elementsQuantity;
  
  
  std::cout <<std::endl<< "Input the elements: " << std::endl;

  Cache::Cache_LFU<int> cache{cacheCapacity};
  int element;
  int hitsCount;
  for (int i = 0; i <= elementsQuantity-1; ++i) {
    std::cin >> element;
    hitsCount+=cache.require(element);
  }

  std::cout <<std::endl<< "Number of hits is: " << std::endl;
  std::cout << hitsCount<<std::endl;
};