from ROOT import TFile, kBlue, kGreen, TCanvas, TH1D, gStyle

# input_file_path = "../build/hexaquarks_lclc.root"
# input_file_path = "../build/hexaquarks_lclc_jpsi2mumu.root"
input_file_path = "../build/hexaquarks_lclc_jpsi2mumu_small.root"

def main():
  gStyle.SetLineScalePS(1)
  
  input_file = TFile.Open(input_file_path)
  
  hist_names = [
    "delta_eta_jPsi_pi",
    "delta_eta_pi_pi",
    "delta_phi_jPsi_pi",
    "delta_phi_pi_pi",
    "delta_r_jPsi_pi",
    "delta_r_pi_pi",
    "pt_jPsi",
    "pt_pi",
  ]
  
  hists = {}
  hists_from_hexa = {}
  
  canvas = TCanvas("canvas", "canvas", 2880, 1800)
  canvas.Divide(3, 3)
  
  i_pad = 1
  for hist_name in hist_names:
    canvas.cd(i_pad)
    hists[hist_name] = input_file.Get(hist_name)
    hists[hist_name].SetLineColor(kBlue)
    hists[hist_name].SetTitle(hist_name)
    
    hists[hist_name].Rebin(5)
    
    hists_from_hexa[hist_name] = input_file.Get(hist_name+"_from_hexa")
    hists_from_hexa[hist_name].SetLineColor(kGreen)
    hists_from_hexa[hist_name].SetTitle(hist_name)
    
    hists_from_hexa[hist_name].Rebin(5)
    
    hists_from_hexa[hist_name].DrawNormalized()
    hists[hist_name].DrawNormalized("same")
    
    
    i_pad += 1
  
  canvas.Update()
  canvas.SaveAs("hexa_hists.pdf")
  
  
  hist_m_inv_from_hexa = input_file.Get("m_inv_from_hexa")
  hist_m_inv_from_hexa.SetLineColor(kBlue)
  hist_m_inv_from_hexa.SetTitle("m_inv_from_hexa")
  hist_m_inv_from_hexa.Rebin(2)
  
  hist_m_inv_after_cuts_from_hexa = input_file.Get("m_inv_after_cuts_from_hexa")
  hist_m_inv_after_cuts_from_hexa.SetLineColor(kGreen)
  hist_m_inv_after_cuts_from_hexa.SetTitle("m_inv_after_cuts_from_hexa")
  hist_m_inv_after_cuts_from_hexa.Rebin(2)
  
  hist_m_inv = input_file.Get("m_inv")
  hist_m_inv.SetLineColor(kBlue)
  hist_m_inv.SetTitle("m_inv")
  hist_m_inv.Rebin(2)
  
  hist_m_inv_after_cuts = input_file.Get("m_inv_after_cuts")
  hist_m_inv_after_cuts.SetLineColor(kGreen)
  hist_m_inv_after_cuts.SetTitle("m_inv_after_cuts")
  hist_m_inv_after_cuts.Rebin(2)
  
  hist_m_inv_mixed = input_file.Get("m_inv_mixed")
  hist_m_inv_mixed.SetLineColor(kBlue)
  hist_m_inv_mixed.SetTitle("m_inv_mixed")
  hist_m_inv_mixed.Rebin(2)
  
  hist_m_inv_mixed_after_cuts = input_file.Get("m_inv_after_cuts_mixed")
  hist_m_inv_mixed_after_cuts.SetLineColor(kGreen)
  hist_m_inv_mixed_after_cuts.SetTitle("m_inv_after_cuts_mixed")
  hist_m_inv_mixed_after_cuts.Rebin(2)
  
  
  hist_diff = TH1D(hist_m_inv)
  hist_m_inv_mixed.Scale(hist_m_inv.GetEntries()/hist_m_inv_mixed.GetEntries())
  hist_diff.Add(hist_m_inv_mixed, -1)
  # hist_diff.Divide(hist_m_inv_mixed)
  hist_diff.SetLineColor(kBlue)
  hist_diff.Rebin(2)
  
  hist_diff_after_cuts = TH1D(hist_m_inv_after_cuts)
  hist_m_inv_mixed_after_cuts.Scale(hist_m_inv_after_cuts.GetEntries()/hist_m_inv_mixed_after_cuts.GetEntries())
  hist_diff_after_cuts.Add(hist_m_inv_mixed_after_cuts, -1)
  # hist_diff_after_cuts.Divide(hist_diff_after_cuts, hist_m_inv_mixed_after_cuts, 1.0, 1.0, "B")
  hist_diff_after_cuts.SetLineColor(kGreen)
  hist_diff_after_cuts.Rebin(2)
  
  canvas_m_inv = TCanvas("canvas_m_inv", "canvas_m_inv", 2880, 1800)
  canvas_m_inv.Divide(2, 3)
  
  canvas_m_inv.cd(1)
  hist_m_inv_from_hexa.Draw()
  hist_m_inv_after_cuts_from_hexa.Draw("same")
  
  canvas_m_inv.cd(2)
  hist_m_inv.Draw()
  hist_m_inv_after_cuts.Draw("same")
  
  canvas_m_inv.cd(3)
  hist_m_inv_mixed.Draw()
  hist_m_inv_mixed_after_cuts.Draw("same")
  
  canvas_m_inv.cd(4)
  hist_diff.Draw()
  
  canvas_m_inv.cd(5)
  hist_diff_after_cuts.Draw()
  
  canvas_m_inv.Update()
  canvas_m_inv.SaveAs("hexa_hists_m_inv.pdf")
  

if __name__ == "__main__":
  main()