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

    // cout << "branch: " << branchName << "\ttype: " << branchType << endl;

    if (branchType == "UInt_t")
    {
      values_uint[branchName] = 0;
      inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_uint[branchName]);
    }
    else if (branchType == "Float_t" && branchName.find("_") != std::string::npos)
    {
      string::size_type pos = branchName.find('_');
      string collectionName = branchName.substr(0, pos);
      string variableName = branchName.substr(pos+1);

      

      inputTrees["Events"]->SetBranchAddress(branchName.c_str(), &currentEvent->values_float_vector[branchName]);

      for(int i=0; i<maxCollectionElements; i++) {
        currentEvent->collections[collectionName]->at(i)->values_float[variableName] = &currentEvent->values_float_vector[branchName][i]; // muons
      }

    }
    else{
      cout<<"ERROR -- unsupported branch type: "<<branchType<<"\t (branch name: "<<branchName<<")"<<endl;
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
  cout << "entering get event" << endl;
  for (auto &[name, tree] : inputTrees)
    tree->GetEntry(iEvent);

  return currentEvent;
}
