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
  void save_hists(std::string output_path);
  
  std::map<std::string, TH1D*> histograms1D;
  std::map<std::string, TH2D*> histograms2D;

 private:
  std::map<std::string, std::string> histTitles;
  std::map<std::string, int> histNbins;
  std::map<std::string, float> histMin;
  std::map<std::string, float> histMax;
  std::map<std::string, std::string> histOutputDir;  
};

#endif /* HistogramsHandler_hpp */
