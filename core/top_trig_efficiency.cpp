#include <string>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>

#include "event_tools.hpp"
#include "Event.hpp"
#include "EventReader.hpp"

using namespace std;

const int max_events = -1;

string input_file_path = "/nfs/dust/cms/user/jniedzie/ttalps_cms/backgrounds/";
string output_file_path = "/afs/desy.de/user/l/lrygaard/TTALP/output2/";

vector<string> filenames = {
  // "TTTo2LNu/C853E1CF-5210-5A44-95EA-511CC3BE4245.root",
  // "TTToHadronic/0BDBFD47-3437-4145-B237-14C107687FB9.root",
  // "TTToSemiLeptonic/2ADCE027-B24B-334E-87AD-D894007D232D.root",
  "TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root",
};

map<string,vector<string>> trigger_sets = {
  {"had_both",            {"HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59",
                          "HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"}},
  {"had_Singlebtag",      {"HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59"}},
  {"had_Doublebtag",      {"HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"}},
  // {"had_Doublebtag_5jet100", {"HLT_PFHT400_FivePFJet_100_100_60_30_30_DoublePFBTagDeepCSV_4p5"}},
  // {"had_Doublebtag_5jet120", {"HLT_PFHT400_FivePFJet_120_120_60_30_30_DoublePFBTagDeepCSV_4p5"}},

  {"he_both",             {"HLT_Ele28_eta2p1_WPTight_Gsf_HT150",
                          "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"}},
  {"he_ele28",            {"HLT_Ele28_eta2p1_WPTight_Gsf_HT150"}},
  {"he_ele30_Jet35",      {"HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"}},
  
  {"ele28",               {"HLT_Ele28_WPTight_Gsf"}},
  {"ele30",               {"HLT_Ele30_WPTight_Gsf"}},
  {"ele32",               {"HLT_Ele32_WPTight_Gsf"}},
  {"ele35",               {"HLT_Ele35_WPTight_Gsf"}},
  {"ele38",               {"HLT_Ele38_WPTight_Gsf"}},
  {"ele23_ele12",         {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"}},

  {"isomu24",             {"HLT_IsoMu24"}},
  {"mu17_mu8",            {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"}},

  {"emu_mu12-8_ele23",    {"HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL",
                          "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"}},
  {"emu_mu23_ele12",      {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL"}},
};

vector<string> variable_names = {
  "muon_max_pt",
  "ele_max_pt",
  "jet_max_pt",
  "jet_ht",
};

vector<string> selection_names = {
  "single_lepton",
  "dilepton",
  "hadron",
};

vector<string> short_names = {"","hh","he","hmu","htau","ee","mumu","tautau","emu","etau","mutau"};

map<string, TH1D*> hists;

void setup_histograms() {
  for(auto short_name : short_names){
    if(short_name != "") short_name = short_name+"_";
    for(auto variable : variable_names){
      hists[short_name+variable] = new TH1D((short_name+variable).c_str(), (short_name+variable).c_str(), 1000, 0, 1000);

      for(auto &trigger_set : trigger_sets){
        string set_name = trigger_set.first;
        hists[short_name+variable+"_"+set_name] = new TH1D((short_name+variable+"_"+set_name).c_str(), (short_name+variable+"_"+set_name).c_str(), 1000, 0, 1000);
        hists[short_name+variable+"_"+set_name+"_eff"] = new TH1D((short_name+variable+"_"+set_name+"_eff").c_str(), (short_name+variable+"_"+set_name+"_eff").c_str(), 1000, 0, 1000);
        for(auto &selection : selection_names){
          hists[short_name+variable+"_"+set_name+"_"+selection] = new TH1D((short_name+variable+"_"+set_name+"_"+selection).c_str(), (short_name+variable+"_"+set_name+"_"+selection).c_str(), 1000, 0, 1000);
        }
      }
    }
  }
}

void fill_trigger_efficiencies(){
  TH1D* hist_tmp;
  for(auto short_name : short_names){
    if(short_name != "") short_name = short_name+"_";
    for(auto &trigger_set : trigger_sets){
      string set_name = trigger_set.first;
      for(auto variable : variable_names){
        hist_tmp = (TH1D*)hists[short_name+variable+"_"+set_name]->Clone((short_name+variable+"_"+set_name+"_eff").c_str());
        hist_tmp->Divide(hist_tmp,hists[short_name+variable],1,1,"B");
        hists[short_name+variable+"_"+set_name+"_eff"] = hist_tmp;
      }
    }
  }
}

float get_max_pt(std::shared_ptr<std::vector<std::shared_ptr<PhysicsObject> > > particles, uint n_particles){
  float max_pt = 0;
  for(int i=0; i<n_particles; i++){ 
    float pt = particles->at(i)->Get("pt");
    if(pt > max_pt) max_pt=pt;
  }
  return max_pt;
}

float get_ht(std::shared_ptr<std::vector<std::shared_ptr<PhysicsObject> > > particles, uint n_particles){
  float ht = 0;
  for(int i=0; i<n_particles; i++){
      float pt = particles->at(i)->Get("pt");
      ht += pt;
  }
  return ht;
}


void fill_hist_variables(const shared_ptr<Event> event, string prefix = "", string suffix = ""){
  if(prefix != "") prefix = prefix+"_";
  if(suffix != "") suffix = "_"+suffix;

  uint nMuons = event->Get("nMuon");
  auto muons = event->GetCollection("Muon");
  uint nElectrons = event->Get("nElectron");
  auto electrons = event->GetCollection("Electron");
  uint nJets = event->Get("nJet");
  auto jets = event->GetCollection("Jet");

  float muon_max_pt = get_max_pt(muons, nMuons);
  float ele_max_pt = get_max_pt(electrons, nElectrons);
  float jet_max_pt = get_max_pt(jets, nJets);
  float jet_ht = get_ht(jets, nJets);

  hists[prefix+"muon_max_pt"+suffix]->Fill(muon_max_pt);
  hists[prefix+"ele_max_pt"+suffix]->Fill(ele_max_pt);
  hists[prefix+"jet_max_pt"+suffix]->Fill(jet_max_pt);
  hists[prefix+"jet_ht"+suffix]->Fill(jet_ht);  
}


bool passes_single_lepton_selections(const shared_ptr<Event> event){

  int leptons_pt30 = 0;
  int leptons_pt15 = 0;
  int jets_btagged = 0;
  int jets_pt30;

  uint nMuons = event->Get("nMuon");
  auto muons = event->GetCollection("Muon");
  for (int i = 0; i < nMuons; i++){
    float muon_pt = muons->at(i)->Get("pt");
    float muon_eta = muons->at(i)->Get("eta");
    if(muon_pt > 30 && abs(muon_eta) <2.4) leptons_pt30++;
    else if(muon_pt > 15 && abs(muon_eta) <2.5) leptons_pt15++;
  }
  uint nElectrons = event->Get("nElectron");
  auto electrons = event->GetCollection("Electron");
  for(int i = 0; i < nElectrons; i++){
    float electron_pt = electrons->at(i)->Get("pt");
    float electron_eta = electrons->at(i)->Get("eta");
    if(electron_pt > 30 && abs(electron_eta) <2.4) leptons_pt30++;
    else if(electron_pt > 15 && abs(electron_eta) <2.5) leptons_pt15++;
  }
  uint nJets = event->Get("nJet");
  auto jets = event->GetCollection("Jet");
  for(int i = 0; i < nJets; i++){
    float jet_pt = jets->at(i)->Get("pt");
    float jet_eta = jets->at(i)->Get("eta");
    float jet_btagDeepB = jets->at(i)->Get("btagDeepB");
    if(jet_pt > 30 && abs(jet_eta) < 2.4) {
      jets_pt30++;
      if(jet_btagDeepB > 0.5) jets_btagged++;
    }
  }

  if(leptons_pt30 != 1) return false;
  if(leptons_pt15 != 0) return false;
  float met_pt = event->Get("MET_pt");
  if(met_pt <= 30) return false;
  if(jets_btagged < 2) return false;
  if(jets_pt30 < 4) return false; 
  return true;
}

bool passes_dilepton_selections(const shared_ptr<Event> event){

  int muons_pt30 = 0;
  int electrons_pt30;
  int jets_btagged = 0;

  uint nMuons = event->Get("nMuon");
  auto muons = event->GetCollection("Muon");
  for (int i = 0; i < nMuons; i++){
    float muon_pt = muons->at(i)->Get("pt");
    float muon_eta = muons->at(i)->Get("eta");
    if(muon_pt > 30 && abs(muon_eta) <2.4) muons_pt30++;
  }
  uint nElectrons = event->Get("nElectron");
  auto electrons = event->GetCollection("Electron");
  for(int i = 0; i < nElectrons; i++){
    float electron_pt = electrons->at(i)->Get("pt");
    float electron_eta = electrons->at(i)->Get("eta");
    if(electron_pt > 30 && abs(electron_eta) <2.4) electrons_pt30++;
  }
  uint nJets = event->Get("nJet");
  auto jets = event->GetCollection("Jet");
  for(int i = 0; i < nJets; i++){
    float jet_pt = jets->at(i)->Get("pt");
    float jet_eta = jets->at(i)->Get("eta");
    float jet_btagDeepB = jets->at(i)->Get("btagDeepB");
    if(jet_pt > 30 && abs(jet_eta) < 2.4 && jet_btagDeepB > 0.5) jets_btagged++;
  }

  if((muons_pt30+electrons_pt30) < 2) return false;
  float met_pt = event->Get("MET_pt");
  if(met_pt <= 30) return false;
  if(jets_btagged < 2) return false;
  return true;
}


bool passes_hadron_selections(const shared_ptr<Event> event){

  int jets_btagged = 0;
  int jets_pt30;

  uint nJets = event->Get("nJet");
  auto jets = event->GetCollection("Jet");
  for(int i = 0; i < nJets; i++){
    float jet_pt = jets->at(i)->Get("pt");
    float jet_eta = jets->at(i)->Get("eta");
    float jet_btagDeepB = jets->at(i)->Get("btagDeepB");
    if(jet_pt > 30 && abs(jet_eta) < 2.4) {
      jets_pt30++;
      if(jet_btagDeepB > 0.5) jets_btagged++;
    }
  }

  if(jets_btagged < 2) return false;
  if(jets_pt30 < 6) return false;
  return true;
}


void fill_hists_for_trigger_sets(const shared_ptr<Event> event, string short_name=""){

  bool passes_triggers;
  bool passes_triggers_single_lepton;
  bool passes_triggers_dilepton;
  bool passes_triggers_hadron;

  for(auto &trigger_set : trigger_sets){
    passes_triggers = false;
    passes_triggers_single_lepton = false;
    passes_triggers_dilepton = false;
    passes_triggers_hadron = false;
    for(auto &trigger_name : trigger_set.second){
      bool trigger = event->Get(trigger_name);
      if(trigger){
        passes_triggers = true;
        if(passes_single_lepton_selections(event)) passes_triggers_single_lepton = true;
        if(passes_dilepton_selections(event)) passes_triggers_dilepton = true;
        if(passes_hadron_selections(event)) passes_triggers_hadron = true;
      }
    }
    if(passes_triggers) fill_hist_variables(event,short_name,trigger_set.first);
    if(passes_triggers_single_lepton) fill_hist_variables(event,short_name,trigger_set.first+"_single_lepton");
    if(passes_triggers_dilepton) fill_hist_variables(event,short_name,trigger_set.first+"_dilepton");
    if(passes_triggers_hadron) fill_hist_variables(event,short_name,trigger_set.first+"_hadron");
  }
}


void save_hists(string output_path){
  auto output_file = new TFile((output_path).c_str(), "recreate");
  output_file->cd();

  for(auto name : short_names) output_file->mkdir(name.c_str());

  for(auto hist : hists){
    string hist_name = hist.first;
    if(hist_name.substr(0,2) == "hh") output_file->cd("hh");
    else if(hist_name.substr(0,2) == "he") output_file->cd("he");
    else if(hist_name.substr(0,3) == "hmu") output_file->cd("hmu");
    else if(hist_name.substr(0,4) == "htau") output_file->cd("htau");
    else if(hist_name.substr(0,2) == "ee") output_file->cd("ee");
    else if(hist_name.substr(0,3) == "emu") output_file->cd("emu");
    else if(hist_name.substr(0,4) == "mumu") output_file->cd("mumu");
    else if(hist_name.substr(0,4) == "etau") output_file->cd("etau");
    else if(hist_name.substr(0,6) == "tautau") output_file->cd("tautau");
    hist.second->Write();
  }
  output_file->Close();
}


int main()
{
  for(auto &filename : filenames) {

    auto eventReader = make_unique<EventReader>(input_file_path+filename);
    
    setup_histograms();

    int n_events = 172000;

    for(int i_event=0; i_event<n_events; i_event++){
      if(max_events >= 0 && i_event>max_events) break;
      if(i_event % 1000 == 0) cout << "Event: " << i_event << endl;

      auto event = eventReader->GetEvent(i_event);

      string short_name = get_short_name(event);
      if(find(short_names.begin(), short_names.end(), short_name) == short_names.end()){
        if(short_name != "error") cout << "Decay mode: " << short_name << " not found - skipped" << endl;
        continue;
      }

      fill_hist_variables(event);
      fill_hist_variables(event,short_name);

      fill_hists_for_trigger_sets(event, "");
      fill_hists_for_trigger_sets(event, short_name);
    }

    fill_trigger_efficiencies();

    save_hists(output_file_path+filename);
  }

  return 0;
}