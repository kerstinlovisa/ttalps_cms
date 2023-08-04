//  Event.hpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#ifndef Event_hpp
#define Event_hpp

#include <vector>

#include "Helpers.hpp"
#include "PhysicsObject.hpp"
// #include "ConfigManager.hpp"

/// This class represents a single event with all its gen-level and reconstructed objects,
/// as well as triggers and other event-wide properties
class Event
{
public:
  Event();
  ~Event();

  /// Removes all physics objects from the event, resets all flags
  void Reset();


  inline UInt_t GetUint(std::string branchName){return values_uint[branchName];}
  inline UInt_t GetInt(std::string branchName){return values_int[branchName];}
  inline Bool_t GetBool(std::string branchName){return values_bool[branchName];}
  inline std::shared_ptr<PhysicsObjects> GetCollection(std::string name){ return collections[name];}


private:
  std::map<std::string, UInt_t> values_uint;
  std::map<std::string, Int_t> values_int;
  std::map<std::string, Bool_t> values_bool;
  std::map<std::string, Float_t> values_float;
  std::map<std::string, ULong64_t> values_ulong;
  std::map<std::string, UChar_t> values_uchar;

  std::map<std::string, Float_t[maxCollectionElements]> values_float_vector;
  std::map<std::string, UChar_t[maxCollectionElements]> values_uchar_vector;
  std::map<std::string, Int_t[maxCollectionElements]> values_int_vector;
  std::map<std::string, Bool_t[maxCollectionElements]> values_bool_vector;

  std::map<std::string, std::shared_ptr<PhysicsObjects>> collections;

  friend class EventReader;
};

#endif /* Event_hpp */
