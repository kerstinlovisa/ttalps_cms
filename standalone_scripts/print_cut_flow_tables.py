from ROOT import TFile

def get_histograms_in_directory(root_dir):
    
    histograms = []

    for key in root_dir.GetListOfKeys():
        obj = key.ReadObj()
        if obj.InheritsFrom("TH1"):
            histograms.append(key.GetName())

    return histograms

def print_as_markdown_table(values, row_title="", print_header=False):

  if print_header:
    print("~~~ | ", end="")
    for name in values.keys():
      print(f"  {name}   |", end = "")
      
    print()
    print("--- | ", end="")
    
    for name in values.keys():
      print(f"  ---   |", end = "")
      
      
    print()
  
  print(f"{row_title} | ", end="")
    
  for value in values.values():
    print(f"  {value:.3f}   |".replace(".", ","), end = "")
    
  print()
    

def get_cuflow_from_file(input_file_name):
  input_file = TFile.Open(input_file_name)
  root_dir = input_file.Get("CutFlow")
  
  histogram_names = get_histograms_in_directory(root_dir)
  
  cut_flow = {}
  
  for name in histogram_names:
    hist = input_file.Get(f"CutFlow/{name}")
    entries = hist.GetBinContent(1)
  
    cut_flow[name] = entries
  
  cut_flow_relative = {}
  cut_flow_absolute = {}
  first = True
  previous_name = ""
  first_name = ""
  
  for name, count in cut_flow.items():
    if first:
      cut_flow_relative[name] = 1.0
      cut_flow_absolute[name] = 1.0
      first = False
      first_name = name
    else:
      cut_flow_relative[name] = count / cut_flow[previous_name]
      cut_flow_absolute[name] = count / cut_flow[first_name]
      
    previous_name = name
  
  return cut_flow, cut_flow_relative, cut_flow_absolute

def main():
  
  # skim = "skimmed_looseSemileptonic"
  # skim = "skimmed_signalLike"
  skim = "skimmed_ttbarLike"
  
  # base_path = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/"
  base_path = "/nfs/dust/cms/user/jniedzie/ttalps_cms/"
  
  file_names = {
    "TTbar": (("TTbar_inclusive", "FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root"),),
    "tW": (("ST_tW_top", "776A38DC-FF27-6F4E-9B16-C55B696BAA92.root"),),
    "t~W": (("ST_tW_antitop", "09B1D3CA-5FCC-0A48-BFA6-E1759D5D7D02.root"),),
    "ttW": (("ttWJets", "5B123882-8484-1B47-9A07-57F8F526F6EF.root"),),
    "ttZ": (("ttZJets", "EB2F627D-0570-7C4C-A561-C29B6E4F123A.root"),),
    "ttH": (("ttHToMuMu", "D41A5AFC-EC31-A64F-9E87-6F1C22ED6DCB.root"),),
    "QCD": (
      ("QCD_Pt_30to50", "72E0DD65-5D9D-3E47-99A9-AAB4E4ACC724.root"),
      ("QCD_Pt_50to80", "CAB74055-4D6E-024F-9B89-465541A2B906.root"),
      ("QCD_Pt_80to120", "5D2183A8-8AC4-7841-9308-A1A221F27EBC.root"),
      ("QCD_Pt_120to170", "EFC4B90E-252D-5145-BE99-C880CAE3B061.root"),
      ("QCD_Pt_170to300", "5C540F1F-6B0C-1047-B020-539529AB3BB6.root"),
      ("QCD_Pt_300to470", "C5A7D337-F76C-B848-A17F-10BBBD72042B.root"),
      ("QCD_Pt_470to600", "452A92CA-5CED-A944-A44F-54CBD68D33CE.root"),
      ("QCD_Pt_600to800", "778F37B3-2E2D-024A-ADB2-66699F11783C.root"),
      ("QCD_Pt_800to1000", "191A7BDE-A93B-9440-9BC9-A6BEE36F43EE.root"),
      ("QCD_Pt_1000to1400", "C5C48DC6-E709-244D-B649-A6CC41B3F190.root"),
      ("QCD_Pt_1400to1800", "99B44156-580D-0249-9241-9E88DF59F04B.root"),
      ("QCD_Pt_1800to2400", "061D96A4-FE04-3348-9F8B-E7E9BA6E4327.root"),
      ("QCD_Pt_2400to3200", "A937F1B0-3D60-4E4D-B9EF-F7882A82450E.root"),
      ("QCD_Pt_3200toInf", "980542CA-9396-6344-88E3-EA9CCB639159.root"),
    ),
    "tta (0.35 GeV)": (("tta_mAlp-0p35GeV", "tta_mAlp-0p35GeV_nEvents-100000.root"),),
    "data": (("SingleMuon2018", "36ED9511-D46A-0C4F-A485-C2DF1C874906.root"),),
  }
  
  first = True
  
  for label, processes in file_names.items():
    
    for sample_name, file_name in processes:
      
      sub_directory = "backgrounds"
      if label == "tta (0.35 GeV)":
        sub_directory = "signals"
      elif label == "data":
        sub_directory = "collision_data"
      
      input_file_name = f"{base_path}/{sub_directory}/{sample_name}/{skim}/{file_name}"
      cut_flow, cut_flow_relative, cut_flow_absolute = get_cuflow_from_file(input_file_name)
  
      # print("\n\nRaw values: ")
      # print_as_markdown_table(cut_flow, label, True)
  
      # print("\n\nRelative:")
      # print_as_markdown_table(cut_flow_relative, label, True)
    
      # print("\n\nAbsolute:")
      print_as_markdown_table(cut_flow_absolute, sample_name, first)
      first = False
      

if __name__ == "__main__":
  main()