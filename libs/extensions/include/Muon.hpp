//  Muon.hpp
//
//  Created by Jeremi Niedziela on 10/08/2023.

#ifndef Muon_hpp
#define Muon_hpp

#include "Helpers.hpp"
#include "PhysicsObject.hpp"

class Muon;
typedef Collection<std::shared_ptr<Muon>> Muons;

class Muon {
 public:
  Muon(std::shared_ptr<PhysicsObject> physicsObject_) : physicsObject(physicsObject_) {}

  inline float GetPt() { return physicsObject->Get("pt"); }
  inline float GetEta() { return physicsObject->Get("eta"); }
  inline float GetPhi() { return physicsObject->Get("phi"); }

 private:
  std::shared_ptr<PhysicsObject> physicsObject;
};

#endif /* Muon_hpp */
