nEvents = 1000

filenames = [
    # "TTTo2LNu/C853E1CF-5210-5A44-95EA-511CC3BE4245.root",
    # "TTToHadronic/0BDBFD47-3437-4145-B237-14C107687FB9.root",
    # "TTToSemiLeptonic/2ADCE027-B24B-334E-87AD-D894007D232D.root",
    # "TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root",
    "FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root",
]

# input_file_path = "/nfs/dust/cms/user/jniedzie/ttalps_cms/backgrounds/"
# output_file_path = "/afs/desy.de/user/l/lrygaard/TTALP/output/"

input_file_path = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/backgrounds/TTbar_inclusive/"
output_file_path = "./"

ttbar_categories = ["inclusive", "hh", "he", "hmu", "htau", "ee", "mumu", "tautau", "emu", "etau", "mutau", "other"]
variable_names = ["muonMaxPt", "eleMaxPt", "jetMaxPt", "jetHt"]
selection_names = ["singleLepton", "dilepton", "hadron"]

trigger_sets = {
    "hadBoth": ("HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59", "HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94"),
    "hadSinglebtag": ("HLT_PFHT450_SixPFJet36_PFBTagDeepCSV_1p59", ),
    "hadDoublebtag": ("HLT_PFHT400_SixPFJet32_DoublePFBTagDeepCSV_2p94", ),
    # "had_Doublebtag_5jet100": ("HLT_PFHT400_FivePFJet_100_100_60_30_30_DoublePFBTagDeepCSV_4p5"),
    # "had_Doublebtag_5jet120": ("HLT_PFHT400_FivePFJet_120_120_60_30_30_DoublePFBTagDeepCSV_4p5"),

    "heBoth": ("HLT_Ele28_eta2p1_WPTight_Gsf_HT150", "HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned"),
    "heEle28": ("HLT_Ele28_eta2p1_WPTight_Gsf_HT150", ),
    "heEle30_Jet35": ("HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned", ),

    "ele28": ("HLT_Ele28_WPTight_Gsf", ),
    "ele30": ("HLT_Ele30_WPTight_Gsf", ),
    "ele32": ("HLT_Ele32_WPTight_Gsf", ),
    "ele35": ("HLT_Ele35_WPTight_Gsf", ),
    "ele38": ("HLT_Ele38_WPTight_Gsf", ),
    "ele23Ele12": ("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL", ),

    "isomu24": ("HLT_IsoMu24", ),
    "mu17Mu8": ("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ", ),

    "emuMu12-8_ele23": ("HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"),
    "emuMu23_ele12": ("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL", ),
}


def GetHistogramsParameters():
    histParams = {}

    for ttbar_category in ttbar_categories:
        for variable in variable_names:
            outputDir = f"{ttbar_category}/{variable}"
            name = f"{ttbar_category}_{variable}"
            histParams[name] = (name, 1000, 0, 1000, outputDir)

            for set_name in trigger_sets.keys():
                name = f"{ttbar_category}_{variable}_{set_name}"
                histParams[name] = (name, 1000, 0, 1000, outputDir)
                name += "_eff"
                histParams[name] = (name, 1000, 0, 1000, outputDir)

                for selection in selection_names:
                    name = f"{ttbar_category}_{variable}_{set_name}_{selection}"
                    histParams[name] = (name, 1000, 0, 1000, outputDir)

    return histParams


histParams = GetHistogramsParameters()


histTitles = {key: params[0] for key, params in histParams.items()}
histNbins = {key: params[1] for key, params in histParams.items()}
histMin = {key: params[2] for key, params in histParams.items()}
histMax = {key: params[3] for key, params in histParams.items()}
histOutputDir = {key: params[4] for key, params in histParams.items()}
