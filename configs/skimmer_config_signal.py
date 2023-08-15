nEvents = 10000



inputFilePath = "/nfs/dust/cms/user/lrygaard/ttalps_cms/LHEtoNanoAOD_output/UL_NanoAOD/tta_mAlp-MASSGeV_nEvents-10000/tta_mAlp-MASSGeV_nEvents-10000_part-PART_NanoAOD_SUBP.root"
outputFilePath = "output/tta_mAlp-MASSGeV_nEvents-10000_part-PART/skimmer_SUBP.root"

triggerSelection = (
    "HLT_Ele28_eta2p1_WPTight_Gsf_HT150",
    "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned",
    "HLT_Ele28_WPTight_Gsf",
    "HLT_Ele30_WPTight_Gsf",
    "HLT_Ele32_WPTight_Gsf",
    "HLT_IsoMu24",
)

selectionOption = "singleLeptonPlusMuons"


extraEventCollections = {
    "ElectronPt30": {
        "inputCollections": ("Electron", ),
        "pt": (30., 9999999.),
        "eta": (-2.4, 2.4),
    },
    "ElectronPt15": {
        "inputCollections": ("Electron", ),
        "pt": (15., 9999999.),
        "eta": (-2.5, 2.5),
    },
    "MuonPt30": {
        "inputCollections": ("Muon", ),
        "pt": (30., 9999999.),
        "eta": (-2.4, 2.4),
    },
    "MuonPt15": {
        "inputCollections": ("Muon", ),
        "pt": (15., 9999999.),
        "eta": (-2.5, 2.5),
    },
    "MuonPt5": {
        "inputCollections": ("Muon", ),
        "pt": (5., 9999999.),
        "eta": (-2.5, 2.5),
    },
    "JetPt30": {
        "inputCollections": ("Jet", ),
        "pt": (30., 9999999.),
        "eta": (-2.4, 2.4),
    },
    "JetBtagged": {
        "inputCollections": ("Jet", ),
        "pt": (30., 9999999.),
        "eta": (-2.4, 2.4),
        "btagDeepB": (0.5, 9999999.),
    },
}

