#ifndef ExtensionsHelpers_hpp
#define ExtensionsHelpers_hpp

#include "GenParticle.hpp"
#include "Helpers.hpp"
#include "Muon.hpp"
#include "Electron.hpp"
#include "Jet.hpp"
#include "HepMCParticle.hpp"
#include "PhysicsObject.hpp"

inline std::shared_ptr<GenParticle> asGenParticle(const std::shared_ptr<PhysicsObject> physicsObject) {
  return std::static_pointer_cast<GenParticle>(physicsObject);
}

inline std::shared_ptr<Muon> asMuon(const std::shared_ptr<PhysicsObject> physicsObject) {
  return std::static_pointer_cast<Muon>(physicsObject);
}

inline std::shared_ptr<Electron> asElectron(const std::shared_ptr<PhysicsObject> physicsObject) {
  return std::static_pointer_cast<Electron>(physicsObject);
}

inline std::shared_ptr<Jet> asJet(const std::shared_ptr<PhysicsObject> physicsObject) {
  return std::static_pointer_cast<Jet>(physicsObject);
}

inline std::shared_ptr<HepMCParticle> asHepMCParticle(const std::shared_ptr<PhysicsObject> physicsObject) {
  return std::static_pointer_cast<HepMCParticle>(physicsObject);
}

#endif /* ExtensionsHelpers_hpp */


