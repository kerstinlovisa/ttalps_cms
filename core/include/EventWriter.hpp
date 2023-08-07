//  EventWriter.hpp
//
//  Created by Jeremi Niedziela on 07/08/2023.

#ifndef EventWriter_hpp
#define EventWriter_hpp

#include "Helpers.hpp"
#include "Event.hpp"
#include "EventReader.hpp"


class EventWriter
{
public:
    EventWriter(std::string outputPath, const std::shared_ptr<EventReader> &eventReader_);
    ~EventWriter();

    void AddCurrentEvent(std::string treeName);
    void Save();

private:
    TFile *outFile;
    std::map<std::string, TTree *> outputTrees;

    std::shared_ptr<EventReader> eventReader;

    void SetupOutputTree(std::string outFileName);
};

#endif /* EventWriter_hpp */
