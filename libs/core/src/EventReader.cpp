//  EventReader.cpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#include "EventReader.hpp"
#include "Helpers.hpp"

using namespace std;

EventReader::EventReader(string inputPath)
    : currentEvent(make_shared<Event>()) {
  SetupBranches(inputPath);
}

EventReader::~EventReader() {}

void EventReader::SetupBranches(string inputPath) {
  
  info() << "Opening input file...";
  TFile *inFile = TFile::Open(inputPath.c_str());
  info() << " done\n";
  
  info() << "Reading trees...";
  vector<string> treeNames = getListOfTrees(inFile);
  for (string treeName : treeNames) {
    cout << "Loading tree: " << treeName << endl;
    inputTrees[treeName] = (TTree *)inFile->Get(treeName.c_str());
  }
  info() << "done\n";

  info() << "Setting branches...";
  auto keysInEventTree = inputTrees["Events"]->GetListOfBranches();
  for (auto i : *keysInEventTree) {
    auto branch = (TBranch *)i;
    string branchName = branch->GetName();
    string branchType = branch->FindLeaf(branchName.c_str())->GetTypeName();

    if (branchType == "")
      cout << "ERROR -- couldn't find branch type for branch: " << branchName
           << endl;

    if (branchName.find("_") == std::string::npos ||
        branchName.find("HLT") == 0 || branchName.find("Flag") == 0 ||
        branchName.find("L1") == 0 || branchName.find("PV_") == 0 ||
        branchName.find("MET_") == 0 || branchName.find("GenMET_") == 0 ||
        branchName.find("CaloMET") == 0 || branchName.find("ChsMET") == 0 ||
        branchName.find("DeepMETResolutionTune") == 0 ||
        branchName.find("DeepMETResponseTune") == 0 ||
        branchName.find("GenVtx") == 0 || branchName.find("Generator") == 0 ||
        branchName.find("HTXS") == 0 || branchName.find("Pileup_") == 0 ||
        branchName.find("PuppiMET") == 0 || branchName.find("RawMET") == 0 ||
        branchName.find("RawPuppiMET") == 0 || branchName.find("TkMET") == 0 ||
        branchName.find("btagWeight") == 0) {  // it's a single number
      currentEvent->values_types[branchName] = branchType;

      if (branchType == "UInt_t") {
        currentEvent->values_uint[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(
            branchName.c_str(), &currentEvent->values_uint[branchName]);
      } else if (branchType == "Int_t") {
        currentEvent->values_int[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(
            branchName.c_str(), &currentEvent->values_int[branchName]);
      } else if (branchType == "Bool_t") {
        currentEvent->values_bool[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(
            branchName.c_str(), &currentEvent->values_bool[branchName]);
      } else if (branchType == "Float_t") {
        currentEvent->values_float[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(
            branchName.c_str(), &currentEvent->values_float[branchName]);
      } else if (branchType == "ULong64_t") {
        currentEvent->values_ulong[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(
            branchName.c_str(), &currentEvent->values_ulong[branchName]);
      } else if (branchType == "UChar_t") {
        currentEvent->values_uchar[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(
            branchName.c_str(), &currentEvent->values_uchar[branchName]);
      } else {
        cout << "ERROR -- unsupported branch type: " << branchType
             << "\t (branch name: " << branchName << ")" << endl;
      }
    } else {  // it's a collection
      string::size_type pos = branchName.find('_');
      string collectionName = branchName.substr(0, pos);
      string variableName = branchName.substr(pos + 1);

      if (!currentEvent->collections.count(collectionName)) {
        currentEvent->collections[collectionName] =
            make_shared<PhysicsObjects>();

        for (int i = 0; i < maxCollectionElements; i++) {
          currentEvent->collections[collectionName]->push_back(
              make_shared<PhysicsObject>());
        }
      }

      for (int i = 0; i < maxCollectionElements; i++) {
        currentEvent->collections[collectionName]
            ->at(i)
            ->values_types[variableName] = branchType;
      }

      if (branchType == "Float_t") {
        inputTrees["Events"]->SetBranchAddress(
            branchName.c_str(), &currentEvent->values_float_vector[branchName]);
        for (int i = 0; i < maxCollectionElements; i++) {
          currentEvent->collections[collectionName]
              ->at(i)
              ->values_float[variableName] =
              &currentEvent->values_float_vector[branchName][i];
        }
      } else if (branchType == "UChar_t") {
        inputTrees["Events"]->SetBranchAddress(
            branchName.c_str(), &currentEvent->values_uchar_vector[branchName]);
        for (int i = 0; i < maxCollectionElements; i++) {
          currentEvent->collections[collectionName]
              ->at(i)
              ->values_uchar[variableName] =
              &currentEvent->values_uchar_vector[branchName][i];
        }
      } else if (branchType == "Int_t") {
        inputTrees["Events"]->SetBranchAddress(
            branchName.c_str(), &currentEvent->values_int_vector[branchName]);
        for (int i = 0; i < maxCollectionElements; i++) {
          currentEvent->collections[collectionName]
              ->at(i)
              ->values_int[variableName] =
              &currentEvent->values_int_vector[branchName][i];
        }
      } else if (branchType == "Bool_t") {
        inputTrees["Events"]->SetBranchAddress(
            branchName.c_str(), &currentEvent->values_bool_vector[branchName]);
        for (int i = 0; i < maxCollectionElements; i++) {
          currentEvent->collections[collectionName]
              ->at(i)
              ->values_bool[variableName] =
              &currentEvent->values_bool_vector[branchName][i];
        }
      } else {
        cout << "ERROR -- unsupported branch type: " << branchType
             << "\t (branch name: " << branchName << ")" << endl;
      }
    }
  }

  info() << "done\n";
}

shared_ptr<Event> EventReader::GetEvent(int iEvent) {
  // Move to desired entry in all trees
  for (auto &[name, tree] : inputTrees) tree->GetEntry(iEvent);

  // Tell collections where to stop in loops, without actually changing their
  // size in memory
  for (auto &[name, collection] : currentEvent->collections) {
    UInt_t collectionSize = currentEvent->Get("n" + name);
    collection->ChangeVisibleSize(collectionSize);
  }
  return currentEvent;
}
