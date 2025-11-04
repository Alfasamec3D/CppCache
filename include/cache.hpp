#ifndef CACHE_HPP
#define CACHE_HPP

#include <list>
#include <unordered_map>

namespace Cache {

template <typename T>

class Cache_LFU
{
    struct Object_freq_
    {
      T object_;
      int freq;
    };

   private:
    // Размер кэша
    size_t sz_;
    size_t capacity_;
    int minFreq;

    // Хэш таблица с объектом
    std::unordered_map<T, typename std::list<Object_freq_>::iterator> cacheHash_;
    std::unordered_map<int, std::list<Object_freq_>> freqHash_;


    // Функция добавления элемента в кэш

   public:

   //Конструктор
    Cache_LFU(size_t capacity) : capacity_(capacity) {};

    //Функция-интерфейс
    bool require(T input) {

        //Если запрашиваемый объект уже в кэше
        if (cacheHash_
          .find(input) != cacheHash_.end()) {

            //Итератор на входной объект в кэше
          const auto inputIterator = cacheHash_[input];

          //Если частота объекта наименьшая в кэшэ и он единственный с такой частотой, то минимальная частота увеличивается на 1
          if ((inputIterator->freq==minFreq)
            &&(freqHash_[minFreq].size() == 1)) {
            ++minFreq;
          }

          // Увеличаем частотную характеристику
          ++(inputIterator->freq);

          //Смещаем объект в область списка соответствующего числа обращений

          /*//Если нет слота с соответствующей частотой в таблице частот, создаем его
          if (freqHash_.find(inputIterator->freq)==freqHash_.end())
          {
            freqHash_[inputIterator->freq];
          }*/

          //Смещаем объект в область списка соответствующего числа обращений
          freqHash_[inputIterator->freq].splice(freqHash_[inputIterator->freq].end(),freqHash_[inputIterator->freq-1] , inputIterator);
          return true;
        }
        else {
          // Если запрашиваемого объекта нет в кэше
          


          // Если кэш заполнен
          if (sz_==capacity_) {
            //Удаляем объект с наименьшей частотой
            freqHash_[minFreq].pop_front();
            --sz_;
          }

          //Добавляем в список объектов с частотой 1 запрошенный элемент
          freqHash_[1].push_back({input, 1});
          ++sz_;

          // Минимальная частота снова 1
          minFreq = 1;

          cacheHash_[input] = --(freqHash_[1].end());

          return false;
        }
    }
};
}

#endif