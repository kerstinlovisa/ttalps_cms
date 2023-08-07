//  EventReader.hpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#ifndef EventReader_hpp
#define EventReader_hpp

#include "Helpers.hpp"
#include "Event.hpp"

/// This class keeps a link to input ROOT trees and creates Event class objects when moving to next event.
class EventReader {
public:
  /// Default constructor.
  /// \param inputPath Path to the file
  /// \param outputPaths Paths to the output files, in case one wants to save selected events to a new tree(s)
  EventReader(std::string inputPath, std::vector<std::string> outputPaths = {});
  
  /// Default destructor
  ~EventReader();

  /// Add event with number iEvent to output tree in file outFileName
  void AddEventToOutputTree(int iEvent, std::string outFileName, bool saveHLTtree=true);
  
  /// Save file with name outFileName
  void SaveOutputTree(std::string outFileName);
  
  /// Returns number of events
  inline long long GetNevents() const { return eventTree->GetEntries(); }
  
  /// Returns Event object for given tree entry index
  std::shared_ptr<Event> GetEvent(int iEvent);
  
private:
  TTree *eventTree, *hltTree, *l1Tree, *zdcTree, *pixelTree;                          ///< Input trees

  std::map<std::string, TTree*> inputTrees;

  std::map<std::string, TTree*> outEventTree, outHltTree, outL1Tree, outZdcTree, outPixelTree;  ///< Output trees
  std::map<std::string, TDirectory*> dirEvent, dirHLT, dirL1, dirZdc, dirPixel;                 ///< Output directories
  std::map<std::string, TFile*> outFile;                                                        ///< Output files
  
  std::shared_ptr<Event> currentEvent; ///< Pointer to the current event
  
  /// Opens input trees and sets branches
  void SetupBranches(std::string inputPath, std::vector<std::string> outputPaths);
  
  /// Creates output file and copies intput trees to it (without copying the entries)
  void SetupOutputTree(std::string outFileName);
};

#endif /* EventReader_hpp */
