#include "HexaquarksHistogramsFiller.hpp"

#include "ConfigManager.hpp"
#include "ExtensionsHelpers.hpp"

using namespace std;

HexaquarksHistogramsFiller::HexaquarksHistogramsFiller(string configPath, shared_ptr<HistogramsHandler> histogramsHandler_)
    : histogramsHandler(histogramsHandler_) {
  auto configManager = make_unique<ConfigManager>(configPath);
  configManager->GetValue("nMixedEventsScale", nMixedEventsScale);

  eventProcessor = make_unique<EventProcessor>();
}

HexaquarksHistogramsFiller::~HexaquarksHistogramsFiller() {}

void HexaquarksHistogramsFiller::FillJpsiPiPiMinvHists(const vector<vector<TLorentzVector>> &jPsi,
                                                       const vector<vector<TLorentzVector>> &piPlus,
                                                       const vector<vector<TLorentzVector>> &piMinus) {
  string histName = "m_inv";
  string histNameMix = "m_inv_mixed";
  histogramsHandler->CheckHistogram(histName);
  histogramsHandler->CheckHistogram(histNameMix);

  int nEvents = jPsi.size();
  

  info() << "Filling real histograms...\n";

  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    for (auto j : jPsi[iEvent]) {
      for (auto p : piPlus[iEvent]) {
        for (auto m : piMinus[iEvent]) {
          auto sum = j + p + m;
          histogramsHandler->histograms1D[histName]->Fill(sum.M());
        }
      }
    }
  }

  int maxMixedEntries = nMixedEventsScale * histogramsHandler->histograms1D[histName]->GetEntries();
  bool mixingFinished = false;
  int nMixedEntries = 0;

  info() << "Requested number of mixed entries: "<<maxMixedEntries<<"\n";
  info() << "Filling mixed histograms...\n";
  
  while(!mixingFinished){
    int jEventIndex = randInt(0, nEvents-1);
    int pEventIndex = randInt(0, nEvents-1);
    int mEventIndex = randInt(0, nEvents-1);

    if(jEventIndex == pEventIndex || jEventIndex == mEventIndex || pEventIndex == mEventIndex) continue;
    if(jPsi[jEventIndex].size() == 0 || piPlus[pEventIndex].size() == 0 || piMinus[mEventIndex].size() == 0) continue;
    
    int j = randInt(0, jPsi[jEventIndex].size()-1);
    int p = randInt(0, piPlus[pEventIndex].size()-1);
    int m = randInt(0, piMinus[mEventIndex].size()-1);
    
    auto sum = jPsi[jEventIndex][j] + piPlus[pEventIndex][p] + piMinus[mEventIndex][m];

    if(sum.M() < 0.16){
      info() << "\n";
      jPsi[jEventIndex][j].Print();
      piPlus[pEventIndex][p].Print();
      piMinus[mEventIndex][m].Print();
    }

    histogramsHandler->histograms1D[histNameMix]->Fill(sum.M());
    nMixedEntries++;

    if(nMixedEntries%10000 == 0){
      info() << 100*nMixedEntries/maxMixedEntries << "%\r" << flush;
    }

    if(nMixedEntries == maxMixedEntries) mixingFinished = true;
  }
}