from ROOT import TFile, TGraph, TCanvas, gPad, TLegend, TLine, TColor
import ROOT

trigger_sets = [
  "had_both",
  "had_Singlebtag",   
  "had_Doublebtag",
  "he_both",
  "he_ele28",
  "he_ele30_Jet35",
  "ele28", 
  "ele30", 
  "ele32", 
  "ele35", 
  "ele38", 
  "ele23_ele12",
  "isomu24",    
  "mu17_mu8",
  "emu_mu12-8_ele23",          
  "emu_mu23_ele12",
]

trigger_dict = {
  "had_both"            :  ["HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59",
                            "HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"],
  "had_Singlebtag"      :  ["HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59"],
  "had_Doublebtag"      :  ["HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"],
  "had_Doublebtag_5jet100":["HLT_PFHT400_FivePFJet_100_100_60_30_30_DoublePFBTagDeepCSV_4p5"],
  "had_Doublebtag_5jet120":["HLT_PFHT400_FivePFJet_120_120_60_30_30_DoublePFBTagDeepCSV_4p5"],

  "he_both"             :  ["HLT_Ele28_eta2p1_WPTight_Gsf_HT150",
                            "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"],
  "he_ele28"            :  ["HLT_Ele28_eta2p1_WPTight_Gsf_HT150"],
  "he_ele30_Jet35"      :  ["HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"],
  
  "ele28"               :  ["HLT_Ele28_WPTight_Gsf"],
  "ele30"               :  ["HLT_Ele30_WPTight_Gsf"],
  "ele32"               :  ["HLT_Ele32_WPTight_Gsf"],
  "ele35"               :  ["HLT_Ele35_WPTight_Gsf"],
  "ele38"               :  ["HLT_Ele38_WPTight_Gsf"],
  "ele23_ele12"         :  ["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"],

  "isomu24"             :  ["HLT_IsoMu24"],
  "mu17_mu8"            :  ["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"],

  "emu_mu12-8_ele23"    :  ["HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL",
                            "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"],
  "emu_mu23_ele12"      :  ["HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL"],
}

decay_modes = [
  ["hh",        "jet_ht"],
  ["he",        "jet_ht"],
  ["hmu",       "jet_ht"],
  # ["htau",     ],
  ["ee",         "ele_max_pt"],
  ["mumu",       "muon_max_pt"],
  # ["tautau",   ],
  ["emu",        "muon_max_pt"],
  # ["etau",     ],
  # ["mutau",   ],
]

selection_names = [
  "",
  "_single_lepton",
  "_dilepton",
  "_hadron",
]

def main():

  canvas = TCanvas("canvas", "canvas", 800, 600)
  canvas.cd()

  input_path = "/afs/desy.de/user/l/lrygaard/TTALP/output/TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root"
  input_file = TFile.Open(input_path)

  for selection_name in selection_names:
    print(f"-- Selection: {selection_name} --")
    print("~~ |",end="")
    for trigger in trigger_sets:
      if len(trigger_dict[trigger])==1: print(trigger_dict[trigger][0], "  |  ",end="")
      else:
        for trigger_name in trigger_dict[trigger]:
          print(f"{trigger_name}, ",end="")
        print("  |  ",end="")
    print()
    for decay_mode, variable in decay_modes:
      print(decay_mode, "  |  ",end="")
      for trigger in trigger_sets:
        if(selection_name == ""): 
          hist_name_tot = decay_mode + "/" + decay_mode + "_" + variable
        else: 
          hist_name_tot = decay_mode + "/" + decay_mode + "_" + variable + "_" + trigger
        hist_tot = input_file.Get(hist_name_tot)
        n_tot = hist_tot.GetEntries()
        hist_name = decay_mode + "/" + decay_mode + "_" + variable + "_" + trigger + selection_name
        hist = input_file.Get(hist_name)
        print(f"{(hist.GetEntries()/n_tot):.3f}  |  ",end="")
      print()
    

if __name__ == "__main__":
    main()