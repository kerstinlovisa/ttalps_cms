from ROOT import TFile, TGraph, TCanvas, gPad, gStyle, gROOT, TLegend, TLine, TColor
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
  "had_both"            :  "HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59 OR HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94",
  "had_Singlebtag"      :  "HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59",
  "had_Doublebtag"      :  "HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94",
  "had_Doublebtag_5jet100":"HLT_PFHT400_FivePFJet_100_100_60_30_30_DoublePFBTagDeepCSV_4p5",
  "had_Doublebtag_5jet120":"HLT_PFHT400_FivePFJet_120_120_60_30_30_DoublePFBTagDeepCSV_4p5",

  "he_both"             :  "HLT_Ele28_eta2p1_WPTight_Gsf_HT150 OR HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned",
  "he_ele28"            :  "HLT_Ele28_eta2p1_WPTight_Gsf_HT150",
  "he_ele30_Jet35"      :  "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned",
  
  "ele28"               :  "HLT_Ele28_WPTight_Gsf",
  "ele30"               :  "HLT_Ele30_WPTight_Gsf",
  "ele32"               :  "HLT_Ele32_WPTight_Gsf",
  "ele35"               :  "HLT_Ele35_WPTight_Gsf",
  "ele38"               :  "HLT_Ele38_WPTight_Gsf",
  "ele23_ele12"         :  "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL",

  "isomu24"             :  "HLT_IsoMu24",
  "mu17_mu8"            :  "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ",

  "emu_mu12-8_ele23"    :  "HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL OR HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL",
  "emu_mu23_ele12"      :  "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",
}

decay_modes = [
  ["hh",       ROOT.kBlue],
  # ["he",       ROOT.kRed],
  # ["hmu",      ROOT.kOrange],  
  # ["htau",     ROOT.kGreen],  
  # ["ee",       ROOT.kGreen],
  # ["mumu",     ROOT.kMagenta],  
  # ["tautau",   kBlue], 
  # ["emu",      ROOT.kBlack],  
  # ["etau",     kBlue],  
  # ["mutau",    kBlue], 
]

variables = [
  ["jet_max_pt",    600],
  # ["jet_ht",        1000],
  # ["muon_max_pt",   200],
  # ["ele_max_pt",    200],
]

def main():

  input_path = "/afs/desy.de/user/l/lrygaard/TTALP/output2/TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root"
  output_path = "/afs/desy.de/user/l/lrygaard/TTALP/plots/TTbar_inclusive/"
  input_file = TFile.Open(input_path)

  for trigger in trigger_sets:
    for variable, xmax in variables:
      canvas = TCanvas("canvas", "canvas", 800, 600)
      canvas.cd()
      gStyle.SetStatStyle(0)
      gStyle.SetTitleStyle(0)
      gROOT.ForceStyle()

      leg = TLegend()
      leg.SetBorderSize(0)
      leg.SetFillColor(0)
      leg.SetFillStyle(0)
      leg.SetTextFont(42)
      leg.SetTextSize(0.035)
      for decay_mode, color in decay_modes:
        hist_name = decay_mode + "/" + decay_mode + "_" + variable + "_" + trigger + "_eff"
        hist = input_file.Get(hist_name)
        hist.SetLineColor(color)
        hist.GetXaxis().SetRangeUser(0,xmax)
        hist.GetXaxis().SetTitle(variable)
        hist.SetTitle(trigger_dict[trigger])
        hist.SetStats(0)
        # hist.Rebin(2)
        hist.Draw("same")
        leg.AddEntry(hist, decay_mode)
      leg.Draw()
      canvas.SaveAs(output_path+variable + "_" + trigger+"_eff.pdf")
    

if __name__ == "__main__":
    main()