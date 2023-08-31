nEvents = -1

# background
# inputFilePath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/backgrounds/TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root"
# outputFilePath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/backgrounds/TTbar_inclusive/skimmed_looseSemileptonic/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root"

# signal
inputFilePath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/signals/tta_mAlp-0p35GeV/tta_mAlp-0p35GeV_nEvents-100000.root"
outputFilePath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/signals/tta_mAlp-0p35GeV/skimmed_looseSemileptonic/tta_mAlp-0p35GeV_nEvents-100000.root"

# data
# inputFilePath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/collision_data/SingleMuon2018/36ED9511-D46A-0C4F-A485-C2DF1C874906.root"
# outputFilePath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/collision_data/SingleMuon2018/skimmed_looseSemileptonic/36ED9511-D46A-0C4F-A485-C2DF1C874906.root"

triggerSelection = (
    "HLT_Ele28_eta2p1_WPTight_Gsf_HT150",
    "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned",
    "HLT_Ele28_WPTight_Gsf",
    "HLT_Ele30_WPTight_Gsf",
    "HLT_Ele32_WPTight_Gsf",
    "HLT_IsoMu24",
)

extraEventCollections = {
    "GoodLeptons": {
        "inputCollections": ("Muon", "Electron"),
        "pt": (30., 9999999.),
        "eta": (-2.4, 2.4),
    },
    "GoodJets": {
        "inputCollections": ("Jet", ),
        "pt": (30., 9999999.),
        "eta": (-2.4, 2.4),
    },
    "GoodBtaggedJets": {
        "inputCollections": ("Jet", ),
        "pt": (30., 9999999.),
        "eta": (-2.4, 2.4),
        "btagDeepB": (0.5, 9999999.),
    },
}

eventSelections = {
    "MET_pt": (30, 9999999),
    "nGoodLeptons": (1, 9999999),
    "nGoodJets": (4, 9999999),
    "nGoodBtaggedJets": (1, 9999999),
}