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
  // Число элементов в кэше
  size_t sz_ = 0;
  // Ёмкоть кэша
  size_t capacity_;
  // Минимальное число вызовов среди элементов кэша
  int minFreq;

  // Хэш таблица с указателями на объекты в кэше
  std::unordered_map<T, typename std::list<Object_freq_>::iterator> cacheHash_;
  // Хэш таблица соответсвтвия числп вызовов и элементов
  std::unordered_map<int, std::list<Object_freq_>> freqHash_;

 public:
  // Конструктор
  Cache_LFU(size_t capacity) : capacity_(capacity) {};

  // Функция-интерфейс
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
void run_program();
}  // namespace Cache
#endif