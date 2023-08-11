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
      if (motherIndex < 0) continue;
      auto mother = asGenParticle(genParticles->at(motherIndex));
      if (!genParticle->IsGoodBottomQuark(mother)) continue;
      bottomIndices.push_back(iGenParticle);
    }
  }
  return bottomIndices;
}

bool EventProcessor::IsGoodParticle(shared_ptr<Event> event, int particleIndex, vector<int> topIndices, vector<int> bottomIndices) {
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
    if (!ParticlesMotherInIndices(event, particleIndex, topIndices)) return false;

    // we don't want leptons coming from b decays
    if (ParticlesMotherInIndices(event, particleIndex, bottomIndices)) return false;

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
    if (!IsGoodParticle(event, iGenParticle, topIndices, bottomIndices)) continue;
    finalState.AddParticle(genParticle->GetPdgId());
  }

  if (!finalState.IsConsistent()) return "other";
  return finalState.GetShortName();
}

bool EventProcessor::ParticlesMotherInIndices(shared_ptr<Event> event, int particleIndex, vector<int> indices) {
  auto genParticles = event->GetCollection("GenPart");
  auto genParticle = asGenParticle(genParticles->at(particleIndex));
  int motherIndex = genParticle->GetMotherIndex();

  if (find(indices.begin(), indices.end(), motherIndex) != indices.end()) return true;
  if (motherIndex < 0) return false;

  return ParticlesMotherInIndices(event, motherIndex, indices);
}

bool EventProcessor::ParticleHasISRmotherAfterTopMother(shared_ptr<Event> event, int particleIndex) {
  auto genParticles = event->GetCollection("GenPart");
  auto genParticle = asGenParticle(genParticles->at(particleIndex));
  int motherIndex = genParticle->GetMotherIndex();

  auto mother = asGenParticle(genParticles->at(motherIndex));

  if (mother->IsTop()) return false;
  if (mother->IsJet()) return true;
  if (motherIndex < 0) return false;

  return ParticleHasISRmotherAfterTopMother(event, motherIndex);
}

float EventProcessor::GetMaxPt(shared_ptr<Event> event, string collectionName) {
  auto collection = event->GetCollection(collectionName);

  float maxPt = 0;
  for (auto element : *collection) {
    float pt = element->Get("pt");
    if (pt > maxPt) maxPt = pt;
  }
  return maxPt;
}

float EventProcessor::GetHt(shared_ptr<Event> event, string collectionName) {
  auto collection = event->GetCollection(collectionName);
  float ht = 0;
  for (auto element : *collection) {
    float pt = element->Get("pt");
    ht += pt;
  }
  return ht;
}

bool EventProcessor::PassesSingleLeptonSelections(const shared_ptr<Event> event) {
  int leptons_pt30 = 0;
  int leptons_pt15 = 0;
  int jets_btagged = 0;
  int jets_pt30;

  uint nMuons = event->Get("nMuon");
  auto muons = event->GetCollection("Muon");
  for (int i = 0; i < nMuons; i++) {
    float muonPt = muons->at(i)->Get("pt");
    float muonEta = muons->at(i)->Get("eta");
    if (muonPt > 30 && abs(muonEta) < 2.4)
      leptons_pt30++;
    else if (muonPt > 15 && abs(muonEta) < 2.5)
      leptons_pt15++;
  }
  uint nElectrons = event->Get("nElectron");
  auto electrons = event->GetCollection("Electron");
  for (int i = 0; i < nElectrons; i++) {
    float electronPt = electrons->at(i)->Get("pt");
    float electronEta = electrons->at(i)->Get("eta");
    if (electronPt > 30 && abs(electronEta) < 2.4)
      leptons_pt30++;
    else if (electronPt > 15 && abs(electronEta) < 2.5)
      leptons_pt15++;
  }
  uint nJets = event->Get("nJet");
  auto jets = event->GetCollection("Jet");
  for (int i = 0; i < nJets; i++) {
    float jetPt = jets->at(i)->Get("pt");
    float jetEta = jets->at(i)->Get("eta");
    float jetBtagDeepB = jets->at(i)->Get("btagDeepB");
    if (jetPt > 30 && abs(jetEta) < 2.4) {
      jets_pt30++;
      if (jetBtagDeepB > 0.5) jets_btagged++;
    }
  }

  if (leptons_pt30 != 1) return false;
  if (leptons_pt15 != 0) return false;
  float met_pt = event->Get("MET_pt");
  if (met_pt <= 30) return false;
  if (jets_btagged < 2) return false;
  if (jets_pt30 < 4) return false;
  return true;
}

bool EventProcessor::PassesDileptonSelections(const shared_ptr<Event> event) {
  int muonsPt30 = 0;
  int electronsPt30;
  int jetsBtagged = 0;

  uint nMuons = event->Get("nMuon");
  auto muons = event->GetCollection("Muon");
  for (int i = 0; i < nMuons; i++) {
    float muonPt = muons->at(i)->Get("pt");
    float muonEta = muons->at(i)->Get("eta");
    if (muonPt > 30 && abs(muonEta) < 2.4) muonsPt30++;
  }
  uint nElectrons = event->Get("nElectron");
  auto electrons = event->GetCollection("Electron");
  for (int i = 0; i < nElectrons; i++) {
    float electronPt = electrons->at(i)->Get("pt");
    float electronEta = electrons->at(i)->Get("eta");
    if (electronPt > 30 && abs(electronEta) < 2.4) electronsPt30++;
  }
  uint nJets = event->Get("nJet");
  auto jets = event->GetCollection("Jet");
  for (int i = 0; i < nJets; i++) {
    float jetPt = jets->at(i)->Get("pt");
    float jet_eta = jets->at(i)->Get("eta");
    float jet_btagDeepB = jets->at(i)->Get("btagDeepB");
    if (jetPt > 30 && abs(jet_eta) < 2.4 && jet_btagDeepB > 0.5) jetsBtagged++;
  }

  if ((muonsPt30 + electronsPt30) < 2) return false;
  float metPt = event->Get("MET_pt");
  if (metPt <= 30) return false;
  if (jetsBtagged < 2) return false;
  return true;
}

bool EventProcessor::PassesHadronSelections(const shared_ptr<Event> event) {
  int jetsBtagged = 0;
  int jetsPt30;

  uint nJets = event->Get("nJet");
  auto jets = event->GetCollection("Jet");
  for (int i = 0; i < nJets; i++) {
    float jetPt = jets->at(i)->Get("pt");
    float jetEta = jets->at(i)->Get("eta");
    float jetBtagDeepB = jets->at(i)->Get("btagDeepB");
    if (jetPt > 30 && abs(jetEta) < 2.4) {
      jetsPt30++;
      if (jetBtagDeepB > 0.5) jetsBtagged++;
    }
  }

  if (jetsBtagged < 2) return false;
  if (jetsPt30 < 6) return false;
  return true;
}