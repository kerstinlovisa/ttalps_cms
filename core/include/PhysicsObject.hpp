//  PhysicsObject.hpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#ifndef PhysicsObject_hpp
#define PhysicsObject_hpp

#include "Helpers.hpp"

class PhysicsObject;
typedef std::vector<std::shared_ptr<PhysicsObject>> PhysicsObjects;

class PhysicsObject
{
public:
  PhysicsObject();
  ~PhysicsObject();

  // double GetEta() const;
  // double GetPhi() const;
  // double GetEt() const;
  // double GetPt() const;

  inline auto Get(std::string branchName)
  {
    struct result
    {
      operator UInt_t() { return physicsObject->GetUint(branchName); }
      operator Int_t() { return physicsObject->GetInt(branchName); }
      operator Bool_t() { return physicsObject->GetBool(branchName); }
      operator Float_t() { return physicsObject->GetFloat(branchName); }
      operator ULong64_t() { return physicsObject->GetULong(branchName); }
      operator UChar_t() { return physicsObject->GetUChar(branchName); }

      PhysicsObject *physicsObject;
      std::string branchName;
    };
    return result{this, branchName};
  }

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
};

#endif /* PhysicsObject_hpp */
