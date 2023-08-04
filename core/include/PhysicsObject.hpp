//  PhysicsObject.hpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#ifndef PhysicsObject_hpp
#define PhysicsObject_hpp

#include "Helpers.hpp"

class PhysicsObject;
typedef std::vector<std::shared_ptr<PhysicsObject>> PhysicsObjects;

/// Class representing a physics object, such as gen particle, reconstructed photon/electron,
/// calorimeter tower, supercluster etc.
class PhysicsObject
{
public:
  /// Default constructor
  PhysicsObject();

  /// Default destructor
  ~PhysicsObject();

  // Trivial getters:
  double GetEta() const;
  double GetPhi() const;
  double GetEt() const;
  double GetPt() const;

  inline float GetFloat(std::string name){return *values_float[name];}

private:
  double eta;    ///< Pseudorapidity
  double phi;    ///< Azimuthal angle
  double energy; ///< Energy

  std::map<std::string, UInt_t> values_uint;
  std::map<std::string, float*> values_float;

  friend class EventReader;
};

#endif /* PhysicsObject_hpp */
