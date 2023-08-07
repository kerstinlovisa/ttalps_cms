
#include "Helpers.hpp"
#include "EventReader.hpp"

using namespace std;

int main()
{
    string inputPath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/backgrounds/TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root";
    auto eventReader = make_unique<EventReader>(inputPath);

    // auto nEvents = eventReader->GetNevents();

    for (int iEvent = 0; iEvent < 3; iEvent++)
    {
        cout<<"\n event "<<iEvent<<endl;
        auto event = eventReader->GetEvent(iEvent);



        // use some event-level info
        ULong64_t eventNumber = event->Get("event");
        double eventNumbers = event->Get("events"); // this is to demonstrate what will happen if you try to access a branch that doesn't exist
        float runNumber = event->Get("run"); // this is to demonstrate what will happen if you try to cast to a wrong type
        bool hltFlag = event->Get("HLT_Mu8_IP3_part4");
        cout<<"Event "<<eventNumber<<" from run: "<<runNumber<<" has the HLT flag: "<<hltFlag<<endl;

        // get a collection and its size
        uint nGenParticles = event->Get("nGenPart");
        int maxParticles = nGenParticles > 10 ? 10 : nGenParticles;
        auto genParticles = event->GetCollection("GenPart");
        
        for (int iParticle = 0; iParticle < maxParticles; iParticle++)
        {
            // access elements of the collection
            auto particle = genParticles->at(iParticle);

            // use some element-level info
            int pdgId = particle->Get("pdgId");
            float pt = particle->Get("pt");
            cout<<"\tParticle ("<<pdgId<<") pt: "<<pt<<endl;
        }

        // and just another example with muons
        uint nMuons = event->Get("nMuon");
        if (nMuons < 1)
            continue;

        auto muons = event->GetCollection("Muon");
        for (int iMuon = 0; iMuon < nMuons; iMuon++)
        {
            float pt = muons->at(iMuon)->Get("pt");
            cout<<"\tMuon pt: "<<pt<<endl;
        }
    }

    return 0;
}