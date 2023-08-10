#ifndef Helpers_hpp
#define Helpers_hpp

#pragma clang diagnostic push                       // save the current state
#pragma clang diagnostic ignored "-Wdocumentation"  // turn off ROOT's warnings
#pragma clang diagnostic ignored "-Wconversion"

#include <TKey.h>

#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "TBranchElement.h"
#include "TCanvas.h"
#include "TEnv.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TGraphPolar.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLatex.h"
#include "TLeaf.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TTree.h"

#pragma clang diagnostic pop  // restores the saved state for diagnostics

#include <any>
#include <sstream>
#include <string>

#include "Logger.hpp"

const int maxCollectionElements = 9999;

inline std::vector<std::string> getListOfTrees(TFile *file) {
  auto keys = file->GetListOfKeys();
  std::vector<std::string> trees;

  for (auto i : *keys) {
    auto key = (TKey *)i;
    if (strcmp(key->GetClassName(), "TTree") == 0) trees.push_back(key->GetName());
  }
  return trees;
}

inline std::vector<std::string> split(std::string input, char splitBy) {
  std::vector<std::string> parts;

  std::istringstream iss(input);
  std::string part;

  while (std::getline(iss, part, splitBy)) parts.push_back(part);
  return parts;
}

#endif /* Helpers_hpp */