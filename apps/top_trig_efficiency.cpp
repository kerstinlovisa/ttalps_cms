#include "ConfigManager.hpp"
#include "Event.hpp"
#include "EventProcessor.hpp"
#include "EventReader.hpp"
#include "ExtensionsHelpers.hpp"
#include "HistogramsHandler.hpp"
#include "HistogramsFiller.hpp"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    fatal() << "Usage: " << argv[0] << " config_path\n";
    exit(1);
  }

  string configPath = argv[1];

  auto eventProcessor = make_unique<EventProcessor>();
  auto configManager = make_unique<ConfigManager>(configPath);

  int maxEvents;
  configManager->GetValue("nEvents", maxEvents);

  string input_file_path, output_file_path;
  configManager->GetValue("input_file_path", input_file_path);
  configManager->GetValue("output_file_path", output_file_path);

  vector<string> filenames;
  configManager->GetVector("filenames", filenames);

  for (auto &filename : filenames) {
    auto histogramsHandler = make_shared<HistogramsHandler>(configPath);
    auto histogramsFiller = make_unique<HistogramsFiller>(configPath, histogramsHandler);
    auto eventReader = make_unique<EventReader>(input_file_path + filename);

    histogramsHandler->setup_histograms();

    for (int i_event = 0; i_event < eventReader->GetNevents(); i_event++) {
      if (maxEvents >= 0 && i_event >= maxEvents) break;
      if (i_event % 1000 == 0) cout << "Event: " << i_event << endl;
      auto event = eventReader->GetEvent(i_event);

      string ttbar_category = eventProcessor->GetTTbarEventCategory(event);
      histogramsFiller->fill_hist_variables(event, "inclusive");
      histogramsFiller->fill_hist_variables(event, ttbar_category);
      histogramsFiller->fill_hists_for_trigger_sets(event, "inclusive");
      histogramsFiller->fill_hists_for_trigger_sets(event, ttbar_category);
    }
    histogramsFiller->fill_trigger_efficiencies();
    histogramsHandler->save_hists(output_file_path + filename);
  }

  return 0;
}