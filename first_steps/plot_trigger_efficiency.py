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

decay_modes = [
  ["hh",       ROOT.kBlue],
  ["he",       ROOT.kRed],
  ["hmu",      ROOT.kOrange],  
  # ["htau",     ROOT.kGreen],  
  ["ee",       ROOT.kGreen],
  ["mumu",     ROOT.kMagenta],  
  # ["tautau",   kBlue], 
  ["emu",      ROOT.kBlack],  
  # ["etau",     kBlue],  
  # ["mutau",    kBlue], 
]

variables = [
  ["jet_max_pt",    600],
  ["jet_ht",        1000],
  ["muon_max_pt",   200],
  ["ele_max_pt",    200],
]

def main():

  input_path = "/afs/desy.de/user/l/lrygaard/TTALP/output/TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root"
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
        hist.SetTitle(trigger)
        hist.SetStats(0)
        hist.Rebin(20)
        hist.Draw("same")
        leg.AddEntry(hist, decay_mode)
      leg.Draw()
      canvas.SaveAs(output_path+variable + "_" + trigger+"_eff.pdf")
    

if __name__ == "__main__":
    main()