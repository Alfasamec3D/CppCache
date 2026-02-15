#include <fstream>
#include <string>

#include "cache.hpp"
bool TestCache(std::string& file_path) {
  std::ifstream file(file_path);
  int x;
  int el_quantity;
  std::vector<int> input_data;
  int hitsCount;

  while (file >> x) {
    Cache::Cache_LFU<int> cache{x};
    Cache::Cache_IDEAL<int> ideal_cache{x};

    file >> el_quantity;
    hitsCount = 0;
    input_data.clear();

    for (int i = 0; i <= el_quantity - 1; ++i) {
      file >> x;
      input_data.push_back(x);
      hitsCount += cache.require(x);
    }

    file >> x;
    if (hitsCount != x) {
      return false;
    }

    file >> x;
    if (ideal_cache.run(input_data) != x) {
      return false;
    }
  }
  return true;
}

int main() {
  std::string test_path{"tests.txt"};
  if (TestCache(test_path)) {
    return 0;
  };
  return 1;
}