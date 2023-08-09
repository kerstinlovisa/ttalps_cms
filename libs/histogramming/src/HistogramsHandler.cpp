//  HistogramsHandler.cpp
//
//  Created by Jeremi Niedziela on 08/08/2023.

#include "HistogramsHandler.hpp"

#include "ConfigManager.hpp"
#include "EventProcessor.hpp"
#include "ExtensionsHelpers.hpp"

using namespace std;

HistogramsHandler::HistogramsHandler() {
  string configPath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/ttalps_cms/apps/configs/trigger_efficiency_histograms.py";

  auto configManager = make_unique<ConfigManager>(configPath);

  vector<string> ttbar_categories;
  configManager->GetVector("ttbar_categories", ttbar_categories);

  variable_names = {
      "muon_max_pt",
      "ele_max_pt",
      "jet_max_pt",
      "jet_ht",
  };

  trigger_sets = {
      {"had_both", {"HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59", "HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"}},
      {"had_Singlebtag", {"HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59"}},
      {"had_Doublebtag", {"HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"}},
      // {"had_Doublebtag_5jet100", {"HLT_PFHT400_FivePFJet_100_100_60_30_30_DoublePFBTagDeepCSV_4p5"}},
      // {"had_Doublebtag_5jet120", {"HLT_PFHT400_FivePFJet_120_120_60_30_30_DoublePFBTagDeepCSV_4p5"}},

      {"he_both", {"HLT_Ele28_eta2p1_WPTight_Gsf_HT150", "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"}},
      {"he_ele28", {"HLT_Ele28_eta2p1_WPTight_Gsf_HT150"}},
      {"he_ele30_Jet35", {"HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"}},

      {"ele28", {"HLT_Ele28_WPTight_Gsf"}},
      {"ele30", {"HLT_Ele30_WPTight_Gsf"}},
      {"ele32", {"HLT_Ele32_WPTight_Gsf"}},
      {"ele35", {"HLT_Ele35_WPTight_Gsf"}},
      {"ele38", {"HLT_Ele38_WPTight_Gsf"}},
      {"ele23_ele12", {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"}},

      {"isomu24", {"HLT_IsoMu24"}},
      {"mu17_mu8", {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"}},

      {"emu_mu12-8_ele23", {"HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"}},
      {"emu_mu23_ele12", {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL"}},
  };

  selection_names = {
      "single_lepton",
      "dilepton",
      "hadron",
  };
}

HistogramsHandler::~HistogramsHandler() {}

void HistogramsHandler::setup_histograms() {
  for (auto ttbar_category : ttbar_categories) {
    if (ttbar_category != "") ttbar_category = ttbar_category + "_";
    for (auto variable : variable_names) {
      histograms1D[ttbar_category + variable] =
          new TH1D((ttbar_category + variable).c_str(), (ttbar_category + variable).c_str(), 1000, 0, 1000);

      for (auto &trigger_set : trigger_sets) {
        string set_name = trigger_set.first;
        histograms1D[ttbar_category + variable + "_" + set_name] = new TH1D(
            (ttbar_category + variable + "_" + set_name).c_str(), (ttbar_category + variable + "_" + set_name).c_str(), 1000, 0, 1000);
        histograms1D[ttbar_category + variable + "_" + set_name + "_eff"] =
            new TH1D((ttbar_category + variable + "_" + set_name + "_eff").c_str(),
                     (ttbar_category + variable + "_" + set_name + "_eff").c_str(), 1000, 0, 1000);
        for (auto &selection : selection_names) {
          histograms1D[ttbar_category + variable + "_" + set_name + "_" + selection] =
              new TH1D((ttbar_category + variable + "_" + set_name + "_" + selection).c_str(),
                       (ttbar_category + variable + "_" + set_name + "_" + selection).c_str(), 1000, 0, 1000);
        }
      }
    }
  }
}

void HistogramsHandler::fill_trigger_efficiencies() {
  TH1D *hist_tmp;
  for (auto ttbar_category : ttbar_categories) {
    if (ttbar_category != "") ttbar_category = ttbar_category + "_";
    for (auto &trigger_set : trigger_sets) {
      string set_name = trigger_set.first;
      for (auto variable : variable_names) {
        hist_tmp = (TH1D *)histograms1D[ttbar_category + variable + "_" + set_name]->Clone(
            (ttbar_category + variable + "_" + set_name + "_eff").c_str());
        hist_tmp->Divide(hist_tmp, histograms1D[ttbar_category + variable], 1, 1, "B");
        histograms1D[ttbar_category + variable + "_" + set_name + "_eff"] = hist_tmp;
      }
    }
  }
}

void HistogramsHandler::fill_hist_variables(const shared_ptr<Event> event, string prefix, string suffix) {
  if (prefix != "") prefix = prefix + "_";
  if (suffix != "") suffix = "_" + suffix;

  EventProcessor &eventProcessor = EventProcessor::getInstance();

  float muon_max_pt = eventProcessor.get_max_pt(event, "Muon");
  float ele_max_pt = eventProcessor.get_max_pt(event, "Electron");
  float jet_max_pt = eventProcessor.get_max_pt(event, "Jet");
  float jet_ht = eventProcessor.get_ht(event, "Jet");

  histograms1D[prefix + "muon_max_pt" + suffix]->Fill(muon_max_pt);
  histograms1D[prefix + "ele_max_pt" + suffix]->Fill(ele_max_pt);
  histograms1D[prefix + "jet_max_pt" + suffix]->Fill(jet_max_pt);
  histograms1D[prefix + "jet_ht" + suffix]->Fill(jet_ht);
}

void HistogramsHandler::fill_hists_for_trigger_sets(const shared_ptr<Event> event, string ttbar_category) {
  bool passes_triggers;
  bool passes_triggers_single_lepton;
  bool passes_triggers_dilepton;
  bool passes_triggers_hadron;

  EventProcessor &eventProcessor = EventProcessor::getInstance();

  for (auto &trigger_set : trigger_sets) {
    passes_triggers = false;
    passes_triggers_single_lepton = false;
    passes_triggers_dilepton = false;
    passes_triggers_hadron = false;
    for (auto &trigger_name : trigger_set.second) {
      bool trigger = event->Get(trigger_name);
      if (trigger) {
        passes_triggers = true;
        if (eventProcessor.passes_single_lepton_selections(event)) passes_triggers_single_lepton = true;
        if (eventProcessor.passes_dilepton_selections(event)) passes_triggers_dilepton = true;
        if (eventProcessor.passes_hadron_selections(event)) passes_triggers_hadron = true;
      }
    }
    if (passes_triggers) fill_hist_variables(event, ttbar_category, trigger_set.first);
    if (passes_triggers_single_lepton) fill_hist_variables(event, ttbar_category, trigger_set.first + "_single_lepton");
    if (passes_triggers_dilepton) fill_hist_variables(event, ttbar_category, trigger_set.first + "_dilepton");
    if (passes_triggers_hadron) fill_hist_variables(event, ttbar_category, trigger_set.first + "_hadron");
  }
}

void HistogramsHandler::save_hists(string output_path) {
  auto output_file = new TFile((output_path).c_str(), "recreate");
  output_file->cd();

  for (auto name : ttbar_categories) output_file->mkdir(name.c_str());

  for (auto hist : histograms1D) {
    string hist_name = hist.first;
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