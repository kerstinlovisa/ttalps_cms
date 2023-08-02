import ROOT
import numpy as np

max_events = -1

input_file_path = "../data/svj_tchannel_test_pfnanoaod.root"
output_file_path = "../data/output_python_loops.root"


input_file = ROOT.TFile.Open(input_file_path)
input_tree = input_file.Get("Events")
  
output_file = ROOT.TFile(output_file_path, "recreate")
output_file.cd()
output_tree = input_tree.CloneTree(0)
    
n_jets = np.zeros((1, ), dtype="i4")
jet_pt = np.zeros((9999, ), dtype="f4")

input_tree.SetBranchAddress("nFatJet", n_jets)
input_tree.SetBranchAddress("FatJet_pt", jet_pt)

i_event_output = 0

for i_event in range(input_tree.GetEntries()):
    if max_events >= 0 and i_event>max_events: 
        break

    input_tree.GetEntry(i_event)
    
    if n_jets[0] != 2: 
        continue

    if jet_pt[0] > 400 or jet_pt[1] > 400:
        output_tree.GetEntry(i_event_output)
        
        output_file.cd()
        output_tree.Fill()

        i_event_output += 1

output_tree.AutoSave()

output_file.Close()
input_file.Close()
