#include "cache.hpp"

namespace Cache {

template <typename T>
bool Cache_LFU<T>::require(T input) {
  // Если запрашиваемый объект уже в кэше
  if (cacheHash_.find(input) != cacheHash_.end()) {
    // Итератор на входной объект в кэше
    const auto inputIterator = cacheHash_[input];

    // Если частота объекта наименьшая в кэшэ и он единственный с такой
    // частотой, то минимальная частота увеличивается на 1
    if ((inputIterator->freq == minFreq) && (freqHash_[minFreq].size() == 1)) {
      ++minFreq;
    }

    // Увеличаем частотную характеристику
    ++(inputIterator->freq);

    // Смещаем объект в область списка соответствующего числа обращений
    freqHash_[inputIterator->freq].splice(freqHash_[inputIterator->freq].end(),
                                          freqHash_[inputIterator->freq - 1],
                                          inputIterator);
#ifndef NDEBUG
    std::cout << std::endl << "Бро, попадание " << input;
#endif
    return true;
  } else {
    // Если запрашиваемого объекта нет в кэше

    // Если кэш заполнен
    if (sz_ == capacity_) {
// Удаляем объект с наименьшей частотой
#ifndef NDEBUG
      std::cout << std::endl
                << "Бро, удаляю " << freqHash_[minFreq].front().object_;
#endif
      cacheHash_.erase(freqHash_[minFreq].front().object_);
      freqHash_[minFreq].pop_front();
      --sz_;
    }
#ifndef NDEBUG
    std::cout << std::endl << "Бро, размер " << sz_;
#endif
    // Добавляем в список объектов с частотой 1 запрошенный элемент
    freqHash_[1].push_back({input, 1});
    ++sz_;
#ifndef NDEBUG
    std::cout << std::endl << "Бро, добавляю " << input;
#endif
    // Минимальная частота снова 1
    minFreq = 1;

    cacheHash_[input] = --(freqHash_[1].end());

#ifndef NDEBUG
    std::cout << std::endl << "Бро, размер " << sz_;
#endif

    return false;
  }
}

template <typename T>
void Cache_IDEAL<T>::delete_latest_elem(
    const std::vector<T>& inputs,
    const typename std::vector<T>::iterator& inputIterator) {
  size_t max_distance{0};
  size_t distance;
  auto deleteIterator = cache_.begin();
  for (auto cacheIterator = cache_.begin(); cacheIterator != cache_.end();
       ++cacheIterator) {
    //
    distance = std::distance(
        inputIterator,
        std::find(std::next(inputIterator), inputs.end(), *cacheIterator));

    if (max_distance < distance) {
      max_distance = distance;
      deleteIterator = cacheIterator;
    }
  }
#ifndef NDEBUG
  std::cout << std::endl << "Erased " << *deleteIterator << std::endl;
#endif
  cache_.erase(deleteIterator);
}

template <typename T>
int Cache_IDEAL<T>::run(const std::vector<T>& inputs) {
  int hitsCount = 0;
  for (auto inputIterator = inputs.begin(); inputIterator != inputs.end();
       ++inputIterator) {
    // Если входного элемента нет в Кэше
    if (cache_.find(*inputIterator) == cache_.end()) {
      // Если кэш заполнен
      if (cache_.size() == capacity_) {
        size_t max_distance{0};
        size_t distance;
        auto deleteIterator = cache_.begin();
        for (auto cacheIterator = cache_.begin(); cacheIterator != cache_.end();
             ++cacheIterator) {
          //
          distance = std::distance(
              inputIterator, std::find(std::next(inputIterator), inputs.end(),
                                       *cacheIterator));

          if (max_distance < distance) {
            max_distance = distance;
            deleteIterator = cacheIterator;
          }
        }
#ifndef NDEBUG
        std::cout << std::endl << "Erased " << *deleteIterator << std::endl;
#endif
        cache_.erase(deleteIterator);
      }

      // Теперь в кэше есть место
      cache_.insert(*inputIterator);
#ifndef NDEBUG
      std::cout << std::endl << "Inserted " << *inputIterator << std::endl;
#endif
    }

    else {
// Если элемент есть в кэше
#ifndef NDEBUG
      std::cout << std::endl << *inputIterator << " is in cache" << std::endl;
#endif
      ++hitsCount;
    }
  }
  return hitsCount;
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
#ifndef NDEBUG
    std::cout << std::endl << "I counted " << hitsCount << " hits" << std::endl;
#endif
  }

  std::cout << std::endl << hitsCount << std::endl;
}

void run_IDEAL() {
  size_t cacheCapacity;
  int elementsQuantity;
  int element;
  int hitsCount = 0;
  std::vector<int> input_elem;

  std::cin >> cacheCapacity;

  std::cin >> elementsQuantity;

  Cache::Cache_IDEAL<int> cache{cacheCapacity};

  for (int i = 0; i <= elementsQuantity - 1; ++i) {
    std::cin >> element;
    input_elem.push_back(element);
  }

  std::cout << std::endl << cache.run(input_elem) << std::endl;
}
}  // namespace Cache