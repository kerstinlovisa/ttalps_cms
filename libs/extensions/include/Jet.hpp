//  Jet.hpp
//
//  Created by Jeremi Niedziela on 10/08/2023.

#ifndef Jet_hpp
#define Jet_hpp

#include "Helpers.hpp"
#include "PhysicsObject.hpp"

class Jet;
typedef Collection<std::shared_ptr<Jet>> Jets;

class Jet : public PhysicsObject {
 public:
  inline float GetPt() { return Get("pt"); }
  inline float GetEta() { return Get("eta"); }
  inline float GetPhi() { return Get("phi"); }
  inline float GetBtagDeepB() { return Get("btagDeepB"); }
};

#endif /* Jet_hpp */
