//  EventProcessor.cpp
//
//  Created by Jeremi Niedziela on 08/08/2023.

#include "EventProcessor.hpp"

#include "ExtensionsHelpers.hpp"

using namespace std;

vector<int> EventProcessor::GetTopIndices(shared_ptr<Event> event) {
  vector<int> topIndices = {-1, -1};

  auto genParticles = event->GetCollection("GenPart");

  int iGenParticle = -1;

  for (auto physicsObject : *genParticles) {
    iGenParticle++;
    auto genParticle = asGenParticle(physicsObject);

    if (!genParticle->IsLastCopy()) continue;
    if (genParticle->GetPdgId() == 6) topIndices[0] = iGenParticle;
    if (genParticle->GetPdgId() == -6) topIndices[1] = iGenParticle;
    if (topIndices[0] >= 0 && topIndices[1] >= 0) break;
  }
  return topIndices;
}

vector<int> EventProcessor::GetBottomIndices(shared_ptr<Event> event) {
  vector<int> bottomIndices;

  auto genParticles = event->GetCollection("GenPart");

  int iGenParticle = -1;

  for (auto physicsObject : *genParticles) {
    iGenParticle++;
    auto genParticle = asGenParticle(physicsObject);

    if (abs(genParticle->GetPdgId()) == 5) {
      int motherIndex = genParticle->GetMotherIndex();
      auto mother = asGenParticle(genParticles->at(motherIndex));
      if (!genParticle->IsGoodBottomQuark(mother)) continue;
      bottomIndices.push_back(iGenParticle);
    }
  }
  return bottomIndices;
}

bool EventProcessor::IsGoodParticle(shared_ptr<Event> event, int particleIndex,
                                    vector<int> topIndices,
                                    vector<int> bottomIndices) {
  auto genParticles = event->GetCollection("GenPart");
  auto particle = asGenParticle(genParticles->at(particleIndex));

  int motherIndex = particle->GetMotherIndex();
  if (motherIndex < 0) return false;
  auto mother = asGenParticle(genParticles->at(motherIndex));

  int pid = particle->GetPdgId();

  if (abs(pid) == 5) {  // b quark
    if (!particle->IsGoodBottomQuark(mother)) return false;
  } else if (abs(pid) >= 1 && abs(pid) <= 4) {  // light quark
    if (!particle->IsGoodUdscQuark(mother)) return false;
  } else {  // leptons
    if (!particle->IsGoodLepton(mother)) return false;

    // we want to make sure it comes from a top
    if (!ParticlesMotherInIndices(event, particleIndex, topIndices))
      return false;

    // we don't want leptons coming from b decays
    if (ParticlesMotherInIndices(event, particleIndex, bottomIndices))
      return false;

    // no gluons/jets after the top
    if (ParticleHasISRmotherAfterTopMother(event, particleIndex)) return false;
  }

  return true;
}

string EventProcessor::GetTTbarEventCategory(shared_ptr<Event> event) {
  vector<int> topIndices = GetTopIndices(event);

  if (topIndices[0] < 0 || topIndices[1] < 0) {
    cout << "ERROR -- couldn't find tt̄ pair in the event..." << endl;
    return "error";
  }

  auto finalState = FinalState();

  vector<int> bottomIndices = GetBottomIndices(event);

  auto genParticles = event->GetCollection("GenPart");

  int iGenParticle = -1;
  for (auto physicsObject : *genParticles) {
    iGenParticle++;
    auto genParticle = asGenParticle(physicsObject);
    if (!IsGoodParticle(event, iGenParticle, topIndices, bottomIndices))
      continue;
    finalState.AddParticle(genParticle->GetPdgId());
  }

  if (!finalState.IsConsistent()) {
    error() << "Final state is not consistent with tt̄ event!\n";
    finalState.print();
    return "error";
  }
  return finalState.GetShortName();
}

bool EventProcessor::ParticlesMotherInIndices(shared_ptr<Event> event,
                                             int particleIndex,
                                             vector<int> indices) {
  auto genParticles = event->GetCollection("GenPart");
  auto genParticle = asGenParticle(genParticles->at(particleIndex));
  int motherIndex = genParticle->GetMotherIndex();

  if (find(indices.begin(), indices.end(), motherIndex) != indices.end())
    return true;
  if (motherIndex < 0) return false;

  return ParticlesMotherInIndices(event, motherIndex, indices);
}

bool EventProcessor::ParticleHasISRmotherAfterTopMother(shared_ptr<Event> event,
                                                        int particleIndex) {
  auto genParticles = event->GetCollection("GenPart");
  auto genParticle = asGenParticle(genParticles->at(particleIndex));
  int motherIndex = genParticle->GetMotherIndex();

  auto mother = asGenParticle(genParticles->at(motherIndex));

  if (mother->IsTop()) return false;
  if (mother->IsJet()) return true;
  if (motherIndex < 0) return false;

  return ParticleHasISRmotherAfterTopMother(event, motherIndex);
}