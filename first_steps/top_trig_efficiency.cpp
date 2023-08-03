#include <string>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>

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
  {"hh",     {"HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59",
             "HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"}},
  {"hh1",    {"HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59"}},
  {"hh2",    {"HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"}},

  {"he",     {"HLT_Ele28_eta2p1_WPTight_Gsf_HT150",
             "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"}},
  {"he1",    {"HLT_Ele28_eta2p1_WPTight_Gsf_HT150"}},
  {"he2",    {"HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"}},
  
  {"e",      {"HLT_Ele28_WPTight_Gsf"}},
  {"ee",     {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"}},

  {"mu",     {"HLT_IsoMu24"}},
  {"mumu",   {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"}},

  {"emu1",   {"HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL",
             "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"}},
  {"emu2",   {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL"}},
};

map<string, TH1D*> get_setup_histograms() {
  map<string, TH1D*> hists;
  hists["muon_max_pt"] = new TH1D("muon_max_pt", "muon_max_pt", 500, 0, 500);
  hists["ele_max_pt"] = new TH1D("ele_max_pt", "ele_max_pt", 500, 0, 500);
  hists["jet_ht"] = new TH1D("jet_ht", "jet_ht", 1000, 0, 1000);

  for(auto &trigger_set : trigger_sets){
    string set_name = trigger_set.first;
    hists["muon_max_pt_"+set_name] = new TH1D(("muon_max_pt_"+set_name).c_str(), ("muon_max_pt_"+set_name).c_str(), 500, 0, 500);
    hists["muon_max_pt_"+set_name+"_eff"] = new TH1D(("muon_max_pt_"+set_name+"_eff").c_str(), ("muon_max_pt_"+set_name+"_eff").c_str(), 500, 0, 500);
    hists["ele_max_pt_"+set_name] = new TH1D(("ele_max_pt_"+set_name).c_str(), ("ele_max_pt_"+set_name).c_str(), 500, 0, 500);
    hists["ele_max_pt_"+set_name+"_eff"] = new TH1D(("ele_max_pt_"+set_name+"_eff").c_str(), ("ele_max_pt_"+set_name+"_eff").c_str(), 500, 0, 500);
    hists["jet_ht_"+set_name] = new TH1D(("jet_ht_"+set_name).c_str(), ("jet_ht_"+set_name).c_str(), 1000, 0, 1000);
    hists["jet_ht_"+set_name+"_eff"] = new TH1D(("jet_ht_"+set_name+"_eff").c_str(), ("jet_ht_"+set_name+"_eff").c_str(), 1000, 0, 1000);
  }
  return hists;
}

map<string, TH1D*> fill_trigger_efficiencies(map<string, TH1D*> hists){
  TH1D* hist_tmp;
  for(auto &trigger_set : trigger_sets){
    string set_name = trigger_set.first;
    hist_tmp = (TH1D*)hists["muon_max_pt_"+set_name]->Clone(("muon_max_pt_"+set_name+"_eff").c_str());
    hist_tmp->Divide(hist_tmp,hists["muon_max_pt_"+set_name],1,1,"B");
    hists["muon_max_pt_"+set_name+"_eff"] = hist_tmp;
    hists["muon_max_pt_"+set_name+"_eff"]->Divide(hists["muon_max_pt_"+set_name+"_eff"],hists["muon_max_pt_"+set_name],1,1,"B");
    hist_tmp = (TH1D*)hists["jet_ht_"+set_name]->Clone(("jet_ht_"+set_name+"_eff").c_str());
    hist_tmp->Divide(hist_tmp,hists["jet_ht_"+set_name],1,1,"B");
    hists["jet_ht_"+set_name+"_eff"] = hist_tmp;
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

void fill_hists_for_trigger_sets(float muon_max_pt, float ele_max_pt, float jet_ht, map<string, TH1D*> hists, map<string, bool> trigger_branches){
  for(auto &trigger_set : trigger_sets){
    for(auto &trigger : trigger_set.second){
      if(trigger_branches[trigger]){
        hists["muon_max_pt_"+trigger_set.first]->Fill(muon_max_pt);
        hists["ele_max_pt_"+trigger_set.first]->Fill(ele_max_pt);
        hists["jet_ht_"+trigger_set.first]->Fill(jet_ht);
        break;
      }
    }
  }
}

int main()
{

  uint n_muons = 0;
  float ele_pt[9999];
  uint n_ele = 0;
  float muon_pt[9999];
  uint n_jets = 0;
  float jet_pt[9999];

  for(auto &filename : filenames) {
    auto input_file = TFile::Open((input_file_path+filename).c_str());
    auto input_tree = (TTree*)input_file->Get("Events");

    auto output_file = new TFile((output_file_path+filename).c_str(), "recreate");
    output_file->cd();
    
    input_tree->SetBranchAddress("nMuon", &n_muons);
    input_tree->SetBranchAddress("Muon_pt", &muon_pt);
    input_tree->SetBranchAddress("nElectron", &n_ele);
    input_tree->SetBranchAddress("Electron_pt", &ele_pt);
    input_tree->SetBranchAddress("nJet", &n_jets);
    input_tree->SetBranchAddress("Jet_pt", &jet_pt);

    map<string, bool> trigger_branches = set_trigger_branches(input_tree);
    
    int i_event_output = 0;

    map<string, TH1D*> hists = get_setup_histograms();

    for(int i_event=0; i_event<input_tree->GetEntries(); i_event++){
      if(max_events >= 0 && i_event>max_events) break;
      if(i_event % 1000 == 0) cout << "Event: " << i_event << endl;

      input_tree->GetEntry(i_event);

      // if(n_muons == 0) continue;

      float muon_max_pt = get_max_pt(n_muons, muon_pt);
      float ele_max_pt = get_max_pt(n_ele, ele_pt);
      float jet_ht = get_jet_ht(n_jets, jet_pt);
      
      hists["muon_max_pt"]->Fill(muon_max_pt);
      hists["ele_max_pt"]->Fill(ele_max_pt);
      hists["jet_ht"]->Fill(jet_ht);

      fill_hists_for_trigger_sets(muon_max_pt, ele_max_pt, jet_ht, hists, trigger_branches);
    }

    hists = fill_trigger_efficiencies(hists);

    for(auto &hist : hists)
    {
      hist.second->Write();
    }

    output_file->Close();
    input_file->Close();
  }

  return 0;
}