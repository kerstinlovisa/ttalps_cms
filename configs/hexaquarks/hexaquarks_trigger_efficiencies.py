nEvents = 1000

nMixedEventsScale = 0.01

inputFilePath = "/Users/jeremi/Documents/Physics/DESY/hexaquarks/data/hexaquarks_lclc.root"
outputFilePath = "./hexaquarks_lclc.root"

histParams = {
    "m_inv": ("m_inv", 1000, 0, 10, ""),
    "m_inv_mixed": ("m_inv_mixed", 1000, 0, 10, ""),
}

histTitles = {key: params[0] for key, params in histParams.items()}
histNbins = {key: params[1] for key, params in histParams.items()}
histMin = {key: params[2] for key, params in histParams.items()}
histMax = {key: params[3] for key, params in histParams.items()}
histOutputDir = {key: params[4] for key, params in histParams.items()}
