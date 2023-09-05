//  Electron.hpp
//
//  Created by Jeremi Niedziela on 10/08/2023.

#ifndef Electron_hpp
#define Electron_hpp

#include "Helpers.hpp"
#include "PhysicsObject.hpp"

class Electron;
typedef Collection<std::shared_ptr<Electron>> Electrons;

class Electron {
 public:
  Electron(std::shared_ptr<PhysicsObject> physicsObject_) : physicsObject(physicsObject_) {}

  inline float GetPt() { return physicsObject->Get("pt"); }
  inline float GetEta() { return physicsObject->Get("eta"); }
  inline float GetPhi() { return physicsObject->Get("phi"); }

 private:
  std::shared_ptr<PhysicsObject> physicsObject;
};

#endif /* Electron_hpp */
