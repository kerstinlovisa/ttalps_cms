import ROOT
from ROOT import TColor

signal_path = "build"
background_path = "build"
output_path = "../plots_test"

files = {
  #name                  filename     type(signal/background)
  # "tta_mAlp-0p35GeV"  :  ["tta_mAlp-0p35GeV_nEvents-100000.root", "signal"],
  # "TTbar_inclusive"   :  ["TTbar_inclusive_FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root", "background"],
  "tta_mAlp-0p35GeV"  :  ["tta_mAlp-0p35GeV_skimmed_looseSemileptonic_hists.root", "signal"],
  "TTJets"            :  ["TTJets_skimmed_looseSemileptonic_hists.root", "background"],
  "TTbar_inclusive"   :  ["TT_skimmed_looseSemileptonic_hists.root", "background"],

}

color_palette = [
  TColor.GetColor(230, 159, 0),
  TColor.GetColor(86, 180, 233),
  TColor.GetColor(0, 158, 115),
  TColor.GetColor(0, 114, 178),
  TColor.GetColor(213, 94, 0),
]

lines = {
  #name                   color
  "tta_mAlp-0p35GeV"  :  [color_palette[0], ROOT.kSolid],
  "TTbar_inclusive"   :  [33, ROOT.kSolid],
  "TTJets"            :  [36, ROOT.kSolid],
} 

legends = {
  #name                   color
  "tta_mAlp-0p35GeV"  :  "m_{a} = 0.35 GeV",
  "TTbar_inclusive"   :  "t#bar{t} incl.",
  "TTJets"            :  "t#bar{t} + jets",
}

signal_legend_position = (0.7,0.7,0.85,0.75)
background_legend_position = (0.7,0.75,0.85,0.85)

variables = {
# key                    title        norm1   rebin    xmin     xmax     ymin,  ymax,  xlabel                 ylabel
  "n_muons"   :          ("",          True,   1,       0,       10,      -1,    -1,    "Number of muons",    "Fraction of events"  ),
  "muon_pt"   :          ("",          True,   5,       0,       100,     -1,    -1,    "p_{T}^{#mu} [GeV]",  "Fraction of events"  ),
  # "muon_leading_pt" :    ("",          True,   5,       0,       100,     -1,    -1,    "Leading p_{T}^{#mu} [GeV]",  "Fraction of events"  ),
  # "muon_subleading_pt" : ("",          True,   5,       0,       100,     -1,    -1,    "All subleading p_{T}^{#mu} [GeV]",  "Fraction of events"  ),
  "muon_eta"  :          ("",          True,   10,      -2.5,    2.5,     -1,    -1,    "#eta^{#mu}",         "Fraction of events"  ),
  "muon_dxy"  :          ("",          True,   2,       -10,     10,      -1,    -1,    "d_{xy}^{#mu}",       "Fraction of events"  ),
  "muon_dz"   :          ("",          True,   2,       -10,     10,      -1,    -1,    "d_{z}^{#mu}",        "Fraction of events"  ),
  "n_eles"    :          ("",          True,   1,       0,       10,      -1,    -1,    "Number of electrons","Fraction of events"  ),
  "ele_pt"    :          ("",          True,   5,       0,       200,     -1,    -1,    "p_{T}^{e} [GeV]",    "Fraction of events"  ),
  "ele_eta"   :          ("",          True,   10,      -2.5,    2.5,     -1,    -1,    "#eta^{e}",           "Fraction of events"  ),
  "ele_dxy"   :          ("",          True,   2,       -10,     10,      -1,    -1,    "d_{xy}^{e}",         "Fraction of events"  ),
  "ele_dz"    :          ("",          True,   2,       -10,     10,      -1,    -1,    "d_{z}^{e}",          "Fraction of events"  ),
  "n_jets"    :          ("",          True,   1,       0,       30,      -1,    -1,    "Number of jets",     "Fraction of events"  ),
  "jet_pt"    :          ("",          True,   5,       0,       200,     -1,    -1,    "p_{T}^{j} [GeV]",    "Fraction of events"  ),
  "jet_eta"   :          ("",          True,   10,      -2.5,    2.5,     -1,    -1,    "#eta^{j}",           "Fraction of events"  ),
  "jet_btagDeepB":       ("",          True,   2,       -1,      1,       -1,    -1,    "jet btagDeepB",      "Fraction of events"  ),
  "cutFlow"   :          ("cutflow",   False,  1,       0,       7,       -1,    -1,    "Selection",          "Number of events"  ),
}

efficiency_plots = {
# key                     from hist  title                  xlabel       ylabel
  "cutFlowEfficiency":   ("cutFlow", "Cutflow efficiency",  "Selection", "Number of events"  ),
}
