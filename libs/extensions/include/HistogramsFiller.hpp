//  HistogramsFiller.hpp
//
//  Created by Jeremi Niedziela on 10/08/2023.

#ifndef HistogramsFiller_hpp
#define HistogramsFiller_hpp

#include "Event.hpp"
#include "Helpers.hpp"
#include "HistogramsHandler.hpp"
#include "CutFlowManager.hpp"

class HistogramsFiller {
 public:
  HistogramsFiller(std::string configPath, std::shared_ptr<HistogramsHandler> histogramsHandler_);
  ~HistogramsFiller();

  void FillTriggerEfficiencies();
  void FillTriggerVariables(const std::shared_ptr<Event> event, std::string prefix = "", std::string suffix = "");
  void FillTriggerVariablesPerTriggerSet(const std::shared_ptr<Event> event, std::string ttbarCategory = "");

  void FillHistograms1D(const std::shared_ptr<Event> event);
  void FillDefaultHistograms1D(const std::shared_ptr<Event> event, std::string histName, std::vector<std::string> variableLocation);
  void FillSubLeadingPt(const std::shared_ptr<Event> event, std::string histName, std::vector<std::string> variableLocation);

  void FillCutFlowHist(const std::shared_ptr<CutFlowManager> cutFlowManager);

  void FillTTAlpsHists(const std::shared_ptr<Event> event);
  

 private:
  std::shared_ptr<HistogramsHandler> histogramsHandler;

  std::map<std::string, std::vector<std::string>> triggerSets;
  std::map<std::string, std::vector<std::string>> histVariables;

  std::vector<std::string> triggerNames;
  bool EndsWithTriggerName(std::string name);
};

#endif /* HistogramsFiller_hpp */
