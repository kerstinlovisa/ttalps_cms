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
  base_path = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/"
  input_file_name = f"{base_path}/backgrounds/TTbar_inclusive/skimmed_looseSemileptonic/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root"
  cut_flow_background, cut_flow_background_relative, cut_flow_background_absolute = get_cuflow_from_file(input_file_name)
  
  input_file_name = f"{base_path}/signals/tta_mAlp-0p35GeV/skimmed_looseSemileptonic/tta_mAlp-0p35GeV_nEvents-100000.root"
  cut_flow_signal, cut_flow_signal_relative, cut_flow_signal_absolute = get_cuflow_from_file(input_file_name)
  
  input_file_name = f"{base_path}/collision_data/SingleMuon2018/skimmed_looseSemileptonic/36ED9511-D46A-0C4F-A485-C2DF1C874906.root"
  cut_flow_data, cut_flow_data_relative, cut_flow_data_absolute = get_cuflow_from_file(input_file_name)
  
  print("\n\nRaw values: ")
  print_as_markdown_table(cut_flow_background, "background", True)
  print_as_markdown_table(cut_flow_signal, "signal")
  print_as_markdown_table(cut_flow_data, "data")
  
  print("\n\nRelative:")
  print_as_markdown_table(cut_flow_background_relative, "background", True)
  print_as_markdown_table(cut_flow_signal_relative, "signal")
  print_as_markdown_table(cut_flow_data_relative, "data")
  
  print("\n\nAbsolute:")
  print_as_markdown_table(cut_flow_background_absolute, "background", True)
  print_as_markdown_table(cut_flow_signal_absolute, "signal")
  print_as_markdown_table(cut_flow_data_absolute, "data")

if __name__ == "__main__":
  main()