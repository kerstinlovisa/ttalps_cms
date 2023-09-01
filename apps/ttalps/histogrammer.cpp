#include "ConfigManager.hpp"
#include "Event.hpp"
#include "EventReader.hpp"
#include "TTAlpsHistogramsFiller.hpp"
#include "HistogramsHandler.hpp"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    fatal() << "Usage: " << argv[0] << " config_path\n";
    exit(1);
  }

  string configPath = argv[1];

  auto eventReader = make_unique<EventReader>(configPath);
  auto histogramsHandler = make_shared<HistogramsHandler>(configPath);
  auto histogramsFiller = make_unique<TTAlpsHistogramsFiller>(configPath, histogramsHandler);

  histogramsHandler->SetupHistograms();

  for (int i_event = 0; i_event < eventReader->GetNevents(); i_event++) {
    auto event = eventReader->GetEvent(i_event);

    histogramsFiller->FillHistograms1D(event);

  }

  histogramsHandler->SaveHistograms();

  return 0;
}