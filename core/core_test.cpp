
#include "Helpers.hpp"
#include "EventReader.hpp"

using namespace std;

int main()
{
    string inputPath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/backgrounds/TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root";
    auto eventReader = make_unique<EventReader>(inputPath);

    cout<<"\n\nentering event loop\n\n"<<endl;

    for(int i_event=0; i_event<10; i_event++){
        cout<<"\n\nevent: "<<i_event<<endl;
        auto event = eventReader->GetEvent(i_event);
        
        uint runNumber = event->GetUint("run");

        cout<<"run number: "<<runNumber<<endl;
        
        uint nMuons = event->GetUint("nMuons");
        cout<<"n muons: "<<nMuons<<endl;

        auto muons = event->GetCollection("Muon");

        cout<<"starting muons loop"<<endl;
        for(int i_muon=0; i_muon<nMuons; i_muon++){
            float pt = muons->at(i_muon)->GetFloat("pt");
            cout<<"muon pt: "<<pt<<endl;
        }

        uint nGenParticles = event->GetUint("nGenPart");
        auto genParticles = event->GetCollection("GenPart");

        for(int i_part=0; i_part<nGenParticles; i_part++){
            int pdgId =  genParticles->at(i_part)->GetInt("pdgId");
            float pt = genParticles->at(i_part)->GetFloat("pt");
            cout<<"gen particle: "<<pdgId<<"\tpt: "<<pt<<endl;
        }

        cout<<"finished event"<<endl;

    }

    return 0;
}