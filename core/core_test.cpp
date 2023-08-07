
#include "Helpers.hpp"
#include "EventReader.hpp"
#include "EventWriter.hpp"

using namespace std;

bool showBadExamples = true;

void printEventInto(const shared_ptr<Event> event)
{
    if (showBadExamples)
    {
        double eventNumberBad = event->Get("events"); // this is to demonstrate what will happen if you try to access a branch that doesn't exist
        float runNumberBad = event->Get("run");       // this is to demonstrate what will happen if you try to cast to a wrong type
    }

    ULong64_t eventNumber = event->Get("event");
    uint runNumber = event->Get("run");
    bool hltFlag = event->Get("HLT_Mu8_IP3_part4");

    cout << "Event " << eventNumber << " from run: " << runNumber << " has the HLT flag: " << hltFlag << endl;
}

void printGenParticlesInfo(const shared_ptr<Event> event)
{
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
        cout << "\tParticle (" << pdgId << ") pt: " << pt << endl;
    }
}

void printMuonsInfo(const shared_ptr<Event> event)
{
    if (showBadExamples)
    {
        auto muonsBad = event->GetCollection("Muons"); // this is to demonstrate what will happen if you try t access a branch that doesn't exist
    }

    auto muons = event->GetCollection("Muon");

    uint nMuons = event->Get("nMuon");

    for (int iMuon = 0; iMuon < nMuons; iMuon++)
    {
        float pt = muons->at(iMuon)->Get("pt");

        if (showBadExamples)
        {
            bool ptBad = muons->at(iMuon)->Get("pt"); // this is to demonstrate what will happen if you try to cast to a wrong type
        }

        cout << "\tMuon pt: " << pt << endl;
    }
}

int main()
{
    string inputPath = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/backgrounds/TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root";
    string outputPath = "test.root";

    auto eventReader = make_shared<EventReader>(inputPath);
    auto eventWriter = make_unique<EventWriter>(outputPath, eventReader);
    
    // auto nEvents = eventReader->GetNevents();

    for (int iEvent = 0; iEvent < 10; iEvent++)
    {
        cout << "\n event " << iEvent << endl;
        auto event = eventReader->GetEvent(iEvent);
        uint nMuons = event->Get("nMuon");
        if (nMuons < 1)
            continue;

        printEventInto(event);
        // printGenParticlesInfo(event);
        printMuonsInfo(event);

        eventWriter->AddCurrentEvent("Events");
    }

    eventWriter->Save();

    return 0;
}