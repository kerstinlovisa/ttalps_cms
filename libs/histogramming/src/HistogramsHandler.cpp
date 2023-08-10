//  HistogramsHandler.cpp
//
//  Created by Jeremi Niedziela on 08/08/2023.

#include "HistogramsHandler.hpp"

#include "ConfigManager.hpp"
#include "EventProcessor.hpp"
#include "ExtensionsHelpers.hpp"

using namespace std;

HistogramsHandler::HistogramsHandler(string configPath) {
  auto configManager = std::make_unique<ConfigManager>(configPath);
  configManager->GetMap("histTitles", histTitles);
  configManager->GetMap("histNbins", histNbins);
  configManager->GetMap("histMin", histMin);
  configManager->GetMap("histMax", histMax);
  configManager->GetMap("histOutputDir", histOutputDir);
}

HistogramsHandler::~HistogramsHandler() {}

void HistogramsHandler::setup_histograms() {
  for (auto &[name, title] : histTitles) {
    histograms1D[name] = new TH1D(title.c_str(), title.c_str(), histNbins[name], histMin[name], histMax[name]);
  }
}

void HistogramsHandler::save_hists(std::string output_path) {
  auto output_file = new TFile((output_path).c_str(), "recreate");
  output_file->cd();

  for (auto &[name, hist] : histograms1D) {
    string outputDir = histOutputDir[name];
    if(!output_file->Get(outputDir.c_str())) output_file->mkdir(outputDir.c_str());
    output_file->cd(outputDir.c_str());
    hist->Write();
  }
  output_file->Close();
}