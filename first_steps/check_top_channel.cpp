#include <TFile.h>
#include <TTree.h>

#include "event_tools.hpp"

using namespace std;

const int max_events = 10;

string input_file_path = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/backgrounds/TTToSemiLeptonic/2ADCE027-B24B-334E-87AD-D894007D232D.root";

int main()
{
    auto input_file = TFile::Open(input_file_path.c_str());
    auto input_tree = (TTree*)input_file->Get("Events");
  
    uint nGenPart = 0;
    int GenPart_pdgId[9999];
    int GenPart_genPartIdxMother[9999];
    int GenPart_statusFlags[9999];
    
    input_tree->SetBranchAddress("nGenPart", &nGenPart);
    input_tree->SetBranchAddress("GenPart_pdgId", &GenPart_pdgId);
    input_tree->SetBranchAddress("GenPart_genPartIdxMother", &GenPart_genPartIdxMother);
    input_tree->SetBranchAddress("GenPart_statusFlags", &GenPart_statusFlags);
    
    map<string, int> count;

    for(int i_event=0; i_event<input_tree->GetEntries(); i_event++){
        if(max_events >= 0 && i_event>=max_events) break;

        input_tree->GetEntry(i_event);
        string short_name = get_short_name(nGenPart, GenPart_pdgId, GenPart_genPartIdxMother, GenPart_statusFlags);
        
        cout<<"Event "<<i_event<<": "<<short_name<<endl;
        
        if (count.find(short_name) == count.end()) count[short_name] = 1;
        else count[short_name]++;
    }

    for(auto &[key, value] : count) cout<<key<<": "<<value<<endl;

    input_file->Close();
    return 0;
}