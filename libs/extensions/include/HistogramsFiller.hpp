//  HistogramsFiller.hpp
//
//  Created by Jeremi Niedziela on 10/08/2023.

#ifndef HistogramsFiller_hpp
#define HistogramsFiller_hpp

#include "Event.hpp"
#include "Helpers.hpp"
#include "HistogramsHandler.hpp"

class HistogramsFiller {
 public:
  HistogramsFiller(std::string configPath, std::shared_ptr<HistogramsHandler> histogramsHandler_);
  ~HistogramsFiller();

  void fill_trigger_efficiencies();
  void fill_hist_variables(const std::shared_ptr<Event> event, std::string prefix = "", std::string suffix = "");
  void fill_hists_for_trigger_sets(const std::shared_ptr<Event> event, std::string ttbar_category = "");

 private:
  std::shared_ptr<HistogramsHandler> histogramsHandler;
  
  std::map<std::string, std::vector<std::string>> trigger_sets;
  
  std::vector<std::string> triggerNames;
  bool EndsWithTriggerName(std::string name);
};

#endif /* HistogramsFiller_hpp */
