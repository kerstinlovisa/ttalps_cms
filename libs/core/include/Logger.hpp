//  Logger.hpp
//
//  Created by Jeremi Niedziela on 07/08/2023.

#ifndef Logger_hpp
#define Logger_hpp

#include "Helpers.hpp"

struct info {
  template <class T> info &operator<<(const T &v) {
    std::cout << v;
    return *this;
  }
};

struct warn {
  template <class T> warn &operator<<(const T &v) {
    std::cout << "\033[1;33m" << v << "\033[0m";
    return *this;
  }
};

struct error {
  template <class T> error &operator<<(const T &v) {
    std::cout << "\033[1;31m" << v << "\033[0m";
    return *this;
  }
};

struct fatal {
  template <class T> fatal &operator<<(const T &v) {
    std::cout << "\033[1;35m" << v << "\033[0m";
    return *this;
  }
};

#endif /* Logger_hpp */