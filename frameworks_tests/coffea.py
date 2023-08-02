import uproot

import awkward as ak
from coffea import processor
from coffea.nanoevents.methods import candidate

input_file_path = "../data/svj_tchannel_test_pfnanoaod.root"
output_file_path = "../data/output_python_loops.root"

class MyProcessor(processor.ProcessorABC):
    def __init__(self):
        pass

    def process(self, events):
        
        jets = ak.zip(
            {"pt": events.FatJet_pt,},
            with_name="PtEtaPhiMCandidate",
            behavior=candidate.behavior,
        )
        cut = (ak.num(jets) == 2) & (jets[0].pt > 400) & (jets[1].pt > 400)
        return events[cut]

    def postprocess(self, accumulator):
        pass
    
    
processor = MyProcessor()
    
    
events = uproot.open(input_file_path + ":Events").arrays(entry_stop=1)

print(f"{events=}")

skimmed_events = processor(events)

print(f"{skimmed_events=}")