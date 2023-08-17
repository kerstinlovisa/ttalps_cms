#include "ConfigManager.hpp"
#include "Event.hpp"
#include "EventProcessor.hpp"
#include "EventReader.hpp"
#include "ExtensionsHelpers.hpp"
#include "HistogramsFiller.hpp"
#include "HistogramsHandler.hpp"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    fatal() << "Usage: " << argv[0] << " config_path\n";
    exit(1);
  }

  string configPath = argv[1];
  auto configManager = make_unique<ConfigManager>(configPath);

  auto eventProcessor = make_unique<EventProcessor>();
  

  int maxEvents;
  configManager->GetValue("nEvents", maxEvents);

  string inputFilePath;
  configManager->GetValue("inputFilePath", inputFilePath);
  
  auto histogramsHandler = make_shared<HistogramsHandler>(configPath);
  auto histogramsFiller = make_unique<HistogramsFiller>(configPath, histogramsHandler);
  auto eventReader = make_unique<EventReader>(inputFilePath);

  histogramsHandler->SetupHistograms();

  for (int i_event = 0; i_event < eventReader->GetNevents(); i_event++) {
    if (maxEvents >= 0 && i_event >= maxEvents) break;
    if (i_event % 1000 == 0) cout << "Event: " << i_event << endl;
    auto event = eventReader->GetEvent(i_event);

    string ttbarCategory = eventProcessor->GetTTbarEventCategory(event);
    histogramsFiller->FillTriggerVariables(event, "inclusive");
    histogramsFiller->FillTriggerVariables(event, ttbarCategory);
    histogramsFiller->FillTriggerVariablesPerTriggerSet(event, "inclusive");
    histogramsFiller->FillTriggerVariablesPerTriggerSet(event, ttbarCategory);
  }
  histogramsFiller->FillTriggerEfficiencies();
  histogramsHandler->SaveHistograms();

  return 0;
}