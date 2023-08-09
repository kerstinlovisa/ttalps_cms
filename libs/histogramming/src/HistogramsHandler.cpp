//  HistogramsHandler.cpp
//
//  Created by Jeremi Niedziela on 08/08/2023.

#include "HistogramsHandler.hpp"

#include "ConfigManager.hpp"
#include "EventProcessor.hpp"
#include "ExtensionsHelpers.hpp"

using namespace std;

HistogramsHandler::HistogramsHandler() {
  std::string configPath =
      "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/ttalps_cms/apps/configs/trigger_efficiency_histograms.py";

  auto configManager = std::make_unique<ConfigManager>(configPath);

  configManager->GetVector("ttbar_categories", ttbar_categories);
  configManager->GetVector("variable_names", variable_names);
  configManager->GetVector("selection_names", selection_names);

  trigger_sets = {
      {"hadBoth", {"HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59", "HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"}},
      {"hadSinglebtag", {"HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59"}},
      {"hadDoublebtag", {"HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"}},
      // {"had_Doublebtag_5jet100", {"HLT_PFHT400_FivePFJet_100_100_60_30_30_DoublePFBTagDeepCSV_4p5"}},
      // {"had_Doublebtag_5jet120", {"HLT_PFHT400_FivePFJet_120_120_60_30_30_DoublePFBTagDeepCSV_4p5"}},

      {"heBoth", {"HLT_Ele28_eta2p1_WPTight_Gsf_HT150", "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"}},
      {"heEle28", {"HLT_Ele28_eta2p1_WPTight_Gsf_HT150"}},
      {"heEle30_Jet35", {"HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"}},

      {"ele28", {"HLT_Ele28_WPTight_Gsf"}},
      {"ele30", {"HLT_Ele30_WPTight_Gsf"}},
      {"ele32", {"HLT_Ele32_WPTight_Gsf"}},
      {"ele35", {"HLT_Ele35_WPTight_Gsf"}},
      {"ele38", {"HLT_Ele38_WPTight_Gsf"}},
      {"ele23Ele12", {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"}},

      {"isomu24", {"HLT_IsoMu24"}},
      {"mu17Mu8", {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"}},

      {"emuMu12-8_ele23", {"HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"}},
      {"emuMu23_ele12", {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL"}},
  };

  for (auto it = trigger_sets.begin(); it != trigger_sets.end(); ++it) {
    triggerNames.push_back(it->first);
  }

  configManager->GetMap("histTitles", histTitles);
  configManager->GetMap("histNbins", histNbins);
  configManager->GetMap("histMin", histMin);
  configManager->GetMap("histMax", histMax);
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

  string muonName = prefix + "muon_max_pt" + suffix;
  string eleName = prefix + "ele_max_pt" + suffix;
  string jetPtName = prefix + "jet_max_pt" + suffix;
  string jetHtName = prefix + "jet_ht" + suffix;

  if (!histograms1D.count(muonName)) error() << "Couldn't find key: " << muonName << " in histograms map\n";
  if (!histograms1D.count(eleName)) error() << "Couldn't find key: " << eleName << " in histograms map\n";
  if (!histograms1D.count(jetPtName)) error() << "Couldn't find key: " << jetPtName << " in histograms map\n";
  if (!histograms1D.count(jetHtName)) error() << "Couldn't find key: " << jetHtName << " in histograms map\n";

  histograms1D[prefix + "muon_max_pt" + suffix]->Fill(muon_max_pt);
  histograms1D[prefix + "ele_max_pt" + suffix]->Fill(ele_max_pt);
  histograms1D[prefix + "jet_max_pt" + suffix]->Fill(jet_max_pt);
  histograms1D[prefix + "jet_ht" + suffix]->Fill(jet_ht);
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
    if (passes_triggers_single_lepton) fill_hist_variables(event, ttbar_category, trigger_set.first + "_single_lepton");
    if (passes_triggers_dilepton) fill_hist_variables(event, ttbar_category, trigger_set.first + "_dilepton");
    if (passes_triggers_hadron) fill_hist_variables(event, ttbar_category, trigger_set.first + "_hadron");
  }
}

void HistogramsHandler::save_hists(std::string output_path) {
  auto output_file = new TFile((output_path).c_str(), "recreate");
  output_file->cd();

  for (auto name : ttbar_categories) output_file->mkdir(name.c_str());

  for (auto hist : histograms1D) {
    std::string hist_name = hist.first;
    if (hist_name.substr(0, 2) == "hh")
      output_file->cd("hh");
    else if (hist_name.substr(0, 2) == "he")
      output_file->cd("he");
    else if (hist_name.substr(0, 3) == "hmu")
      output_file->cd("hmu");
    else if (hist_name.substr(0, 4) == "htau")
      output_file->cd("htau");
    else if (hist_name.substr(0, 2) == "ee")
      output_file->cd("ee");
    else if (hist_name.substr(0, 3) == "emu")
      output_file->cd("emu");
    else if (hist_name.substr(0, 4) == "mumu")
      output_file->cd("mumu");
    else if (hist_name.substr(0, 4) == "etau")
      output_file->cd("etau");
    else if (hist_name.substr(0, 6) == "tautau")
      output_file->cd("tautau");
    hist.second->Write();
  }
  output_file->Close();
}