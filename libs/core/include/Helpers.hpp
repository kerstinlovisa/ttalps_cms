#ifndef Helpers_hpp
#define Helpers_hpp

#pragma clang diagnostic push                      // save the current state
#pragma clang diagnostic ignored "-Wdocumentation" // turn off ROOT's warnings
#pragma clang diagnostic ignored "-Wconversion"

#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TTree.h"
#include "TFile.h"
#include "TEnv.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLine.h"
#include "TLatex.h"
#include "TBranchElement.h"
#include "TLeaf.h"
// #include "RooFit.h"
// #include "RooRealVar.h"
// #include "RooDataSet.h"
// #include "RooArgSet.h"
#include "TGraphPolar.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "Math/GenVector/LorentzVector.h"
#include <TKey.h>

#pragma clang diagnostic pop // restores the saved state for diagnostics

#include <string>
#include <any>

#include "Logger.hpp"

const int maxCollectionElements = 9999;

inline std::vector<std::string> getListOfTrees(TFile *file)
{
    auto keys = file->GetListOfKeys();
    std::vector<std::string> trees;

    for (auto i : *keys)
    {
        auto key = (TKey *)i;
        if (strcmp(key->GetClassName(), "TTree") == 0)
            trees.push_back(key->GetName());
    }
    return trees;
}

#endif /* Helpers_hpp */