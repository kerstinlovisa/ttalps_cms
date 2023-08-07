//  PhysicsObject.hpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#ifndef PhysicsObject_hpp
#define PhysicsObject_hpp

#include "Helpers.hpp"
#include "Multitype.hpp"
#include "Collection.hpp"


class PhysicsObject;
typedef Collection<std::shared_ptr<PhysicsObject>> PhysicsObjects;

class PhysicsObject
{
public:
  PhysicsObject();
  ~PhysicsObject();

  void Reset();

  inline auto Get(std::string branchName)
  {
    bool badBranch = false;

    if (values_types.count(branchName) == 0)
    {
      Err() << "Trying to access incorrect physics object-level branch: " << branchName<<"\n";
      badBranch = true;
    }

    return Multitype(this, branchName, badBranch);
  }

  std::map<std::string, std::string> values_types; /// contains all branch names and corresponding types

private:
  inline UInt_t GetUint(std::string branchName) { return *values_uint[branchName]; }
  inline UInt_t GetInt(std::string branchName) { return *values_int[branchName]; }
  inline Bool_t GetBool(std::string branchName) { return *values_bool[branchName]; }
  inline Float_t GetFloat(std::string branchName) { return *values_float[branchName]; }
  inline ULong64_t GetULong(std::string branchName) { return *values_ulong[branchName]; }
  inline UChar_t GetUChar(std::string branchName) { return *values_uchar[branchName]; }

  std::map<std::string, UInt_t *> values_uint;
  std::map<std::string, Int_t *> values_int;
  std::map<std::string, Bool_t *> values_bool;
  std::map<std::string, Float_t *> values_float;
  std::map<std::string, ULong64_t *> values_ulong;
  std::map<std::string, UChar_t *> values_uchar;

  friend class EventReader;
  template <typename T> friend class Multitype;
};

#endif /* PhysicsObject_hpp */
