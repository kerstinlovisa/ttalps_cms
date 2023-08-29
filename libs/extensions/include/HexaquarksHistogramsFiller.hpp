#ifndef HexaquarksHistogramsFiller_hpp
#define HexaquarksHistogramsFiller_hpp

#include "Event.hpp"
#include "EventProcessor.hpp"
#include "Helpers.hpp"
#include "HistogramsHandler.hpp"

class HexaquarksHistogramsFiller {
 public:
  HexaquarksHistogramsFiller(std::string configPath, std::shared_ptr<HistogramsHandler> histogramsHandler_);
  ~HexaquarksHistogramsFiller();
  
  void FillJpsiPiPiMinvHists(const std::vector<std::vector<TLorentzVector>> &jPsi,
                                 const std::vector<std::vector<TLorentzVector>> &piPlus,
                                 const std::vector<std::vector<TLorentzVector>> &piMinus);

 private:
  std::shared_ptr<HistogramsHandler> histogramsHandler;
  std::unique_ptr<EventProcessor> eventProcessor;

  float nMixedEventsScale;

};

#endif /* HexaquarksHistogramsFiller_hpp */
