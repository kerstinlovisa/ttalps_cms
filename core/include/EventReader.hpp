//  EventReader.hpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#ifndef EventReader_hpp
#define EventReader_hpp

#include "Event.hpp"
#include "Helpers.hpp"

class EventReader {
public:
  EventReader(std::string inputPath);
  ~EventReader();

  inline long long GetNevents() const {
    return inputTrees.at("Events")->GetEntries();
  }
  std::shared_ptr<Event> GetEvent(int iEvent);

private:
  std::map<std::string, TTree *> inputTrees;
  std::shared_ptr<Event> currentEvent;

  void SetupBranches(std::string inputPath);

  friend class EventWriter;
};

#endif /* EventReader_hpp */
