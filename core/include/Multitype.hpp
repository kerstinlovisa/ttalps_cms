//  Multitype.hpp
//
//  Created by Jeremi Niedziela on 07/08/2023.

#ifndef Multitype_hpp
#define Multitype_hpp

#include "Helpers.hpp"

template <typename T>
class Multitype
{
public:
    Multitype(T *object_, std::string branchName_, bool badBranch_) : object(object_), branchName(branchName_), badBranch(badBranch_){}
    operator UInt_t()
    {
        if (badBranch || !isCorrectType("UInt_t"))
            return 0;
        return object->GetUint(branchName);
    }
    operator Int_t()
    {
        if (badBranch || !isCorrectType("Int_t"))
            return 0;
        return object->GetInt(branchName);
    }
    operator Bool_t()
    {
        if (badBranch || !isCorrectType("Bool_t"))
            return 0;
        return object->GetBool(branchName);
    }
    operator Float_t()
    {
        if (badBranch || !isCorrectType("Float_t"))
            return 0;
        return object->GetFloat(branchName);
        ;
    }
    operator ULong64_t()
    {
        if (badBranch || !isCorrectType("ULong64_t"))
            return 0;
        return object->GetULong(branchName);
    }
    operator UChar_t()
    {
        if (badBranch || !isCorrectType("UChar_t"))
            return 0;
        return object->GetUChar(branchName);
    }

private:
    T *object;
    std::string branchName;
    bool badBranch;

    bool isCorrectType(std::string typeName)
    {
        std::string branchType = object->values_types.at(branchName);
        if (branchType != typeName)
        {
            Warn() << "Casting a physics object-level branch " << branchName << " (" << branchType << ") to " << typeName << "\n";
            return false;
        }
        return true;
    }
};

#endif /* Multitype_hpp */