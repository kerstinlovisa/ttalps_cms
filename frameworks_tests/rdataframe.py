import ROOT
RDF = ROOT.ROOT.RDataFrame

input_file_path = "../data/svj_tchannel_test_pfnanoaod.root"
output_file_path = "../data/output_rdataframe_py.root"

d = RDF("Events", input_file_path)

dSmall = d.Filter("nFatJet==2")
dSmall = dSmall.Filter("FatJet_pt[0]>400 || FatJet_pt[1]>400")
  
dSmall.Snapshot("Events", output_file_path)