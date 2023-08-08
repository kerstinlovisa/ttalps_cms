//  Collection.hpp
//
//  Created by Jeremi Niedziela on 07/08/2023.

#ifndef Collection_hpp
#define Collection_hpp

#include <vector>

template <typename T>
class Collection : public std::vector<T> {
 private:
  size_t stopIndex;

 public:
  void ChangeVisibleSize(size_t index) { stopIndex = index; }

  class Iterator {
   public:
    Iterator(Collection &v, size_t index) : vec(v), currentIndex(index) {}

    Iterator &operator++() {
      ++currentIndex;
      return *this;
    }

    bool operator!=(const Iterator &other) const {
      return currentIndex != other.vec.stopIndex;
    }

    T &operator*() { return vec[currentIndex]; }

   private:
    Collection &vec;
    size_t currentIndex;
  };

  Iterator begin() { return Iterator(*this, 0); }
  Iterator end() { return Iterator(*this, stopIndex); }
};

#endif /* Collection_hpp */