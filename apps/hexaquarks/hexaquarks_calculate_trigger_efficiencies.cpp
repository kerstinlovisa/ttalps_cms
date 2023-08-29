#include "ConfigManager.hpp"
#include "Event.hpp"
#include "EventProcessor.hpp"
#include "EventReader.hpp"
#include "ExtensionsHelpers.hpp"
#include "HexaquarksHistogramsFiller.hpp"
#include "HistogramsHandler.hpp"

using namespace std;

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

  for (int i_event = 0; i_event < eventReader->GetNevents() - 2; i_event++) {
    if (i_event % 100 == 0) cout << "Event: " << i_event << endl;
    auto event = eventReader->GetEvent(i_event);

    auto particles = event->GetCollection("Particle");

    vector<TLorentzVector> j, p, m;

    for (auto physObj : *particles) {
      auto particle = asHepMCParticle(physObj);

      if (particle->IsLastJPsi())
        j.push_back(particle->GetLorentzVector());
      else if (particle->IsLastPion() && particle->GetCharge() > 0)
        p.push_back(particle->GetLorentzVector());
      else if (particle->IsLastPion() && particle->GetCharge() < 0)
        m.push_back(particle->GetLorentzVector());
    }
    jPsi.push_back(j);
    piPlus.push_back(p);
    piMinus.push_back(m);
  }

  info() << "Filling histograms...\n";
  histogramsFiller->FillJpsiPiPiMinvHists(jPsi, piPlus, piMinus);

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