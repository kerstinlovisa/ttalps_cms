//  PhysicsObject.cpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#include "PhysicsObject.hpp"

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::Reset()
{
  for (auto &[key, value] : values_uint) value = 0;
  for (auto &[key, value] : values_int) value = 0;
  for (auto &[key, value] : values_bool) value = 0;
  for (auto &[key, value] : values_float) value = 0;
  for (auto &[key, value] : values_ulong) value = 0;
  for (auto &[key, value] : values_uchar) value = 0;
}