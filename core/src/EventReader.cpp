//  EventReader.cpp
//
//  Created by Jeremi Niedziela on 04/08/2023.

#include "Helpers.hpp"
#include "EventReader.hpp"
// #include "Logger.hpp"

#include <TKey.h>

using namespace std;

EventReader::EventReader(string inputPath, vector<string> outputPaths) : currentEvent(make_shared<Event>())
{
  SetupBranches(inputPath, outputPaths);
}

EventReader::~EventReader()
{
}

void EventReader::SetupBranches(string inputPath, vector<string> outputPaths)
{
  // Read trees from input files
  TFile *inFile = TFile::Open(inputPath.c_str());

  vector<string> treeNames = getListOfTrees(inFile);

  for (string treeName : treeNames)
  {
    cout << "Loading tree: " << treeName << endl;
    inputTrees[treeName] = (TTree *)inFile->Get(treeName.c_str());
  }

  // for (string outputPath : outputPaths)
  //   SetupOutputTree(outputPath);

  auto keysInEventTree = inputTrees["Events"]->GetListOfBranches();

  for (auto i : *keysInEventTree)
  {
    auto branch = (TBranch *)i;
    string branchName = branch->GetName();
    string branchType = branch->FindLeaf(branchName.c_str())->GetTypeName();

    if (branchType == "")
      cout << "ERROR -- couldn't find branch type for branch: " << branchName << endl;

    if (branchName.find("_") == std::string::npos || branchName.find("HLT") == 0 || branchName.find("Flag") == 0 || branchName.find("L1") == 0 || branchName.find("PV_") == 0 || branchName.find("MET_") == 0 || branchName.find("GenMET_") == 0 || branchName.find("CaloMET") == 0 || branchName.find("ChsMET") == 0 || branchName.find("DeepMETResolutionTune") == 0 || branchName.find("DeepMETResponseTune") == 0 || branchName.find("GenVtx") == 0 || branchName.find("Generator") == 0 || branchName.find("HTXS") == 0)
    { // it's a single number
      currentEvent->values_types[branchName] = branchType;

      if (branchType == "UInt_t")
      {
        currentEvent->values_uint[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_uint[branchName]);
      }
      else if (branchType == "Int_t")
      {
        currentEvent->values_int[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_int[branchName]);
      }
      else if (branchType == "Bool_t")
      {
        currentEvent->values_bool[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_bool[branchName]);
      }
      else if (branchType == "Float_t")
      {
        currentEvent->values_float[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_float[branchName]);
      }
      else if (branchType == "ULong64_t")
      {
        currentEvent->values_ulong[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_ulong[branchName]);
      }
      else if (branchType == "UChar_t")
      {
        currentEvent->values_uchar[branchName] = 0;
        inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_uchar[branchName]);
      }
      else
      {
        cout << "ERROR -- unsupported branch type: " << branchType << "\t (branch name: " << branchName << ")" << endl;
      }
    }
    else
    { // it's a collection
      string::size_type pos = branchName.find('_');
      string collectionName = branchName.substr(0, pos);
      string variableName = branchName.substr(pos + 1);

      if (!currentEvent->collections.count(collectionName))
      {
        currentEvent->collections[collectionName] = make_shared<PhysicsObjects>();

        for (int i = 0; i < maxCollectionElements; i++)
        {
          currentEvent->collections[collectionName]->push_back(make_shared<PhysicsObject>());
        }
      }

      for (int i = 0; i < maxCollectionElements; i++)
      {
        currentEvent->collections[collectionName]->at(i)->values_types[variableName] = branchType;
      }

      if (branchType == "Float_t")
      {
        inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_float_vector[branchName]);
        for (int i = 0; i < maxCollectionElements; i++)
        {
          currentEvent->collections[collectionName]->at(i)->values_float[variableName] = &currentEvent->values_float_vector[branchName][i];
        }
      }
      else if (branchType == "UChar_t")
      {
        inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_uchar_vector[branchName]);
        for (int i = 0; i < maxCollectionElements; i++)
        {
          currentEvent->collections[collectionName]->at(i)->values_uchar[variableName] = &currentEvent->values_uchar_vector[branchName][i];
        }
      }
      else if (branchType == "Int_t")
      {
        inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_int_vector[branchName]);
        for (int i = 0; i < maxCollectionElements; i++)
        {
          currentEvent->collections[collectionName]->at(i)->values_int[variableName] = &currentEvent->values_int_vector[branchName][i];
        }
      }
      else if (branchType == "Bool_t")
      {
        inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_bool_vector[branchName]);
        for (int i = 0; i < maxCollectionElements; i++)
        {
          currentEvent->collections[collectionName]->at(i)->values_bool[variableName] = &currentEvent->values_bool_vector[branchName][i];
        }
      }
      else
      {
        cout << "ERROR -- unsupported branch type: " << branchType << "\t (branch name: " << branchName << ")" << endl;
      }
    }
  }
}

void EventReader::SetupOutputTree(string outFileName)
{
  outFile[outFileName] = new TFile(outFileName.c_str(), "recreate");
  outFile[outFileName]->cd();

  dirEvent[outFileName] = outFile[outFileName]->mkdir("ggHiNtuplizer");
  dirHLT[outFileName] = outFile[outFileName]->mkdir("hltanalysis");
  dirL1[outFileName] = outFile[outFileName]->mkdir("l1object");

  outEventTree[outFileName] = eventTree->CloneTree(0);
  outHltTree[outFileName] = hltTree->CloneTree(0);
  outL1Tree[outFileName] = l1Tree->CloneTree(0);

  outEventTree[outFileName]->Reset();
  outHltTree[outFileName]->Reset();
  outL1Tree[outFileName]->Reset();

  if (zdcTree)
  {
    dirZdc[outFileName] = outFile[outFileName]->mkdir("rechitanalyzerpp");
    outZdcTree[outFileName] = zdcTree->CloneTree(0);
    outZdcTree[outFileName]->Reset();
  }
  else
  {
    outZdcTree[outFileName] = nullptr;
    cout << "\n\nWARNING -- no ZDC tree found in the input file!\n\n"
         << endl;
  }

  if (pixelTree)
  {
    dirPixel[outFileName] = outFile[outFileName]->mkdir("pixelTracks");
    outPixelTree[outFileName] = pixelTree->CloneTree(0);
    outPixelTree[outFileName]->Reset();
  }
  else
  {
    cout << "\n\nWARNING -- no Pixel Tracks tree found in the input file!\n\n"
         << endl;
  }
}

void EventReader::AddEventToOutputTree(int iEvent, string outFileName, bool saveHLTtree)
{
  eventTree->GetEntry(iEvent);
  hltTree->GetEntry(iEvent);
  l1Tree->GetEntry(iEvent);
  if (zdcTree)
    zdcTree->GetEntry(iEvent);

  outEventTree[outFileName]->Fill();

  outHltTree[outFileName]->Fill();
  outL1Tree[outFileName]->Fill();
  outZdcTree[outFileName]->Fill();
}

void EventReader::SaveOutputTree(string outFileName)
{
  dirHLT[outFileName]->cd();
  outHltTree[outFileName]->Write();
  dirL1[outFileName]->cd();
  outL1Tree[outFileName]->Write();
  dirEvent[outFileName]->cd();
  outEventTree[outFileName]->Write();

  if (outZdcTree[outFileName])
  {
    dirZdc[outFileName]->cd();
    outZdcTree[outFileName]->Write();
  }

  if (pixelTree)
  {
    dirPixel[outFileName]->cd();
    outPixelTree[outFileName]->Write();
  }

  outFile[outFileName]->Close();
}

shared_ptr<Event> EventReader::GetEvent(int iEvent)
{
  // Move to desired entry in all trees
  for (auto &[name, tree] : inputTrees)
    tree->GetEntry(iEvent);

  // for (auto &[name, collection] : currentEvent->collections)
  // {
  //   UInt_t collectionSize = currentEvent->GetUint("n" + name);
  //   collection->resize(collectionSize);
  // }
  return currentEvent;
}
