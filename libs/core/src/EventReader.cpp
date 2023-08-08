//  EventReader.cpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#include "EventReader.hpp"

#include "Helpers.hpp"

using namespace std;

EventReader::EventReader(string inputPath) : currentEvent(make_shared<Event>()) { SetupBranches(inputPath); }

EventReader::~EventReader() {}

void EventReader::SetupBranches(string inputPath) {
  TFile *inFile = TFile::Open(inputPath.c_str());
  vector<string> treeNames = getListOfTrees(inFile);
  for (string treeName : treeNames) {
    cout << "Loading tree: " << treeName << endl;
    inputTrees[treeName] = (TTree *)inFile->Get(treeName.c_str());
  }
  
  auto keysInEventTree = inputTrees["Events"]->GetListOfBranches();
  for (auto i : *keysInEventTree) {
    auto branch = (TBranch *)i;
    string branchName = branch->GetName();
    string branchType = branch->FindLeaf(branchName.c_str())->GetTypeName();
    if (branchType == "") error() << "Couldn't find branch type for branch: " << branchName << "\n";

    bool branchIsVector = false;

    TLeaf *leaf = branch->GetLeaf(branch->GetName());
    if (leaf) {
      branchIsVector = leaf->GetLenStatic() > 1 || leaf->GetLeafCount() != nullptr;
    } else {
      fatal() << "Couldn't get leaf for branch: " << branchName << "\n";
      exit(1);
    }

    if (branchIsVector) {
      SetupVectorBranch(branchName, branchType);
    } else {
      SetupScalarBranch(branchName, branchType);
    }
  }
}

void EventReader::SetupScalarBranch(string branchName, string branchType) {
  currentEvent->values_types[branchName] = branchType;

  if (branchType == "UInt_t") {
    currentEvent->values_uint[branchName] = 0;
    inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_uint[branchName]);
  } else if (branchType == "Int_t") {
    currentEvent->values_int[branchName] = 0;
    inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_int[branchName]);
  } else if (branchType == "Bool_t") {
    currentEvent->values_bool[branchName] = 0;
    inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_bool[branchName]);
  } else if (branchType == "Float_t") {
    currentEvent->values_float[branchName] = 0;
    inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_float[branchName]);
  } else if (branchType == "ULong64_t") {
    currentEvent->values_ulong[branchName] = 0;
    inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_ulong[branchName]);
  } else if (branchType == "UChar_t") {
    currentEvent->values_uchar[branchName] = 0;
    inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_uchar[branchName]);
  } else {
    cout << "ERROR -- unsupported branch type: " << branchType << "\t (branch name: " << branchName << ")" << endl;
  }
}

void EventReader::SetupVectorBranch(string branchName, string branchType) {
  string::size_type pos = branchName.find('_');
  string collectionName = branchName.substr(0, pos);
  string variableName = branchName.substr(pos + 1);

  InitializeCollection(collectionName);

  for (int i = 0; i < maxCollectionElements; i++) {
    currentEvent->collections[collectionName]->at(i)->values_types[variableName] = branchType;
  }

  if (branchType == "Float_t") {
    inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_float_vector[branchName]);
    for (int i = 0; i < maxCollectionElements; i++) {
      currentEvent->collections[collectionName]->at(i)->values_float[variableName] = &currentEvent->values_float_vector[branchName][i];
    }
  } else if (branchType == "UChar_t") {
    inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_uchar_vector[branchName]);
    for (int i = 0; i < maxCollectionElements; i++) {
      currentEvent->collections[collectionName]->at(i)->values_uchar[variableName] = &currentEvent->values_uchar_vector[branchName][i];
    }
  } else if (branchType == "Int_t") {
    inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_int_vector[branchName]);
    for (int i = 0; i < maxCollectionElements; i++) {
      currentEvent->collections[collectionName]->at(i)->values_int[variableName] = &currentEvent->values_int_vector[branchName][i];
    }
  } else if (branchType == "Bool_t") {
    inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_bool_vector[branchName]);
    for (int i = 0; i < maxCollectionElements; i++) {
      currentEvent->collections[collectionName]->at(i)->values_bool[variableName] = &currentEvent->values_bool_vector[branchName][i];
    }
  } else {
    cout << "ERROR -- unsupported branch type: " << branchType << "\t (branch name: " << branchName << ")" << endl;
  }
}

void EventReader::InitializeCollection(string collectionName) {
  if (currentEvent->collections.count(collectionName)) return;

  currentEvent->collections[collectionName] = make_shared<PhysicsObjects>();
  for (int i = 0; i < maxCollectionElements; i++) {
    currentEvent->collections[collectionName]->push_back(make_shared<PhysicsObject>());
  }
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
