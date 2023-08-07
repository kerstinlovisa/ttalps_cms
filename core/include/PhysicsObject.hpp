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

  void Reset();

  inline auto Get(std::string branchName)
  {
    bool badBranch = false;

    if (values_types.count(branchName) == 0)
    {
      std::cout << "\033[1;31m ERROR -- you're trying to access incorrect physics object-level branch: " << branchName << "\033[0m"
                << std::endl;
      badBranch = true;
    }

    struct result
    {
      operator UInt_t()
      {
        if (badBranch || !isCorrectType("UInt_t"))return 0;
        return physicsObject->GetUint(branchName);
      }
      operator Int_t()
      {
        if (badBranch || !isCorrectType("Int_t"))return 0;
        return physicsObject->GetInt(branchName);
      }
      operator Bool_t()
      {
        if (badBranch || !isCorrectType("Bool_t"))return 0;
        return physicsObject->GetBool(branchName);
      }
      operator Float_t()
      {
        if (badBranch || !isCorrectType("Float_t"))return 0;
        return physicsObject->GetFloat(branchName);;
      }
      operator ULong64_t()
      {
        if (badBranch || !isCorrectType("ULong64_t"))return 0;
        return physicsObject->GetULong(branchName);
      }
      operator UChar_t()
      {
        if (badBranch || !isCorrectType("UChar_t"))return 0;
        return physicsObject->GetUChar(branchName);
      }

      PhysicsObject *physicsObject;
      std::string branchName;
      bool badBranch;

      bool isCorrectType(std::string typeName)
      {
        std::string branchType = physicsObject->values_types[branchName];
        if (branchType != typeName)
        {
          std::cout << "\033[1;33m WARNING -- you're trying to cast a physics object-level branch " << branchName << " (" << branchType << ") to " << typeName << "\033[0m"
                    << std::endl;
          return false;
        }
        return true;
      }
    };

    return result{this, branchName, badBranch};
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
};

#endif /* PhysicsObject_hpp */
