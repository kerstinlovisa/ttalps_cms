#ifndef HepMCParticle_hpp
#define HepMCParticle_hpp

#include "Helpers.hpp"
#include "PhysicsObject.hpp"

class HepMCParticle;
typedef Collection<std::shared_ptr<HepMCParticle>> HepMCParticles;

class HepMCParticle : public PhysicsObject {
 public:
  float GetPx() { return Get("px"); }
  float GetPy() { return Get("py"); }
  float GetPz() { return Get("pz"); }
  float GetEnergy() { return Get("energy"); }

  int GetStatus() { return Get("status"); }
  int GetPid() { return Get("pid"); }

  TLorentzVector GetLorentzVector(){
    TLorentzVector vec;
    vec.SetPxPyPzE(GetPx(), GetPy(), GetPz(), GetEnergy());
    return vec;
  }

 private:

};

#endif /* HepMCParticle_hpp */
