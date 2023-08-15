//  Muon.hpp
//
//  Created by Jeremi Niedziela on 10/08/2023.

#ifndef Muon_hpp
#define Muon_hpp

#include "Helpers.hpp"
#include "PhysicsObject.hpp"

class Muon;
typedef Collection<std::shared_ptr<Muon>> Muons;

class Muon : public PhysicsObject {
 public:
  inline float GetPt() { return Get("pt"); }
  inline float GetEta() { return Get("eta"); }
  inline float GetPhi() { return Get("phi"); }
};

#endif /* Muon_hpp */
