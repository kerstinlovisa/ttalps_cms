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

  
  double GetEta() const;
  double GetPhi() const;
  double GetEt() const;
  double GetPt() const;

  inline float GetFloat(std::string name){return *values_float[name];}
  inline float GetInt(std::string name){return *values_int[name];}

private:
  std::map<std::string, UInt_t> values_uint;
  std::map<std::string, Float_t*> values_float;
  std::map<std::string, UChar_t*> values_uchar;
  std::map<std::string, Int_t*> values_int;
  std::map<std::string, Bool_t*> values_bool;

  friend class EventReader;
};

#endif /* PhysicsObject_hpp */
