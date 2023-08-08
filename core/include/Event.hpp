//  Event.hpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#ifndef Event_hpp
#define Event_hpp

#include "Helpers.hpp"
#include "Multitype.hpp"
#include "PhysicsObject.hpp"
// #include "ConfigManager.hpp"

class Event {
public:
  Event();
  ~Event();

  void Reset();

  inline std::shared_ptr<PhysicsObjects> GetCollection(std::string name) const {
    if (!collections.count(name)) {
      FatalErr() << "Tried to get a collection that doesn't exist: "; 
      FatalErr() << name << "\n";
      exit(1);
    }
    return collections.at(name);
  }

  inline auto Get(std::string branchName) {
    bool badBranch = false;

    if (values_types.count(branchName) == 0) {
      Err() << "Trying to access incorrect event-level branch: " << branchName
            << "\n";
      badBranch = true;
    }

    return Multitype(this, branchName, badBranch);
  }

private:
  inline UInt_t GetUint(std::string branchName) {
    return values_uint[branchName];
  }
  inline Int_t GetInt(std::string branchName) { return values_int[branchName]; }
  inline Bool_t GetBool(std::string branchName) {
    return values_bool[branchName];
  }
  inline Float_t GetFloat(std::string branchName) {
    return values_float[branchName];
  }
  inline ULong64_t GetULong(std::string branchName) {
    return values_ulong[branchName];
  }
  inline UChar_t GetUChar(std::string branchName) {
    return values_uchar[branchName];
  }

  std::map<std::string, std::string>
      values_types; /// contains all branch names and corresponding types

  std::map<std::string, UInt_t> values_uint;
  std::map<std::string, Int_t> values_int;
  std::map<std::string, Bool_t> values_bool;
  std::map<std::string, Float_t> values_float;
  std::map<std::string, ULong64_t> values_ulong;
  std::map<std::string, UChar_t> values_uchar;

  std::map<std::string, Int_t[maxCollectionElements]> values_int_vector;
  std::map<std::string, Bool_t[maxCollectionElements]> values_bool_vector;
  std::map<std::string, Float_t[maxCollectionElements]> values_float_vector;
  std::map<std::string, UChar_t[maxCollectionElements]> values_uchar_vector;

  std::map<std::string, std::shared_ptr<PhysicsObjects>> collections;

  friend class EventReader;
  template <typename T> friend class Multitype;
};

#endif /* Event_hpp */
