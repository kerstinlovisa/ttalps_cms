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
  TFile *inputFile;
  std::map<std::string, TTree *> inputTrees;
  std::shared_ptr<Event> currentEvent;

  void SetupBranches(std::string inputPath);

  void SetupScalarBranch(std::string branchName, std::string branchType);
  void SetupVectorBranch(std::string branchName, std::string branchType);
  void InitializeCollection(std::string collectionName);

  friend class EventWriter;
  friend class CutFlowManager;
};

#endif /* EventReader_hpp */
