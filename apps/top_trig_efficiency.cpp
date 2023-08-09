#include <string>
#include <iostream>

#include <TFile.h>
#include <TH1D.h>

#include "Event.hpp"
#include "EventReader.hpp"
#include "EventProcessor.hpp"
#include "HistogramsHandler.hpp"

using namespace std;

const int max_events = -1;

// string input_file_path = "/nfs/dust/cms/user/jniedzie/ttalps_cms/backgrounds/";
// string output_file_path = "/afs/desy.de/user/l/lrygaard/TTALP/output/";

string input_file_path = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/backgrounds/TTbar_inclusive/";
string output_file_path = "./";

vector<string> filenames = {
  // "TTTo2LNu/C853E1CF-5210-5A44-95EA-511CC3BE4245.root",
  // "TTToHadronic/0BDBFD47-3437-4145-B237-14C107687FB9.root",
  // "TTToSemiLeptonic/2ADCE027-B24B-334E-87AD-D894007D232D.root",
  // "TTbar_inclusive/FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root",
  "FCA55055-C8F3-C44B-8DCC-6DCBC0B8B992.root",
};

vector<string> ttbar_categories = {"", "hh", "he", "hmu", "htau", "ee", "mumu", "tautau", "emu", "etau", "mutau"};

int main()
{
  auto histogramsHandler = make_unique<HistogramsHandler>();
  EventProcessor &eventProcessor = EventProcessor::getInstance();

  for(auto &filename : filenames) {

    auto eventReader = make_unique<EventReader>(input_file_path+filename);
    cout<<"Event reader created for input file: "<<input_file_path+filename<<endl;
    

    histogramsHandler->setup_histograms();

    int n_events = 172000;

    for(int i_event=0; i_event<n_events; i_event++){
      if(max_events >= 0 && i_event>max_events) break;
      if(i_event % 1000 == 0) cout << "Event: " << i_event << endl;

      auto event = eventReader->GetEvent(i_event);

      string ttbar_category = eventProcessor.GetTTbarEventCategory(event);
      if(find(ttbar_categories.begin(), ttbar_categories.end(), ttbar_category) == ttbar_categories.end()){
        if(ttbar_category != "error") cout << "Decay mode: " << ttbar_category << " not found - skipped" << endl;
        continue;
      }

      histogramsHandler->fill_hist_variables(event);
      histogramsHandler->fill_hist_variables(event,ttbar_category);

      histogramsHandler->fill_hists_for_trigger_sets(event, "");
      histogramsHandler->fill_hists_for_trigger_sets(event, ttbar_category);
    }

    histogramsHandler->fill_trigger_efficiencies();

    histogramsHandler->save_hists(output_file_path+filename);
  }

  return 0;
}