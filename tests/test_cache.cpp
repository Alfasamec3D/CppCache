#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <string>

#include "cache.hpp"
TEST(CacheTest, BasicScenario) {
  std::ifstream file("tests/tests.txt");
  ASSERT_TRUE(file.is_open()) << "Cannot open tests.txt";

  size_t x;
  int el_quantity;
  std::vector<int> input_data;
  int hitsCount;

  while (file >> x) {
    size_t capacity = x;
    Cache::Cache_LFU<int> cache{capacity};

    file >> el_quantity;
    hitsCount = 0;
    input_data.clear();

    for (int i = 0; i <= el_quantity - 1; ++i) {
      file >> x;
      input_data.push_back(x);
      hitsCount += cache.require(x);
    }

    file >> x;
    EXPECT_EQ(hitsCount, x);

    file >> x;
    EXPECT_EQ(Cache::runIDEAL(capacity, input_data), x);
  }
}