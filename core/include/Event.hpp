//  Event.hpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#ifndef Event_hpp
#define Event_hpp

#include <vector>

#include "Helpers.hpp"
#include "PhysicsObject.hpp"
// #include "ConfigManager.hpp"

enum class EPhysObjType {
  kGenParticle,
  kPhoton,
  kCaloTower,
  kCastor,
  kGeneralTrack,
  kElectron,
  kMuon,
  kL1EG,
  kPixelTrack,
  kZDC,
  kVertex,
};

const std::vector<EPhysObjType> physObjTypes = {
  EPhysObjType::kGenParticle,
  EPhysObjType::kPhoton,
  EPhysObjType::kCaloTower,
  EPhysObjType::kCastor,
  EPhysObjType::kGeneralTrack,
  EPhysObjType::kElectron,
  EPhysObjType::kMuon,
  EPhysObjType::kL1EG,
  EPhysObjType::kPixelTrack,
  EPhysObjType::kZDC,
  EPhysObjType::kVertex,
};

/// This class represents a single event with all its gen-level and reconstructed objects,
/// as well as triggers and other event-wide properties
class Event
{
public:
  Event();
  ~Event();

  /// Removes all physics objects from the event, resets all flags
  void Reset();

  /// Returns a vector of physics objects
  /// \param type Defines which type of physics objects to return (as defined in Helpers.hpp)
  // PhysObjects GetPhysObjects(EPhysObjType type);

  inline uint GetRunNumber() const { return runNumber; }
  inline uint GetLumiSection() const { return lumiSection; }
  inline ULong64_t GetEventNumber() const { return eventNumber; }

  inline int GetNdisplacedTracks() const { return nDisplacedTracks; }
  inline int GetNpixelClusters() const { return nPixelClusters; }
  inline int GetNpixelRecHits() const { return nPixelRecHits; }
  inline int GetNdedxHits() const { return nDedxHits; }

  inline UInt_t GetUint(std::string branchName){return values_uint[branchName];}
  inline std::shared_ptr<PhysicsObjects> GetCollection(std::string name){ return collections[name];}

  // inline std::map<std::string, UInt_t> GetUintVariables(){return values_uint;}
  // inline std::map<std::string, float[maxCollectionElements] > GetFloatVectorVariables(){return values_float_vector;}

private:
  // std::map<ETrigger, bool> triggerValues; ///< Vactor of booleans corresponding to LbL triggers

  // std::map<EPhysObjType, PhysObjects> physObjects; ///< Map of physics objects in the event
  std::map<EPhysObjType, bool> physObjectsReady;   ///< Is collection ready?

  // EDataset dataset;

  int nDisplacedTracks;
  int nPixelClusters;
  int nPixelRecHits;
  int nDedxHits;

  uint runNumber;
  uint lumiSection;
  ULong64_t eventNumber;

  std::map<std::string, UInt_t> values_uint;
  std::map<std::string, float[maxCollectionElements]> values_float_vector;

  std::map<std::string, std::shared_ptr<PhysicsObjects>> collections;

  friend class EventReader;
};

#endif /* Event_hpp */
