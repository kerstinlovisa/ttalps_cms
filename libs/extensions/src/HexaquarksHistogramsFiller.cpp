#include "HexaquarksHistogramsFiller.hpp"

#include "ConfigManager.hpp"
#include "ExtensionsHelpers.hpp"

using namespace std;

HexaquarksHistogramsFiller::HexaquarksHistogramsFiller(string configPath, shared_ptr<HistogramsHandler> histogramsHandler_)
    : histogramsHandler(histogramsHandler_) {
  auto configManager = make_unique<ConfigManager>(configPath);
  // configManager->GetValue("nMixedEventsScale", nMixedEventsScale);

  eventProcessor = make_unique<EventProcessor>();
}

HexaquarksHistogramsFiller::~HexaquarksHistogramsFiller() {}

void HexaquarksHistogramsFiller::FillJpsiPiPiMinvHists(vector<vector<TLorentzVector>> &particle0, vector<vector<TLorentzVector>> &particle1,
                                                       vector<vector<TLorentzVector>> &particle2, string histName) {
  
  histogramsHandler->CheckHistogram(histName);
  
  int nEvents = particle0.size();
  
  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    for (auto p0 : particle0[iEvent]) {
      for (auto p1 : particle1[iEvent]) {
        for (auto p2 : particle2[iEvent]) {
          auto sum = p0 + p1 + p2;
          histogramsHandler->histograms1D[histName]->Fill(sum.M());
        }
      }
    }
  }
}