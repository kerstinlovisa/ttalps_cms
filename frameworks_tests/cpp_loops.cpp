#include <string>
#include <iostream>

#include <TFile.h>
#include <TTree.h>

using namespace std;

const int max_events = -1;

string input_file_path = "../data/svj_tchannel_test_pfnanoaod.root";
string output_file_path = "../data/output_cpp_loops.root";

int main()
{
    

    auto input_file = TFile::Open(input_file_path.c_str());
    auto input_tree = (TTree*)input_file->Get("Events");
  
    auto output_file = new TFile(output_file_path.c_str(), "recreate");
    output_file->cd();
    auto output_tree = input_tree->CloneTree(0);
    
    uint n_jets = 0;
    float jet_pt[9999];
    
    input_tree->SetBranchAddress("nFatJet", &n_jets);
    input_tree->SetBranchAddress("FatJet_pt", &jet_pt);
    
    int i_event_output = 0;

    for(int i_event=0; i_event<input_tree->GetEntries(); i_event++){
        if(max_events >= 0 && i_event>max_events) break;

        input_tree->GetEntry(i_event);
        
        if(n_jets != 2) continue;

        if(jet_pt[0] > 400 || jet_pt[1] > 400){
            output_tree->GetEntry(i_event_output);
            
            output_file->cd();
            output_tree->Fill();

            i_event_output++;
        }
    }

    output_tree->AutoSave();

    output_file->Close();
    input_file->Close();

    return 0;
}