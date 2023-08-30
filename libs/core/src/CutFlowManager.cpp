//  CutFlowManager.cpp
//
//  Created by Jeremi Niedziela on 16/08/2023.

#include "CutFlowManager.hpp"

#include "Helpers.hpp"
#include "Logger.hpp"

using namespace std;

CutFlowManager::CutFlowManager(shared_ptr<EventReader> eventReader_, shared_ptr<EventWriter> eventWriter_)
    : eventReader(eventReader_), eventWriter(eventWriter_), currentIndex(0) {
  if (!eventReader->inputFile->Get("CutFlow")) {
    info() << "Input file doesn't contain CutFlow directory yet... will create a new one in the output file.\n";
    eventWriter->outFile->mkdir("CutFlow");
  } else {
    hasInitial = true;
    info() << "Input file contains CutFlow directory - will copy it to the output file.\n";

    auto sourceDir = (TDirectory *)eventReader->inputFile->Get("CutFlow");
    eventWriter->outFile->mkdir("CutFlow");
    auto destDir = (TDirectory *)eventWriter->outFile->Get("CutFlow");

    TIter nextKey(sourceDir->GetListOfKeys());
    TKey *key;

    while ((key = dynamic_cast<TKey *>(nextKey()))) {
      TObject *obj = key->ReadObj();
      destDir->cd();
      obj->Write(key->GetName());
      delete obj;
      currentIndex++;
    }
  }
}

CutFlowManager::~CutFlowManager() {}

void CutFlowManager::UpdateCutFlow(string cutName) {
  float weight = 1;

  string fullCutName;

  bool found = false;
  for (const auto &[key, value] : weightsAfterCuts) {
    if (key.find(cutName) != std::string::npos) {
      fullCutName = key;
      found = true;
      break;
    }
  }
  if(!found){
    fullCutName = to_string(currentIndex) + "_" + cutName;
  }

  try {
    eventReader->currentEvent->Get("genWeight");
  } catch (Exception &) {
  }

  if (weightsAfterCuts.count(fullCutName)) {
    weightsAfterCuts[fullCutName] += weight;
  } else {
    weightsAfterCuts[fullCutName] = weight;
    currentIndex++;
  }
}

void CutFlowManager::SaveCutFlow() {
  eventWriter->outFile->cd("CutFlow");

  for (auto &[cutName, sumOfWeights] : weightsAfterCuts) {
    if (cutName == "0_initial" && hasInitial) {
      info() << "Initial sum of weights already exists, skipping...\n";
      continue;
    }

    auto hist = new TH1D(cutName.c_str(), cutName.c_str(), 1, 0, 1);
    hist->SetBinContent(1, sumOfWeights);
    hist->Write();
  }
  eventWriter->outFile->cd();
}
