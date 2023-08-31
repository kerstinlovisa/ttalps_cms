#include "ConfigManager.hpp"
#include "Event.hpp"
#include "EventProcessor.hpp"
#include "EventReader.hpp"
#include "ExtensionsHelpers.hpp"
#include "HexaquarksHistogramsFiller.hpp"
#include "HistogramsHandler.hpp"

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

  histogramsHandler->SetupHistograms();

  info() << "Storing interesting particles' vectors...\n";
  vector<vector<TLorentzVector>> jPsi, piPlus, piMinus;
  vector<vector<TLorentzVector>> jPsiFromHexa, piPlusFromHexa, piMinusFromHexa;

  map<string, float> time = {
    {"setup_mothers", 0},
    {"fill_vectors", 0},
    {"prepare_mixing", 0},
  };

  for (int i_event = 0; i_event < eventReader->GetNevents(); i_event++) {
    if (i_event % 100 == 0) cout << "Event: " << i_event << endl;
    auto event = eventReader->GetEvent(i_event);

    auto particles = event->GetCollection("Particle");

    vector<TLorentzVector> j, p, m;
    vector<TLorentzVector> jFromHexa, pFromHexa, mFromHexa;
    HepMCParticles hepMCparticles;

    int index = 0;
    for (auto physObj : *particles) {
      auto particle = asHepMCParticle(physObj);
      particle->SetIndex(index++);
      hepMCparticles.push_back(particle);
    }


    auto t0 = now();
    SetupMothers(hepMCparticles);
    time["setup_mothers"] += duration(t0, now());

    t0 = now();
    for (auto particle : hepMCparticles) {
      bool fromHexa = false;
      if (particle->IsLastJPsi() || particle->IsLastPion()) {
        fromHexa = particle->IsMother(511, hepMCparticles) || particle->IsMother(521, hepMCparticles);
      }
      if (particle->IsLastJPsi()) {
        j.push_back(particle->GetLorentzVector());
        if (fromHexa) jFromHexa.push_back(particle->GetLorentzVector());

      } else if (particle->IsLastPion() && particle->GetCharge() > 0) {
        p.push_back(particle->GetLorentzVector());
        if (fromHexa) pFromHexa.push_back(particle->GetLorentzVector());

      } else if (particle->IsLastPion() && particle->GetCharge() < 0) {
        m.push_back(particle->GetLorentzVector());
        if (fromHexa) mFromHexa.push_back(particle->GetLorentzVector());
      }
    }
    time["fill_vectors"] += duration(t0, now());
    jPsi.push_back(j);
    piPlus.push_back(p);
    piMinus.push_back(m);

    jPsiFromHexa.push_back(jFromHexa);
    piPlusFromHexa.push_back(pFromHexa);
    piMinusFromHexa.push_back(mFromHexa);
  }

  info() << "Filling real histograms" << endl;
  histogramsFiller->FillJpsiPiPiMinvHists(jPsi, piPlus, piMinus, "m_inv");
  histogramsFiller->FillJpsiPiPiMinvHists(jPsiFromHexa, piPlusFromHexa, piMinusFromHexa, "m_inv_from_hexaquark");

  auto configManager = make_unique<ConfigManager>(configPath);
  int nMixedEvents;
  configManager->GetValue("nMixedEvents", nMixedEvents);

  bool mixingFinished = false;
  int nMixedEntries = 0;

  info() << "Requested number of mixed entries: " << nMixedEvents << "\n";

  info() << "Collecting particles for mixing..." << endl;
  vector<vector<TLorentzVector>> jForMixing, pForMixing, mForMixing;

  auto t0 = now();
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
  time["prepare_mixing"] += duration(t0, now());
  
  info() << "Filling mixed histograms...\n";
  histogramsFiller->FillJpsiPiPiMinvHists(jForMixing, pForMixing, mForMixing, "m_inv_mixed");


  for(auto &[name, t] : time) info()<<name<<": "<<t<<" (s)"<<endl;
  // auto histMixedScaled = new TH1D(*histMixed);
  // histMixedScaled->Scale(hist->GetEntries() / histMixed->GetEntries());

  // auto file = new TFile("hexa_test.root", "recreate");
  // file->cd();
  // hist->Write();
  // histMixed->Write();
  // histMixedScaled->Write();

  // auto histDiff = new TH1D(*hist);
  // histDiff->SetName("diff");
  // histDiff->SetTitle("diff");
  // histDiff->Add(histMixedScaled, -1);
  // histDiff->Write();

  // file->Close();

  // histogramsFiller->FillTriggerEfficiencies();
  histogramsHandler->SaveHistograms();

  return 0;
}