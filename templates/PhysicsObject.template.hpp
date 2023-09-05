#ifndef TemplateName_hpp
#define TemplateName_hpp

#include "Helpers.hpp"
#include "PhysicsObject.hpp"

class TemplateName;
typedef Collection<std::shared_ptr<TemplateName>> TemplateNames;

class TemplateName {
 public:
  TemplateName(std::shared_ptr<PhysicsObject> physicsObject_) : physicsObject(physicsObject_) {}

 private:
  std::shared_ptr<PhysicsObject> physicsObject;
};

#endif /* TemplateName_hpp */
