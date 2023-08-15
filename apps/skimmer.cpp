//  skimmer.cpp
//
//  Created by Jeremi Niedziela on 10/08/2023.

#include "ConfigManager.hpp"
#include "Event.hpp"
#include "EventProcessor.hpp"
#include "EventReader.hpp"
#include "ExtensionsHelpers.hpp"
#include "EventWriter.hpp"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    fatal() << "Usage: " << argv[0] << " config_path\n";
    exit(1);
  }

  string configPath = argv[1];
  auto configManager = make_unique<ConfigManager>(configPath);

  auto eventProcessor = make_unique<EventProcessor>(configPath);
  
  int maxEvents;
  configManager->GetValue("nEvents", maxEvents);

  string inputFilePath, outputFilePath;
  configManager->GetValue("inputFilePath", inputFilePath);
  configManager->GetValue("outputFilePath", outputFilePath);
  
  auto eventReader = make_shared<EventReader>(inputFilePath);
  auto eventWriter = make_unique<EventWriter>(outputFilePath, eventReader);

  for (int i_event = 0; i_event < eventReader->GetNevents(); i_event++) {
    if (maxEvents >= 0 && i_event >= maxEvents) break;
    if (i_event % 1000 == 0) info() << "Event: " << i_event << "\n";
  
    auto event = eventReader->GetEvent(i_event);

    if(!eventProcessor->PassesTriggerSelections(event)) continue;
    if(!eventProcessor->PassesSingleLeptonSelections(event)) continue;
    
    eventWriter->AddCurrentEvent("Events");
  }

  eventWriter->Save();

  return 0;
}