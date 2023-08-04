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
  // for (auto &[type, objects] : physObjects) objects.clear();
  
  for(auto &[key, value] : values_uint) value = 0;
  for(auto &[key, value] : values_int) value = 0;
  for(auto &[key, value] : values_bool) value = 0;
  for(auto &[key, value] : values_float) value = 0;
  for(auto &[key, value] : values_ulong) value = 0;
}