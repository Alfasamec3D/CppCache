#include <gtest/gtest.h>

#include "cache.hpp"
using namespace Cache;

TEST(GetKeyTest, BasicCase) {
  int key = 2;
  Page<int> page{key};
  EXPECT_EQ(page.get_key(), key);
}

TEST(LookUpUpdateTest, BasicCase) {
  size_t capacity = 3;
  Cache_LFU<int> cache{capacity};
  int key = 2;
  bool result = cache.lookup_update(key);
  bool expected_result = false;
  EXPECT_EQ(result, expected_result);
}