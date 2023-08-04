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
  
  // EDataset dataset; ///< Dataset type (Data, MC_QED, MC_CEP etc.)
  
  std::shared_ptr<Event> currentEvent; ///< Pointer to the current event
  
  // Handles to variables stored in ntuple trees
  // std::map<ETrigger, int>    triggerValues;
  
  std::vector<float> *mcEta                  = nullptr;
  std::vector<float> *mcPhi                  = nullptr;
  std::vector<float> *mcEt                   = nullptr;
  std::vector<float> *mcE                    = nullptr;
  std::vector<float> *mcPID                  = nullptr;
  
  std::vector<int>   *electronCharge         = nullptr;
  std::vector<int>   *electronNmissing       = nullptr;
  std::vector<float> *electronPt             = nullptr;
  std::vector<float> *electronEta            = nullptr;
  std::vector<float> *electronPhi            = nullptr;
  std::vector<float> *electronHoverE         = nullptr;
  std::vector<float> *electronEoverP         = nullptr;
  std::vector<float> *electronRelIsoWithEA   = nullptr;
  std::vector<float> *electronDetaSeed       = nullptr;
  std::vector<float> *electronSCEta          = nullptr;
  std::vector<float> *electronSCEt           = nullptr;
  std::vector<float> *electronSCPhi          = nullptr;
  std::vector<float> *electronEn             = nullptr;
  std::vector<float> *electronSCEn           = nullptr;
  std::vector<float> *electronChIso          = nullptr;
  std::vector<float> *electronPhoIso         = nullptr;
  std::vector<float> *electronNeuIso         = nullptr;
  std::vector<int>   *electronPassesConvVeto = nullptr;
  
  std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double>>> *CastorTower_p4  = nullptr;
  
  std::map<EPhysObjType, int> nPhysObjects;     ///< Stores number of physics objects
  unsigned short int nL1EGs;
  
  uint runNumber = 0;
  uint lumiSection = 0;
  ULong64_t eventNumber = 0;
 /////////////////////
  int nDisplacedTracks = 0;
  int nPixelClusters = 0;
  int nPixelRecHits = 0;
  int nDedxHits = 0;
  
  std::map<std::string, UInt_t> values_uint;
  std::map<std::string, float[maxCollectionElements]> values_float_vector;

  /// Opens input trees and sets branches
  void SetupBranches(std::string inputPath, std::vector<std::string> outputPaths);
  
  /// Creates output file and copies intput trees to it (without copying the entries)
  void SetupOutputTree(std::string outFileName);
};

#endif /* EventReader_hpp */
