#include "ConfigManager.hpp"
#include "Event.hpp"
#include "EventProcessor.hpp"
#include "EventReader.hpp"
#include "ExtensionsHelpers.hpp"
#include "HexaquarksHistogramsFiller.hpp"
#include "HistogramsHandler.hpp"
#include "Profiler.hpp"
#include "HepMCProcessor.hpp"

using namespace std;

void SetupMothers(HepMCParticles &particles) {
  for (int particleIndex = 0; particleIndex < particles.size(); particleIndex++) {
    auto particle = particles[particleIndex];
    for (int daughterIndex : particle->GetDaughters()) {
      if (daughterIndex < 0) continue;
      particles[daughterIndex]->SetMother(particleIndex);
      particles[daughterIndex]->AddMother(particleIndex);
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fatal() << "Usage: " << argv[0] << " config_path\n";
    exit(1);
  }

  string configPath = argv[1];

  auto histogramsHandler = make_shared<HistogramsHandler>(configPath);
  auto histogramsFiller = make_unique<HexaquarksHistogramsFiller>(configPath, histogramsHandler);
  auto eventReader = make_unique<EventReader>(configPath);
  auto eventProcessor = make_unique<EventProcessor>();
  auto configManager = make_unique<ConfigManager>(configPath);
  auto hepMCProcessor = make_unique<HepMCProcessor>();

  Profiler &profiler = Profiler::GetInstance();

  histogramsHandler->SetupHistograms();

  int nMixedEvents;
  configManager->GetValue("nMixedEvents", nMixedEvents);
  info() << "Requested number of mixed entries: " << nMixedEvents << "\n";

  info() << "Storing interesting particles' vectors...\n";
  vector<vector<TLorentzVector>> jPsi, piPlus, piMinus;
  vector<vector<TLorentzVector>> jPsiFromHexa, piPlusFromHexa, piMinusFromHexa;

  int maxNdaughters;
  configManager->GetValue("maxNdaughters", maxNdaughters);

  for (int i_event = 0; i_event < eventReader->GetNevents(); i_event++) {
    auto event = eventReader->GetEvent(i_event);

    auto particles = event->GetCollection("Particle");
    vector<TLorentzVector> j, p, m, jFromHexa, pFromHexa, mFromHexa;
    HepMCParticles hepMCparticles;

    int index = 0;
    for (auto physObj : *particles) {
      auto particle = asHepMCParticle(physObj, index++, maxNdaughters);
      hepMCparticles.push_back(particle);
    }

    SetupMothers(hepMCparticles);

    for (auto particle : hepMCparticles) {
      bool fromHexa = false;
      
      if (particle->IsJPsi() || particle->IsPion()) {
        if(!hepMCProcessor->IsLastCopy(particle, hepMCparticles)) continue;
        fromHexa = particle->IsMother(511, hepMCparticles) || particle->IsMother(521, hepMCparticles);
      }

      if (particle->IsJPsi()) {
        j.push_back(particle->GetLorentzVector());
        if (fromHexa) jFromHexa.push_back(particle->GetLorentzVector());

      } else if (particle->IsPion() && particle->GetCharge() > 0) {
        p.push_back(particle->GetLorentzVector());
        if (fromHexa) pFromHexa.push_back(particle->GetLorentzVector());

      } else if (particle->IsPion() && particle->GetCharge() < 0) {
        m.push_back(particle->GetLorentzVector());
        if (fromHexa) mFromHexa.push_back(particle->GetLorentzVector());
      }
    }

    jPsi.push_back(j);
    piPlus.push_back(p);
    piMinus.push_back(m);
    jPsiFromHexa.push_back(jFromHexa);
    piPlusFromHexa.push_back(pFromHexa);
    piMinusFromHexa.push_back(mFromHexa);
  }

  info() << "Filling real histograms" << endl;
  histogramsFiller->FillMinvHists(jPsi, piPlus, piMinus, "m_inv");
  histogramsFiller->FillMinvHists(jPsiFromHexa, piPlusFromHexa, piMinusFromHexa, "m_inv_from_hexa");

  histogramsFiller->FillDeltaHists(piPlus, piMinus, "delta_eta_pi_pi");
  histogramsFiller->FillDeltaHists(jPsi, piPlus, "delta_eta_jPsi_pi");
  histogramsFiller->FillDeltaHists(jPsi, piMinus, "delta_eta_jPsi_pi");

  histogramsFiller->FillDeltaHists(piPlus, piMinus, "delta_phi_pi_pi");
  histogramsFiller->FillDeltaHists(jPsi, piPlus, "delta_phi_jPsi_pi");
  histogramsFiller->FillDeltaHists(jPsi, piMinus, "delta_phi_jPsi_pi");

  histogramsFiller->FillDeltaHists(piPlus, piMinus, "delta_r_pi_pi");
  histogramsFiller->FillDeltaHists(jPsi, piPlus, "delta_r_jPsi_pi");
  histogramsFiller->FillDeltaHists(jPsi, piMinus, "delta_r_jPsi_pi");

  histogramsFiller->FillPtHists(jPsi, "pt_jPsi");
  histogramsFiller->FillPtHists(piPlus, "pt_pi");
  histogramsFiller->FillPtHists(piMinus, "pt_pi");

  histogramsFiller->FillDeltaHists(piPlusFromHexa, piMinusFromHexa, "delta_eta_pi_pi_from_hexa");
  histogramsFiller->FillDeltaHists(jPsiFromHexa, piPlusFromHexa, "delta_eta_jPsi_pi_from_hexa");
  histogramsFiller->FillDeltaHists(jPsiFromHexa, piMinusFromHexa, "delta_eta_jPsi_pi_from_hexa");

  histogramsFiller->FillDeltaHists(piPlusFromHexa, piMinusFromHexa, "delta_phi_pi_pi_from_hexa");
  histogramsFiller->FillDeltaHists(jPsiFromHexa, piPlusFromHexa, "delta_phi_jPsi_pi_from_hexa");
  histogramsFiller->FillDeltaHists(jPsiFromHexa, piMinusFromHexa, "delta_phi_jPsi_pi_from_hexa");

  histogramsFiller->FillDeltaHists(piPlusFromHexa, piMinusFromHexa, "delta_r_pi_pi_from_hexa");
  histogramsFiller->FillDeltaHists(jPsiFromHexa, piPlusFromHexa, "delta_r_jPsi_pi_from_hexa");
  histogramsFiller->FillDeltaHists(jPsiFromHexa, piMinusFromHexa, "delta_r_jPsi_pi_from_hexa");

  histogramsFiller->FillPtHists(jPsiFromHexa, "pt_jPsi_from_hexa");
  histogramsFiller->FillPtHists(piPlusFromHexa, "pt_pi_from_hexa");
  histogramsFiller->FillPtHists(piMinusFromHexa, "pt_pi_from_hexa");


  bool mixingFinished = false;
  long long nMixedEntries = 0;  

  info() << "Collecting particles for mixing..." << endl;
  vector<vector<TLorentzVector>> jForMixing, pForMixing, mForMixing;

  while (!mixingFinished) {
    int jEventIndex = randInt(0, jPsi.size() - 1);
    int pEventIndex = randInt(0, piPlus.size() - 1);
    int mEventIndex = randInt(0, piMinus.size() - 1);

    if (jEventIndex == pEventIndex || jEventIndex == mEventIndex || pEventIndex == mEventIndex) continue;
    if (jPsi[jEventIndex].size() == 0 || piPlus[pEventIndex].size() == 0 || piMinus[mEventIndex].size() == 0) continue;

    int j = randInt(0, jPsi[jEventIndex].size() - 1);
    int p = randInt(0, piPlus[pEventIndex].size() - 1);
    int m = randInt(0, piMinus[mEventIndex].size() - 1);

    jForMixing.push_back({jPsi[jEventIndex][j]});
    pForMixing.push_back({piPlus[pEventIndex][p]});
    mForMixing.push_back({piMinus[mEventIndex][m]});

    nMixedEntries++;
    if (nMixedEntries % 10000 == 0) {
      info() << 100 * nMixedEntries / nMixedEvents << "%\r" << flush;
    }
    if (nMixedEntries == nMixedEvents) mixingFinished = true;
  }

  info() << "Filling mixed histograms...\n";
  histogramsFiller->FillMinvHists(jForMixing, pForMixing, mForMixing, "m_inv_mixed");

  profiler.Print();
  histogramsHandler->SaveHistograms();

  return 0;
}