nEvents = 10000

inputFilePath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/backgrounds/TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root"
outputFilePath = "./histogram.root"


histParams = {
# key              collection    variable      bins    xmin     xmax    outputdir  
  "n_muons"   :   ("Event",      "nMuon",      10,     0,       10,      ""  ),
  "muon_pt"   :   ("Muon",       "pt",         200,    0,       200,     ""  ),
  "muon_eta"  :   ("Muon",       "eta",        100,    -2.5,    2.5,     ""  ),
  "muon_dxy"  :   ("Muon",       "dxy",        400,    -20,     20,      ""  ),
  "muon_dz"   :   ("Muon",       "dz",         400,    -20,     20,      ""  ),
  "n_eles"    :   ("Event",      "nElectron",  10,     0,       10,      ""  ),
  "ele_pt"    :   ("Electron",   "pt",         200,    0,       200,     ""  ),
  "ele_eta"   :   ("Electron",   "eta",        100,    -2.5,    2.5,     ""  ),
  "ele_dxy"   :   ("Electron",   "dxy",        400,    -20,     20,      ""  ),
  "ele_dz"    :   ("Electron",   "dz",         400,    -20,     20,      ""  ),
  "n_jets"    :   ("Event",      "nJet",       10,     0,       10,      ""  ),
  "jet_pt"    :   ("jet",        "pt",         200,    0,       200,     ""  ),
  "jet_eta"   :   ("jet",        "eta",        100,    -2.5,    2.5,     ""  ),
  "jet_dxy"   :   ("jet",        "dxy",        400,    -20,     20,      ""  ),
  "jet_dz"    :   ("jet",        "dz",         400,    -20,     20,      ""  ),
}

histVariables = {key: (params[0],params[1]) for key, params in histParams.items()}

histTitles = {key: key for key, params in histParams.items()}
histNbins = {key: params[2] for key, params in histParams.items()}
histMin = {key: params[3] for key, params in histParams.items()}
histMax = {key: params[4] for key, params in histParams.items()}
histOutputDir = {key: params[5] for key, params in histParams.items()}