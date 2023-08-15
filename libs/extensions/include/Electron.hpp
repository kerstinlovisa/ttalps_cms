//  Electron.hpp
//
//  Created by Jeremi Niedziela on 10/08/2023.

#ifndef Electron_hpp
#define Electron_hpp

#include "Helpers.hpp"
#include "PhysicsObject.hpp"

class Electron;
typedef Collection<std::shared_ptr<Electron>> Electrons;

class Electron : public PhysicsObject {
 public:
  inline float GetPt() { return Get("pt"); }
  inline float GetEta() { return Get("eta"); }
  inline float GetPhi() { return Get("phi"); }
};

#endif /* Electron_hpp */
