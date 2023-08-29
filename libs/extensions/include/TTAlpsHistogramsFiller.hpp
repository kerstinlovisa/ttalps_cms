//  TTAlpsHistogramsFiller.hpp
//
//  Created by Jeremi Niedziela on 10/08/2023.

#ifndef TTAlpsHistogramsFiller_hpp
#define TTAlpsHistogramsFiller_hpp

#include "Event.hpp"
#include "Helpers.hpp"
#include "HistogramsHandler.hpp"

class TTAlpsHistogramsFiller {
 public:
  TTAlpsHistogramsFiller(std::string configPath, std::shared_ptr<HistogramsHandler> histogramsHandler_);
  ~TTAlpsHistogramsFiller();

  void FillTriggerEfficiencies();
  void FillTriggerVariables(const std::shared_ptr<Event> event, std::string prefix = "", std::string suffix = "");
  void FillTriggerVariablesPerTriggerSet(const std::shared_ptr<Event> event, std::string ttbarCategory = "");

  void FillHistograms1D(const std::shared_ptr<Event> event);

 private:
  std::shared_ptr<HistogramsHandler> histogramsHandler;

  std::map<std::string, std::vector<std::string>> triggerSets;
  std::map<std::string, std::vector<std::string>> histVariables;

  std::vector<std::string> triggerNames;
  bool EndsWithTriggerName(std::string name);
};

#endif /* TTAlpsHistogramsFiller_hpp */
