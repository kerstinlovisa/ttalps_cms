//  HistogramsHandler.hpp
//
//  Created by Jeremi Niedziela on 08/08/2023.

#ifndef HistogramsHandler_hpp
#define HistogramsHandler_hpp

#include "Event.hpp"
#include "Helpers.hpp"

class HistogramsHandler {
 public:
  HistogramsHandler(std::string configPath);
  ~HistogramsHandler();

  void setup_histograms();

  void fill_trigger_efficiencies();
  void fill_hist_variables(const std::shared_ptr<Event> event, std::string prefix = "", std::string suffix = "");
  void fill_hists_for_trigger_sets(const std::shared_ptr<Event> event, std::string ttbar_category = "");

  void save_hists(std::string output_path);

 private:
  std::map<std::string, TH1D*> histograms1D;
  std::map<std::string, TH2D*> histograms2D;

  std::map<std::string, std::vector<std::string>> trigger_sets;
  
  std::map<std::string, std::string> histTitles;
  std::map<std::string, int> histNbins;
  std::map<std::string, float> histMin;
  std::map<std::string, float> histMax;
  std::map<std::string, std::string> histOutputDir;

  std::vector<std::string> triggerNames;
  bool EndsWithTriggerName(std::string name);
};

#endif /* HistogramsHandler_hpp */
