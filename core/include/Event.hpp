//  Event.hpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#ifndef Event_hpp
#define Event_hpp

#include <vector>

#include "Helpers.hpp"
#include "PhysicsObject.hpp"
// #include "ConfigManager.hpp"

#include <type_traits>

class Event
{
public:
  Event();
  ~Event();

  void Reset();

  inline std::shared_ptr<PhysicsObjects> GetCollection(std::string name) { return collections[name]; }

  inline auto Get(std::string branchName)
  {
    bool badBranch = false;

    if (values_types.count(branchName) == 0)
    {
      std::cout << "\033[1;31m ERROR -- you're trying to access incorrect event-level branch: " << branchName << "\033[0m"
                << std::endl;
                badBranch = true;
    }

    struct result
    {
      operator UInt_t()
      {
        if(badBranch) return 0;
        checkType("UInt_t");
        return event->GetUint(branchName);
      }
      operator Int_t()
      {
        if(badBranch) return 0;
        checkType("Int_t");
        return event->GetInt(branchName);
      }
      operator Bool_t()
      {
        if(badBranch) return 0;
        checkType("Bool_t");
        return event->GetBool(branchName);
      }
      operator Float_t()
      {
        if(badBranch) return 0;
        checkType("Float_t");
        return event->GetFloat(branchName);
      }
      operator ULong64_t()
      {
        if(badBranch) return 0;
        checkType("ULong64_t");
        return event->GetULong(branchName);
      }
      operator UChar_t()
      {
        if(badBranch) return 0;
        checkType("UChar_t");
        return event->GetUChar(branchName);
      }

      Event *event;
      std::string branchName;
      bool badBranch;
      
      void checkType(std::string typeName)
      {
        std::string branchType = event->values_types[branchName];
        if (branchType != typeName)
        {
          std::cout << "\033[1;33m WARNING -- you're trying to cast an event-level branch \"" << branchName << "\" (" << branchType << ") to " << typeName << "\033[0m"
                    << std::endl;
        }
      }
    };

    return result{this, branchName, badBranch};
  }

private:
  inline UInt_t GetUint(std::string branchName) { return values_uint[branchName]; }
  inline Int_t GetInt(std::string branchName) { return values_int[branchName]; }
  inline Bool_t GetBool(std::string branchName) { return values_bool[branchName]; }
  inline Float_t GetFloat(std::string branchName) { return values_float[branchName]; }
  inline ULong64_t GetULong(std::string branchName) { return values_ulong[branchName]; }
  inline UChar_t GetUChar(std::string branchName) { return values_uchar[branchName]; }

  std::map<std::string, std::string> values_types; /// contains all branch names and corresponding types

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
};

#endif /* Event_hpp */
