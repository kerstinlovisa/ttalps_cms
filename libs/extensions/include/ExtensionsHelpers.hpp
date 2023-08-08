#ifndef ExtensionsHelpers_hpp
#define ExtensionsHelpers_hpp

#include "Helpers.hpp"
#include "GenParticle.hpp"
#include "PhysicsObject.hpp"

inline std::shared_ptr<GenParticle> asGenParticle(const std::shared_ptr<PhysicsObject> physicsObject)
{
    return std::static_pointer_cast<GenParticle>(physicsObject);
}

#endif /* ExtensionsHelpers_hpp */