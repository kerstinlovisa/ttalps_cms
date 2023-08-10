//  HistogramsHandler.cpp
//
//  Created by Jeremi Niedziela on 08/08/2023.

#include "HistogramsHandler.hpp"

#include "ConfigManager.hpp"
#include "EventProcessor.hpp"
#include "ExtensionsHelpers.hpp"

using namespace std;

HistogramsHandler::HistogramsHandler(string configPath) {
  auto configManager = std::make_unique<ConfigManager>(configPath);

  configManager->GetMap("trigger_sets", trigger_sets);
  configManager->GetMap("histTitles", histTitles);
  configManager->GetMap("histNbins", histNbins);
  configManager->GetMap("histMin", histMin);
  configManager->GetMap("histMax", histMax);
  configManager->GetMap("histOutputDir", histOutputDir);

  for (auto it = trigger_sets.begin(); it != trigger_sets.end(); ++it) triggerNames.push_back(it->first);
}

HistogramsHandler::~HistogramsHandler() {}

void HistogramsHandler::setup_histograms() {
  for (auto &[name, title] : histTitles) {
    histograms1D[name] = new TH1D(title.c_str(), title.c_str(), histNbins[name], histMin[name], histMax[name]);
  }
}

bool HistogramsHandler::EndsWithTriggerName(string name) {
  string lastPart = name.substr(name.rfind("_") + 1);
  return find(triggerNames.begin(), triggerNames.end(), lastPart) != triggerNames.end();
}

void HistogramsHandler::fill_trigger_efficiencies() {
  TH1D *hist_tmp;

  for (auto &[name, hist] : histograms1D) {
    if (!EndsWithTriggerName(name)) continue;
    string nameWithoutTrigger = name.substr(0, name.rfind("_"));
    string newName = name + "_eff";
    hist_tmp = (TH1D *)histograms1D[name]->Clone(newName.c_str());
    hist_tmp->Divide(hist_tmp, histograms1D[nameWithoutTrigger], 1, 1, "B");
    histograms1D[newName] = hist_tmp;
  }
}

void HistogramsHandler::fill_hist_variables(const std::shared_ptr<Event> event, std::string prefix, std::string suffix) {
  if (prefix != "") prefix = prefix + "_";
  if (suffix != "") suffix = "_" + suffix;

  auto eventProcessor = make_unique<EventProcessor>();

  float muon_max_pt = eventProcessor->get_max_pt(event, "Muon");
  float ele_max_pt = eventProcessor->get_max_pt(event, "Electron");
  float jet_max_pt = eventProcessor->get_max_pt(event, "Jet");
  float jet_ht = eventProcessor->get_ht(event, "Jet");

  string muonName = prefix + "muonMaxPt" + suffix;
  string eleName = prefix + "eleMaxPt" + suffix;
  string jetPtName = prefix + "jetMaxPt" + suffix;
  string jetHtName = prefix + "jetHt" + suffix;

  if (!histograms1D.count(muonName)) error() << "Couldn't find key: " << muonName << " in histograms map\n";
  if (!histograms1D.count(eleName)) error() << "Couldn't find key: " << eleName << " in histograms map\n";
  if (!histograms1D.count(jetPtName)) error() << "Couldn't find key: " << jetPtName << " in histograms map\n";
  if (!histograms1D.count(jetHtName)) error() << "Couldn't find key: " << jetHtName << " in histograms map\n";

  histograms1D[prefix + "muonMaxPt" + suffix]->Fill(muon_max_pt);
  histograms1D[prefix + "eleMaxPt" + suffix]->Fill(ele_max_pt);
  histograms1D[prefix + "jetMaxPt" + suffix]->Fill(jet_max_pt);
  histograms1D[prefix + "jetHt" + suffix]->Fill(jet_ht);
}

void HistogramsHandler::fill_hists_for_trigger_sets(const std::shared_ptr<Event> event, std::string ttbar_category) {
  bool passes_triggers;
  bool passes_triggers_single_lepton;
  bool passes_triggers_dilepton;
  bool passes_triggers_hadron;

  auto eventProcessor = make_unique<EventProcessor>();

  for (auto &trigger_set : trigger_sets) {
    passes_triggers = false;
    passes_triggers_single_lepton = false;
    passes_triggers_dilepton = false;
    passes_triggers_hadron = false;
    for (auto &trigger_name : trigger_set.second) {
      bool trigger = event->Get(trigger_name);
      if (trigger) {
        passes_triggers = true;
        if (eventProcessor->passes_single_lepton_selections(event)) passes_triggers_single_lepton = true;
        if (eventProcessor->passes_dilepton_selections(event)) passes_triggers_dilepton = true;
        if (eventProcessor->passes_hadron_selections(event)) passes_triggers_hadron = true;
      }
    }
    if (passes_triggers) fill_hist_variables(event, ttbar_category, trigger_set.first);
    if (passes_triggers_single_lepton) fill_hist_variables(event, ttbar_category, trigger_set.first + "_singleLepton");
    if (passes_triggers_dilepton) fill_hist_variables(event, ttbar_category, trigger_set.first + "_dilepton");
    if (passes_triggers_hadron) fill_hist_variables(event, ttbar_category, trigger_set.first + "_hadron");
  }
}

void HistogramsHandler::save_hists(std::string output_path) {
  auto output_file = new TFile((output_path).c_str(), "recreate");
  output_file->cd();

  for (auto &[name, hist] : histograms1D) {
    string outputDir = histOutputDir[name];
    if(!output_file->Get(outputDir.c_str())) output_file->mkdir(outputDir.c_str());
    output_file->cd(outputDir.c_str());
    hist->Write();
  }
  output_file->Close();
}