nEvents = 60000

nMixedEvents = 10000000

maxNdaughters = 10

# inputFilePath = "/Users/jeremi/Documents/Physics/DESY/hexaquarks/data/hexaquarks_lclc.root"
# outputFilePath = "./hexaquarks_lclc.root"

inputFilePath = "/Users/jeremi/Documents/Physics/DESY/hexaquarks/data/hexaquarks_lclc_jpsi2mumu.root"
outputFilePath = "./hexaquarks_lclc_jpsi2mumu.root"


histParams = {
    "m_inv": ("m_inv", 1000, 0, 10, ""),
    "m_inv_from_hexa": ("m_inv_from_hexa", 1000, 0, 10, ""),
    "m_inv_mixed": ("m_inv_mixed", 1000, 0, 10, ""),
    
    "m_inv_after_cuts": ("m_inv_after_cuts", 1000, 0, 10, ""),
    "m_inv_after_cuts_from_hexa": ("m_inv_after_cuts_from_hexa", 1000, 0, 10, ""),
    "m_inv_after_cuts_mixed": ("m_inv_after_cuts_mixed", 1000, 0, 10, ""),
    
    "delta_eta_pi_pi": ("delta_eta_pi_pi", 1000, 0, 10, ""),
    "delta_eta_jPsi_pi": ("delta_eta_jPsi_pi", 1000, 0, 10, ""),
    "delta_phi_pi_pi": ("delta_phi_pi_pi", 1000, 0, 3.5, ""),
    "delta_phi_jPsi_pi": ("delta_phi_jPsi_pi", 1000, 0, 3.5, ""),
    "delta_r_pi_pi": ("delta_r_pi_pi", 1000, 0, 10, ""),
    "delta_r_jPsi_pi": ("delta_r_jPsi_pi", 1000, 0, 10, ""),   
    
    "delta_eta_pi_pi_from_hexa": ("delta_eta_pi_pi_from_hexa", 1000, 0, 10, ""),
    "delta_eta_jPsi_pi_from_hexa": ("delta_eta_jPsi_pi_from_hexa", 1000, 0, 10, ""),
    "delta_phi_pi_pi_from_hexa": ("delta_phi_pi_pi_from_hexa", 1000, 0, 3.5, ""),
    "delta_phi_jPsi_pi_from_hexa": ("delta_phi_jPsi_pi_from_hexa", 1000, 0, 3.5, ""),
    "delta_r_pi_pi_from_hexa": ("delta_r_pi_pi_from_hexa", 1000, 0, 10, ""),
    "delta_r_jPsi_pi_from_hexa": ("delta_r_jPsi_pi_from_hexa", 1000, 0, 10, ""),   
}

histTitles = {key: params[0] for key, params in histParams.items()}
histNbins = {key: params[1] for key, params in histParams.items()}
histMin = {key: params[2] for key, params in histParams.items()}
histMax = {key: params[3] for key, params in histParams.items()}
histOutputDir = {key: params[4] for key, params in histParams.items()}
