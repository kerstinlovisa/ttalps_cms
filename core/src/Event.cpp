//  Event.cpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#include "Event.hpp"

Event::Event()
{
}

Event::~Event()
{
}

void Event::Reset()
{
  for (auto &[key, value] : values_uint) value = 0;
  for (auto &[key, value] : values_int) value = 0;
  for (auto &[key, value] : values_bool) value = 0;
  for (auto &[key, value] : values_float) value = 0;
  for (auto &[key, value] : values_ulong) value = 0;
  for (auto &[key, value] : values_uchar) value = 0;

  for(auto &[name, collection] : collections){
    for(auto element : *collection) element->Reset();
  }
}