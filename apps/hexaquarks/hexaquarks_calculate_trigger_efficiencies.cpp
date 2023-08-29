#include "ConfigManager.hpp"
#include "Event.hpp"
#include "EventProcessor.hpp"
#include "EventReader.hpp"
#include "ExtensionsHelpers.hpp"
#include "HistogramsFiller.hpp"
#include "HistogramsHandler.hpp"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    fatal() << "Usage: " << argv[0] << " config_path\n";
    exit(1);
  }

  string configPath = argv[1];

  auto histogramsHandler = make_shared<HistogramsHandler>(configPath);
  // auto histogramsFiller = make_unique<HistogramsFiller>(configPath, histogramsHandler);
  auto eventReader = make_unique<EventReader>(configPath);
  auto eventProcessor = make_unique<EventProcessor>();

  histogramsHandler->SetupHistograms();

  int pidJpsi = 443;
  int pidPion = 211;
  int pidHexa = 511;

  int nJpsi = 0;
  int nPi = 0;

  auto hist = new TH1D("hist", "hist", 1000, 0, 10);
  auto histMixed = new TH1D("histMixed", "histMixed", 1000, 0, 10);

  vector<TLorentzVector> jpsisForMixing, pionsPlusForMixing, pionsMinusForMixing;

  int mixingCounter = 0;

  for (int i_event = 0; i_event < eventReader->GetNevents(); i_event++) {
    if (i_event % 1000 == 0) cout << "Event: " << i_event << endl;
    auto event = eventReader->GetEvent(i_event);

    auto particles = event->GetCollection("Particle");

    vector<TLorentzVector> jpsiVectors, pionPlusVectors, pionMinusVectors;

    for (auto physObj : *particles) {
      auto particle = asHepMCParticle(physObj);

      int pid = particle->GetPid();
      int status = particle->GetStatus();
      auto vector = particle->GetLorentzVector();

      if ((abs(pid) == pidJpsi && status == 2) || (abs(pid) == pidPion && status == 1)) {
        if (abs(pid) == pidJpsi) {
          jpsiVectors.push_back(vector);
        } else if (pid > 0) {
          pionPlusVectors.push_back(vector);
        } else {
          pionMinusVectors.push_back(vector);
        }
      }
    }

    if (mixingCounter == 0) {
      if (jpsiVectors.size() > 0) {
        jpsisForMixing.push_back(jpsiVectors[0]);
        mixingCounter++;
      }
    } else if (mixingCounter == 1) {
      if (pionPlusVectors.size() > 0) {
        pionsPlusForMixing.push_back(pionPlusVectors[0]);
        mixingCounter++;
      }
    } else if (mixingCounter == 2) {
      if (pionMinusVectors.size() > 0) {
        pionsMinusForMixing.push_back(pionMinusVectors[0]);
        mixingCounter = 0;
      }
    }

    // string ttbarCategory = eventProcessor->GetTTbarEventCategory(event);
    // histogramsFiller->FillTriggerVariables(event, "inclusive");
    // histogramsFiller->FillTriggerVariables(event, ttbarCategory);
    // histogramsFiller->FillTriggerVariablesPerTriggerSet(event, "inclusive");
    // histogramsFiller->FillTriggerVariablesPerTriggerSet(event, ttbarCategory);

    for (auto jpsi : jpsiVectors) {
      for (auto pionPlus : pionPlusVectors) {
        for (auto pionMinus : pionPlusVectors) {
          if (jpsi.DeltaR(pionPlus) < 0.8 && jpsi.DeltaR(pionMinus) < 0.8 && pionMinus.DeltaR(pionPlus) < 0.8) {
            auto sum = jpsi + pionPlus + pionMinus;
            hist->Fill(sum.M());
          }
        }
      }
    }
  }

  
  for (auto jpsi : jpsisForMixing) {
    for (auto pionPlus : pionsPlusForMixing) {
      for (auto pionMinus : pionsMinusForMixing) {
        if (jpsi.DeltaR(pionPlus) < 0.8 && jpsi.DeltaR(pionMinus) < 0.8 && pionMinus.DeltaR(pionPlus) < 0.8) {
          auto sum = jpsi + pionPlus + pionMinus;
          histMixed->Fill(sum.M());
        }
      }
    }
  }
  auto histMixedScaled = new TH1D(*histMixed);
  histMixedScaled->Scale(hist->GetEntries() / histMixed->GetEntries());

  auto file = new TFile("hexa_test.root", "recreate");
  file->cd();
  hist->Write();
  histMixed->Write();
  histMixedScaled->Write();

  auto histDiff = new TH1D(*hist);
  histDiff->SetName("diff");
  histDiff->SetTitle("diff");
  histDiff->Add(histMixedScaled, -1);
  histDiff->Write();

  file->Close();

  // histogramsFiller->FillTriggerEfficiencies();
  histogramsHandler->SaveHistograms();

  return 0;
}