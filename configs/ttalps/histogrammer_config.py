nEvents = -1

# inputFilePath = "/nfs/dust/cms/user/lrygaard/ttalps_cms/LHEtoNanoAOD_output/UL_NanoAOD/tta_mAlp-0p35GeV_nEvents-10000/tta_mAlp-0p35GeV_nEvents-10000_part-39_NanoAOD_0.root"
# outputFilePath = "./tta_mAlp-0p35GeV_nEvents-10000_part-39_NanoAOD_0.root"

# inputFilePath = "/nfs/dust/cms/user/lrygaard/ttalps_cms/tta_mAlp-0p35GeV_nEvents-100000.root"
# outputFilePath = "./tta_mAlp-0p35GeV_nEvents-100000.root"

# inputFilePath = "/nfs/dust/cms/user/jniedzie/ttalps_cms/backgrounds/TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root"
# outputFilePath = "./TTbar_inclusive_FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root"

# inputFilePath = "tta_mAlp-0p35GeV_skimmed_looseSemileptonic.root"
# outputFilePath = "./tta_mAlp-0p35GeV_skimmed_looseSemileptonic_hists.root"

# inputFilePath = "TT_skimmed_looseSemileptonic.root"
# outputFilePath = "TT_skimmed_looseSemileptonic_hists.root"
inputFilePath = "TTJets_skimmed_looseSemileptonic.root"
outputFilePath = "TTJets_skimmed_looseSemileptonic_hists.root"

histParams = {
# key              collection    variable      bins    xmin     xmax    outputdir  
  "n_muons"   :   ("Event",      "nMuon",      50,     0,       50,      ""  ),
  "muon_pt"   :   ("Muon",       "pt",         400,    0,       200,     ""  ),
  "muon_eta"  :   ("Muon",       "eta",        100,    -2.5,    2.5,     ""  ),
  "muon_dxy"  :   ("Muon",       "dxy",        400,    -20,     20,      ""  ),
  "muon_dz"   :   ("Muon",       "dz",         400,    -20,     20,      ""  ),
  "n_eles"    :   ("Event",      "nElectron",  50,     0,       50,      ""  ),
  "ele_pt"    :   ("Electron",   "pt",         400,    0,       200,     ""  ),
  "ele_eta"   :   ("Electron",   "eta",        100,    -2.5,    2.5,     ""  ),
  "ele_dxy"   :   ("Electron",   "dxy",        400,    -20,     20,      ""  ),
  "ele_dz"    :   ("Electron",   "dz",         400,    -20,     20,      ""  ),
  "n_jets"    :   ("Event",      "nJet",       50,     0,       50,      ""  ),
  "jet_pt"    :   ("Jet",        "pt",         400,    0,       200,     ""  ),
  "jet_eta"   :   ("Jet",        "eta",        100,    -2.5,    2.5,     ""  ),
  "jet_eta"   :   ("Jet",        "eta",        100,    -2.5,    2.5,     ""  ),
  "jet_btagDeepB":("Jet",        "btagDeepB",  200,    -1,      1,       ""  ),

  "muon_leading_pt"   :   ("Muon",       "pt",         400,    0,       200,     ""  ),
  "muon_subleading_pt"   :   ("Muon",       "pt",         400,    0,       200,     ""  ),

  "cutFlow"   :   ("cutFlow",    "cutFlow",    15,     0,       15,      ""  ),
}


histVariables = {key: (params[0],params[1]) for key, params in histParams.items()}

histTitles = {key: key for key, params in histParams.items()}
histNbins = {key: params[2] for key, params in histParams.items()}
histMin = {key: params[3] for key, params in histParams.items()}
histMax = {key: params[4] for key, params in histParams.items()}
histOutputDir = {key: params[5] for key, params in histParams.items()}