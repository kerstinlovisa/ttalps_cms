string input_path_our = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/signals_lhe/our_signal.lhe";
string input_path_their = "/Users/jeremi/Documents/Physics/DESY/ttalps_cms.nosync/data/signals_lhe/ttPhi_2mu_PS_M-1.lhe";

string output_path = "lhe_comparison.root";

map<string, tuple<string, int, double, double>> histParams = {
// name                      title                nBins min     max
  {"our_alp_single_pt"  , make_tuple("top p_{T}"  , 1000, 0.0   , 1000.  )},
  {"our_alp_single_pz"  , make_tuple("top p_{z}"  , 1000, 0.0   , 1000. )},
  {"our_alp_single_eta" , make_tuple("top #eta"   , 100 , -10.0 , 10.0  )},
  {"our_alp_single_phi" , make_tuple("top #phi"   , 100 , -4    , 4     )},

  {"their_alp_single_pt"  , make_tuple("top p_{T}"  , 1000, 0.0   , 1000.  )},
  {"their_alp_single_pz"  , make_tuple("top p_{z}"  , 1000, 0.0   , 1000. )},
  {"their_alp_single_eta" , make_tuple("top #eta"   , 100 , -10.0 , 10.0  )},
  {"their_alp_single_phi" , make_tuple("top #phi"   , 100 , -4    , 4     )},
};

void ReadParticlesFromFile(string input_path, vector<TLorentzVector> &particles);

int nEvents = 0;

void compareLHEfiles()
{
  map<string, TH1D*> hists;
  
  for(auto &[name, paramsTuple] : histParams){
    auto &[title, nBins, min, max] = paramsTuple;
    hists[name] = new TH1D(name.c_str(), title.c_str(), nBins, min, max);
  }
  
  vector<TLorentzVector> our_alps;
  vector<TLorentzVector> their_alps;
  
  ReadParticlesFromFile(input_path_our, our_alps);
  ReadParticlesFromFile(input_path_their, their_alps);
  
  cout<<"N our particles: "<<our_alps.size()<<endl;
  cout<<"N their particles: "<<their_alps.size()<<endl;
  
  for(int i=0; i<our_alps.size(); i++){
    if(i%100000==0) cout<<"Processing particle "<<i<<endl;
    
    hists["our_alp_single_eta"]->Fill(our_alps[i].Eta());
    hists["our_alp_single_pt"]->Fill(our_alps[i].Pt());
    hists["our_alp_single_pz"]->Fill(our_alps[i].Pz());
    hists["our_alp_single_phi"]->Fill(our_alps[i].Phi());
  }

  for(int i=0; i<their_alps.size(); i++){
    if(i%100000==0) cout<<"Processing particle "<<i<<endl;
    
    hists["their_alp_single_eta"]->Fill(their_alps[i].Eta());
    hists["their_alp_single_pt"]->Fill(their_alps[i].Pt());
    hists["their_alp_single_pz"]->Fill(their_alps[i].Pz());
    hists["their_alp_single_phi"]->Fill(their_alps[i].Phi());
  }
  
  TFile *outFile = new TFile(output_path.c_str(),"recreate");
  outFile->cd();
  for(auto &[name, hist] : hists) hist->Write();
  outFile->Close();
  
  cout<<"nEvents:"<<nEvents<<endl;
}

void ReadParticlesFromFile(string input_path, vector<TLorentzVector> &particles)
{
  istringstream curstring;
  
  int nHighAco=0;
  cout<<"Processing file "<<input_path<<endl;
  ifstream lheFile(input_path.c_str());
  
  while(lheFile.good()){
    string line;
    getline(lheFile,line);
    
    curstring.clear();
    curstring.str(line);
    
    if(line.find("<event>")!=string::npos){
      nEvents++;
      
      TLorentzVector particle(0.,0.,0.,0.);
      
      bool first=true;
      
      while(line.find("</event>")==string::npos && lheFile.good()){
        getline(lheFile,line);
        curstring.clear();
        curstring.str(line);
        
        double pdgID, useless, p1[4];
        
        curstring >> pdgID;
        for(int i=0;i<5;i++){curstring >> useless;}
        curstring >> p1[0] >> p1[1] >> p1[2] >> p1[3];
        for(int i=0;i<3;i++){curstring >> useless;}
        
        if(pdgID == 6 || pdgID == -6){
          particle.SetPxPyPzE(p1[0],p1[1],p1[2],p1[3]);
          particles.push_back(particle);
        }
      }
    }
  }
  
  lheFile.close();
}
