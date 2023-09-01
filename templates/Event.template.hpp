#ifndef TemplateName_hpp
#define TemplateName_hpp

#include "Event.hpp"
#include "Helpers.hpp"

class TemplateName {
 public:
  TemplateName(std::shared_ptr<Event> event_) : event(event_) {}

  
 private:
  std::shared_ptr<Event> event;

};

#endif /* TemplateName_hpp */
