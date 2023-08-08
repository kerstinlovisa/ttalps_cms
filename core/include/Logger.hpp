//  Logger.hpp
//
//  Created by Jeremi Niedziela on 07/08/2023.

#ifndef Logger_hpp
#define Logger_hpp

#include "Helpers.hpp"

struct Info {
  template <class T> Info &operator<<(const T &v) {
    std::cout << v;
    return *this;
  }
};

struct Warn {
  template <class T> Warn &operator<<(const T &v) {
    std::cout << "\033[1;33m" << v << "\033[0m";
    return *this;
  }
};

struct Err {
  template <class T> Err &operator<<(const T &v) {
    std::cout << "\033[1;31m" << v << "\033[0m";
    return *this;
  }
};

struct FatalErr {
  template <class T> FatalErr &operator<<(const T &v) {
    std::cout << "\033[1;35m" << v << "\033[0m";
    return *this;
  }
};

#endif /* Logger_hpp */