//  Event.hpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#ifndef Event_hpp
#define Event_hpp

#include "Helpers.hpp"
#include "Multitype.hpp"
#include "PhysicsObject.hpp"

class Event {
 public:
  Event();
  ~Event();

  void Reset();

  inline std::shared_ptr<PhysicsObjects> GetCollection(std::string name) const {
    if (!collections.count(name)) {
      fatal() << "Tried to get a collection that doesn't exist: " << name << "\n";
      exit(1);
    }
    return collections.at(name);
  }

  inline auto Get(std::string branchName) {
    bool badBranch = false;

    if (valuesTypes.count(branchName) == 0) {
      error() << "Trying to access incorrect event-level branch: " << branchName << "\n";
      badBranch = true;
    }

    return Multitype(this, branchName, badBranch);
  }

 private:
  inline UInt_t GetUint(std::string branchName) { return valuesUint[branchName]; }
  inline Int_t GetInt(std::string branchName) { return valuesInt[branchName]; }
  inline Bool_t GetBool(std::string branchName) { return valuesBool[branchName]; }
  inline Float_t GetFloat(std::string branchName) { return valuesFloat[branchName]; }
  inline ULong64_t GetULong(std::string branchName) { return valuesUlong[branchName]; }
  inline UChar_t GetUChar(std::string branchName) { return valuesUchar[branchName]; }

  std::map<std::string, std::string> valuesTypes;  /// contains all branch names and corresponding types

  std::map<std::string, UInt_t> valuesUint;
  std::map<std::string, Int_t> valuesInt;
  std::map<std::string, Bool_t> valuesBool;
  std::map<std::string, Float_t> valuesFloat;
  std::map<std::string, ULong64_t> valuesUlong;
  std::map<std::string, UChar_t> valuesUchar;

  std::map<std::string, Int_t[maxCollectionElements]> valuesIntVector;
  std::map<std::string, Bool_t[maxCollectionElements]> valuesBoolVector;
  std::map<std::string, Float_t[maxCollectionElements]> valuesFloatVector;
  std::map<std::string, UChar_t[maxCollectionElements]> valuesUcharVector;

  std::map<std::string, std::shared_ptr<PhysicsObjects>> collections;

  friend class EventReader;
  template <typename T>
  friend class Multitype;
};

#endif /* Event_hpp */
