//  HistogramsFiller.cpp
//
//  Created by Jeremi Niedziela on 10/08/2023.

#include "HistogramsFiller.hpp"

#include "ConfigManager.hpp"
#include "TTAlpsSelections.hpp"
#include "EventProcessor.hpp"
#include "ExtensionsHelpers.hpp"

using namespace std;

HistogramsFiller::HistogramsFiller(string configPath, shared_ptr<HistogramsHandler> histogramsHandler_)
    : histogramsHandler(histogramsHandler_) {
  auto configManager = std::make_unique<ConfigManager>(configPath);

  configManager->GetMap("triggerSets", triggerSets);
  for (auto it = triggerSets.begin(); it != triggerSets.end(); ++it) triggerNames.push_back(it->first);
}

HistogramsFiller::~HistogramsFiller() {}

bool HistogramsFiller::EndsWithTriggerName(string name) {
  string lastPart = name.substr(name.rfind("_") + 1);
  return find(triggerNames.begin(), triggerNames.end(), lastPart) != triggerNames.end();
}

void HistogramsFiller::FillTriggerEfficiencies() {
  TH1D *hist_tmp;

  for (auto &[name, hist] : histogramsHandler->histograms1D) {
    if (!EndsWithTriggerName(name)) continue;
    string nameWithoutTrigger = name.substr(0, name.rfind("_"));
    string newName = name + "_eff";
    hist_tmp = (TH1D *)histogramsHandler->histograms1D[name]->Clone(newName.c_str());
    hist_tmp->Divide(hist_tmp, histogramsHandler->histograms1D[nameWithoutTrigger], 1, 1, "B");
    histogramsHandler->histograms1D[newName] = hist_tmp;
  }
}

void HistogramsFiller::FillTriggerVariables(const std::shared_ptr<Event> event, std::string prefix, std::string suffix) {
  if (prefix != "") prefix = prefix + "_";
  if (suffix != "") suffix = "_" + suffix;

  string muonName = prefix + "muonMaxPt" + suffix;
  string eleName = prefix + "eleMaxPt" + suffix;
  string jetPtName = prefix + "jetMaxPt" + suffix;
  string jetHtName = prefix + "jetHt" + suffix;

  if (!histogramsHandler->histograms1D.count(muonName)) error() << "Couldn't find key: " << muonName << " in histograms map\n";
  if (!histogramsHandler->histograms1D.count(eleName)) error() << "Couldn't find key: " << eleName << " in histograms map\n";
  if (!histogramsHandler->histograms1D.count(jetPtName)) error() << "Couldn't find key: " << jetPtName << " in histograms map\n";
  if (!histogramsHandler->histograms1D.count(jetHtName)) error() << "Couldn't find key: " << jetHtName << " in histograms map\n";

  auto eventProcessor = make_unique<EventProcessor>();

  histogramsHandler->histograms1D[muonName]->Fill(eventProcessor->GetMaxPt(event, "Muon"));
  histogramsHandler->histograms1D[eleName]->Fill(eventProcessor->GetMaxPt(event, "Electron"));
  histogramsHandler->histograms1D[jetPtName]->Fill(eventProcessor->GetMaxPt(event, "Jet"));
  histogramsHandler->histograms1D[jetHtName]->Fill(eventProcessor->GetHt(event, "Jet"));
}

void HistogramsFiller::FillTriggerVariablesPerTriggerSet(const std::shared_ptr<Event> event, std::string ttbarCategory) {
  auto ttAlpsSelections = make_unique<TTAlpsSelections>();

  bool passesSingleLepton = ttAlpsSelections->PassesSingleLeptonSelections(event);
  bool passesDilepton = ttAlpsSelections->PassesDileptonSelections(event);
  bool passesHadron = ttAlpsSelections->PassesHadronSelections(event);

  for (auto &[triggerSetName, triggerSet] : triggerSets) {
    bool passesTrigger = false;

    for (auto &triggerName : triggerSet) {
      passesTrigger = event->Get(triggerName);
      if (passesTrigger) break;
    }
    if (!passesTrigger) continue;

    FillTriggerVariables(event, ttbarCategory, triggerSetName);
    if (passesSingleLepton) FillTriggerVariables(event, ttbarCategory, triggerSetName + "_singleLepton");
    if (passesDilepton) FillTriggerVariables(event, ttbarCategory, triggerSetName + "_dilepton");
    if (passesHadron) FillTriggerVariables(event, ttbarCategory, triggerSetName + "_hadron");
  }
}