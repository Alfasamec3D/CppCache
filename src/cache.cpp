#include "cache.hpp"

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
#ifndef NDEBUG
    std::cout << std::endl << "Бро, попадание " << input;
#endif
    return true;
  } else {
    //If required object is not in cache

    // if cache is full
    if (sz_ == capacity_) {
// Delete object with minimal frequency
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
    // Add required element to a list of objects with frequency 1
    freqHash_[1].push_back({input, 1});
    ++sz_;
#ifndef NDEBUG
    std::cout << std::endl << "Бро, добавляю " << input;
#endif
    // Minimal frequency is 1 again
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
    // the inout element is in cache
    if (cache_.find(*inputIterator) == cache_.end()) {
      // if cache is full
      if (cache_.size() == capacity_) {
        size_t max_distance{0};
        size_t distance;
        auto deleteIterator = cache_.begin();
        for (auto cacheIterator = cache_.begin(); cacheIterator != cache_.end();
             ++cacheIterator) {
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

      // Now there is space in cache
      cache_.insert(*inputIterator);
#ifndef NDEBUG
      std::cout << std::endl << "Inserted " << *inputIterator << std::endl;
#endif
    }

    else {
// The element is in cache
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