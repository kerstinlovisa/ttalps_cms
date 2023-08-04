#include <string>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>

#include "event_tools.hpp"

using namespace std;

const int max_events = -1;

string input_file_path = "/nfs/dust/cms/user/jniedzie/ttalps_cms/backgrounds/";
string output_file_path = "/afs/desy.de/user/l/lrygaard/TTALP/output/";

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

  {"he_both",             {"HLT_Ele28_eta2p1_WPTight_Gsf_HT150",
                          "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"}},
  {"he_ele28",            {"HLT_Ele28_eta2p1_WPTight_Gsf_HT150"}},
  {"he_ele30_Jet35",      {"HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"}},
  
  {"ele28",               {"HLT_Ele28_WPTight_Gsf"}},
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

vector<string> short_names = {"","hh","he","hmu","htau","ee","mumu","tautau","emu","etau","mutau"};

map<string, TH1D*> get_setup_histograms() {
  map<string, TH1D*> hists;
  for(auto short_name : short_names){
    if(short_name != "") short_name = short_name+"_";
    for(auto variable : variable_names){
      hists[short_name+variable] = new TH1D((short_name+variable).c_str(), (short_name+variable).c_str(), 1000, 0, 1000);

      for(auto &trigger_set : trigger_sets){
        string set_name = trigger_set.first;
        hists[short_name+variable+"_"+set_name] = new TH1D((short_name+variable+"_"+set_name).c_str(), (short_name+variable+"_"+set_name).c_str(), 1000, 0, 1000);
        hists[short_name+variable+"_"+set_name+"_eff"] = new TH1D((short_name+variable+"_"+set_name+"_eff").c_str(), (short_name+variable+"_"+set_name+"_eff").c_str(), 1000, 0, 1000);
      }
      for(auto &selection : selections){
        hists[short_name+variable+"_"+selection] = new TH1D((short_name+variable+"_"+selection).c_str(), (short_name+variable+"_"+selection).c_str(), 1000, 0, 1000);
      }
    }
  }
  return hists;
}

map<string, TH1D*> fill_trigger_efficiencies(map<string, TH1D*> hists){
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
  return hists;
}

map<string, bool> set_trigger_branches(TTree* input_tree){
  std::map<string, bool> triggers;
  vector<string> included;
  for(auto trigger_set : trigger_sets){
    vector<string> trigger_names = trigger_set.second;
    for(auto trigger_name : trigger_names){
      if(!count(included.begin(), included.end(), trigger_name)){
        triggers[trigger_name] = false;
        input_tree->SetBranchAddress(trigger_name.c_str(), &triggers[trigger_name]);
        included.push_back(trigger_name);
      }
    }
  }
  return triggers;
}

float get_max_pt(int n_particles, float* particle_pt){
  float max_pt = 0;
  for(int i=0; i<n_particles; i++){ 
      if(particle_pt[i] > max_pt) max_pt=particle_pt[i];
  }
  return max_pt;
}

float get_jet_ht(int n_jets, float* jet_pt){
  float jet_ht = 0;
  for(int i=0; i<n_jets; i++){ 
      jet_ht += jet_pt[i];
  }
  return jet_ht;
}

void fill_hists_for_trigger_sets(float muon_max_pt, float ele_max_pt, float jet_max_pt, float jet_ht, 
                                map<string, TH1D*> hists, map<string, bool> trigger_branches, string short_name=""){
  if(short_name != "") short_name = short_name+"_";
  for(auto &trigger_set : trigger_sets){
    for(auto &trigger : trigger_set.second){
      if(trigger_branches[trigger]){
        hists[short_name+"muon_max_pt_"+trigger_set.first]->Fill(muon_max_pt);
        hists[short_name+"ele_max_pt_"+trigger_set.first]->Fill(ele_max_pt);
        hists[short_name+"jet_max_pt_"+trigger_set.first]->Fill(jet_max_pt);
        hists[short_name+"jet_ht_"+trigger_set.first]->Fill(jet_ht);
        break;
      }
    }
  }
}

void save_hists(map<string, TH1D*> hists, string output_path){
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

  uint n_muons = 0;
  float ele_pt[9999];
  uint n_ele = 0;
  float muon_pt[9999];
  uint n_jets = 0;
  float jet_pt[9999];
  uint nGenPart = 0;
  int GenPart_pdgId[9999];
  int GenPart_genPartIdxMother[9999];
  int GenPart_statusFlags[9999];

  for(auto &filename : filenames) {
    auto input_file = TFile::Open((input_file_path+filename).c_str());
    auto input_tree = (TTree*)input_file->Get("Events");
    
    input_tree->SetBranchAddress("nMuon", &n_muons);
    input_tree->SetBranchAddress("Muon_pt", &muon_pt);
    input_tree->SetBranchAddress("nElectron", &n_ele);
    input_tree->SetBranchAddress("Electron_pt", &ele_pt);
    input_tree->SetBranchAddress("nJet", &n_jets);
    input_tree->SetBranchAddress("Jet_pt", &jet_pt);
    input_tree->SetBranchAddress("nGenPart", &nGenPart);
    input_tree->SetBranchAddress("GenPart_pdgId", &GenPart_pdgId);
    input_tree->SetBranchAddress("GenPart_genPartIdxMother", &GenPart_genPartIdxMother);
    input_tree->SetBranchAddress("GenPart_statusFlags", &GenPart_statusFlags);

    map<string, bool> trigger_branches = set_trigger_branches(input_tree);
    
    int i_event_output = 0;

    map<string, TH1D*> hists = get_setup_histograms();

    for(int i_event=0; i_event<input_tree->GetEntries(); i_event++){
      if(max_events >= 0 && i_event>max_events) break;
      if(i_event % 1000 == 0) cout << "Event: " << i_event << endl;

      input_tree->GetEntry(i_event);

      string short_name = get_short_name(nGenPart, GenPart_pdgId, GenPart_genPartIdxMother, GenPart_statusFlags);
      if(short_name=="error"){
        cout << "Short name error - skipping event" << endl;
        continue;
      }

      // if(n_muons == 0) continue;

      float muon_max_pt = get_max_pt(n_muons, muon_pt);
      float ele_max_pt = get_max_pt(n_ele, ele_pt);
      float jet_max_pt = get_max_pt(n_jets, jet_pt);
      float jet_ht = get_jet_ht(n_jets, jet_pt);
      
      hists["muon_max_pt"]->Fill(muon_max_pt);
      hists["ele_max_pt"]->Fill(ele_max_pt);
      hists["jet_max_pt"]->Fill(jet_max_pt);
      hists["jet_ht"]->Fill(jet_ht);
      hists[short_name+"_muon_max_pt"]->Fill(muon_max_pt);
      hists[short_name+"_ele_max_pt"]->Fill(ele_max_pt);
      hists[short_name+"_jet_max_pt"]->Fill(jet_max_pt);
      hists[short_name+"_jet_ht"]->Fill(jet_ht);

      fill_hists_for_trigger_sets(muon_max_pt, ele_max_pt, jet_max_pt, jet_ht, hists, trigger_branches, "");
      fill_hists_for_trigger_sets(muon_max_pt, ele_max_pt, jet_max_pt, jet_ht, hists, trigger_branches, short_name);
    }

    hists = fill_trigger_efficiencies(hists);

    save_hists(hists, output_file_path+filename);

    input_file->Close();
  }

  return 0;
}