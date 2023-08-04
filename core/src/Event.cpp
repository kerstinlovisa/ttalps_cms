//  Event.cpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#include "Event.hpp"
// #include "PhysObjectProcessor.hpp"
// #include "Logger.hpp"

Event::Event()
{
  for (auto type : physObjTypes)
  {
    physObjectsReady[type] = false;
    // physObjects[type] = PhysObjects();
  }
  nDisplacedTracks = 0;
  nPixelClusters = 0;
  nPixelRecHits = 0;
  nDedxHits = 0;
}

Event::~Event()
{
}

void Event::Reset()
{
  // for (auto &[type, objects] : physObjects) objects.clear();
  for (auto &[type, ready] : physObjectsReady) ready = false;

  for(auto &[key, value] : values_uint) value = 0;
}

// bool Event::HasTrigger(ETrigger trigger) const
// {
//   return triggerValues.at(trigger);
// }

// PhysObjects Event::GetPhysObjects(EPhysObjType type)
// {
//   return physObjects.at(type);
// }
