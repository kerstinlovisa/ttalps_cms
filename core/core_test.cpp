
#include "Helpers.hpp"
#include "EventReader.hpp"

using namespace std;

int main()
{
    string inputPath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/backgrounds/TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root";
    auto eventReader = make_unique<EventReader>(inputPath);

    cout<<"\n\nentering event loop\n\n"<<endl;

    for(int i_event=0; i_event<1; i_event++){

        auto event = eventReader->GetEvent(i_event);
        
        uint runNumber = event->GetUint("run");

        cout<<"run number: "<<runNumber<<endl;

        auto muons = event->GetCollection("Muon");

        for(auto muon : *muons){
            float pt = muon->GetFloat("pt");
            cout<<"muon pt: "<<pt<<endl;
        }

    }

    return 0;
}