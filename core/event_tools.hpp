#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>

#include "Event.hpp"

using namespace std;

enum StatusFlagMask {
    isPrompt    = 1 << 0,
    isDecayedLeptonHadron = 1 << 1,
    isTauDecayProduct    = 1 << 2,
    isPromptTauDecayProduct     = 1 << 3,
    isDirectTauDecayProduct    = 1 << 4,
    isDirectPromptTauDecayProduct    = 1 << 5,
    isDirectHadronDecayProduct    = 1 << 6,
    isHardProcess       = 1 << 7,
    fromHardProcess       = 1 << 8,
    isHardProcessTauDecayProduct       = 1 << 9,
    isDirectHardProcessTauDecayProduct       = 1 << 10,
    fromHardProcessBeforeFSR       = 1 << 11,
    isFirstCopy       = 1 << 12,
    isLastCopy       = 1 << 13,
    isLastCopyBeforeFSR       = 1 << 14
};

bool is_last_copy(int status_flag){
    return (status_flag & isLastCopy);
}

bool is_first_copy(int status_flag){
    return (status_flag & isFirstCopy);
}

bool has_top_mother(int index, vector<int> top_indices, const shared_ptr<Event> event)
{
    auto genParticles = event->GetCollection("GenPart");
    int mother_index = genParticles->at(index)->Get("genPartIdxMother");

    if(mother_index == top_indices[0] || mother_index == top_indices[1]) return true;
    if(mother_index < 0 ) return false;

    return has_top_mother(mother_index, top_indices, event);
}

bool has_b_mother(int index, vector<int> b_indices, const shared_ptr<Event> event, bool verbose = false)
{
    auto genParticles = event->GetCollection("GenPart");
    int mother_index = genParticles->at(index)->Get("genPartIdxMother");
    int mother_pid = genParticles->at(index)->Get("pdgId");

    if(verbose) cout<<"mother index: "<<mother_index<<"("<<mother_pid<<")"<<endl;

    if(find(b_indices.begin(), b_indices.end(), mother_index) != b_indices.end()) return true;
    if(mother_index < 0 ) return false;

    return has_b_mother(mother_index, b_indices, event, verbose);
}

bool has_ISR_mother_after_top_mother(int index, const shared_ptr<Event> event)
{
    auto genParticles = event->GetCollection("GenPart");
    int mother_index = genParticles->at(index)->Get("genPartIdxMother");
    int mother_pid = genParticles->at(index)->Get("pdgId");

    if(abs(mother_pid) == 6) return false;
    if(mother_pid == 21 || (abs(mother_pid) >= 1 && abs(mother_pid) <= 5)) return true;
    if(mother_index < 0 ) return false;
    
    return has_ISR_mother_after_top_mother(mother_index, event);
}

struct FinalState
{
    int n_neutrinos = 0;
    int n_electrons = 0;
    int n_muons = 0;
    int n_taus = 0;
    int n_udsc_jets = 0;
    int n_b_jets = 0;

    void print(){
        if(n_neutrinos != 0) cout<<n_neutrinos<<" nu, ";
        if(n_electrons != 0) cout<<n_electrons<<" e, ";
        if(n_muons != 0) cout<<n_muons<<" mu, ";
        if(n_taus != 0) cout<<n_taus<<" tau, ";
        if(n_udsc_jets != 0) cout<<n_udsc_jets<<" udsc jets, ";
        if(n_b_jets != 0) cout<<n_b_jets<<" b jets";
        cout<<endl;
    }

    bool is_consistent(){
        if(n_b_jets != 2) return false;
        int n_leptons = n_electrons + n_muons + n_taus;

        if(n_leptons == 2 && n_neutrinos == 2) return true; // fully leptonic
        if(n_leptons == 1 && n_neutrinos == 1 && n_udsc_jets == 2) return true; // semi-leptonic
        if(n_udsc_jets == 4) return true; // fully hadronic

        return false;
    }

    string get_short_name(){
        if(n_udsc_jets == 4) return "hh";
        if(n_electrons == 2) return "ee";
        if(n_muons == 2) return "mumu";
        if(n_taus == 2) return "tautau";
        
        string name = "";
        if(n_udsc_jets == 2) name += "h";
        if(n_electrons == 1) name += "e";
        if(n_muons == 1) name += "mu";
        if(n_taus == 1) name += "tau";
        
        return name;
    }
};


vector<int> get_top_indices(const shared_ptr<Event> event){

    vector<int> top_indices = {-1, -1};

    uint nGenPart = event->Get("nGenPart");
    auto genParticles = event->GetCollection("GenPart");
    
    for(int i_gen_particle=0; i_gen_particle<nGenPart; i_gen_particle++){
        int genParticles_pdgId = genParticles->at(i_gen_particle)->Get("pdgId");
        int genParticles_statusFlag = genParticles->at(i_gen_particle)->Get("statusFlags");

        if(genParticles_pdgId == 6 && is_last_copy(genParticles_statusFlag)) top_indices[0] = i_gen_particle;
        if(genParticles_pdgId == -6 && is_last_copy(genParticles_statusFlag)) top_indices[1] = i_gen_particle;
        if(top_indices[0] >= 0 && top_indices[1] >= 0) break;
    }
    return top_indices;
}


bool is_good_b(int i_gen_particle, const shared_ptr<Event> event)
{
    auto genParticles = event->GetCollection("GenPart");
    int GenPart_statusFlag = genParticles->at(i_gen_particle)->Get("statusFlags");
    if(!is_first_copy(GenPart_statusFlag)) return false;
    int mother_index = genParticles->at(i_gen_particle)->Get("genPartIdxMother");;
    if(mother_index < 0) return false;
    int mother_pid = genParticles->at(mother_index)->Get("pdgId");
    return abs(mother_pid) == 6; // mother must be a top
}

bool is_good_udsc(int i_gen_particle, const shared_ptr<Event> event)
{
    auto genParticles = event->GetCollection("GenPart");
    int GenPart_statusFlag = genParticles->at(i_gen_particle)->Get("statusFlags");
    if(!is_first_copy(GenPart_statusFlag)) return false;
    int mother_index = genParticles->at(i_gen_particle)->Get("genPartIdxMother");
    if(mother_index < 0) return false;
    int mother_pid = genParticles->at(mother_index)->Get("pdgId");
    return abs(mother_pid) == 24; // mother must be a W
}

bool is_good_lepton(int i_gen_particle, const shared_ptr<Event> event, vector<int> top_indices, vector<int> b_indices)
{
    auto genParticles = event->GetCollection("GenPart");
    int GenPart_statusFlag = genParticles->at(i_gen_particle)->Get("statusFlags");
    if(!is_first_copy(GenPart_statusFlag)) return false;
    if(!has_top_mother(i_gen_particle, top_indices, event)) return false; // top must be in the mothers

    int mother_index = genParticles->at(i_gen_particle)->Get("genPartIdxMother");
    if(mother_index < 0) return false;
    int mother_pid = genParticles->at(mother_index)->Get("pdgId");
    int mother_statusFlag = genParticles->at(mother_index)->Get("statusFlags");

    if(!is_last_copy(mother_statusFlag)) return false; // we don't want leptons from some intermediate W's
    if(abs(mother_pid) != 24) return false; // mother must be a W
    if(has_b_mother(i_gen_particle, b_indices, event)) return false; // we don't want leptons coming from b decays
    if(has_ISR_mother_after_top_mother(i_gen_particle, event)) return false; // no gluons/jets after the top

    return true;
}


string get_short_name(const shared_ptr<Event> event)
{
    vector<int> top_indices = get_top_indices(event);

    if(top_indices[0] < 0 || top_indices[1] < 0){
        cout<<"ERROR -- couldn't find tt̄ pair in the event..."<<endl;
        return "error";
    }

    auto final_state = FinalState();
    
    vector<int> b_indices;

    uint nGenPart = event->Get("nGenPart");
    auto genParticles = event->GetCollection("GenPart");

    for(int i_gen_particle=0; i_gen_particle<nGenPart; i_gen_particle++){        
        int GenPart_pdgId = genParticles->at(i_gen_particle)->Get("pdgId");
        if(abs(GenPart_pdgId) == 5){
            if(!is_good_b(i_gen_particle, event)) continue;
            b_indices.push_back(i_gen_particle);
        }
    }

    for(int i_gen_particle=0; i_gen_particle<nGenPart; i_gen_particle++){

        int pid = genParticles->at(i_gen_particle)->Get("pdgId");
        // int mother_idx = genParticles->at(i_gen_particle)->Get("genPartIdxMother");
        // if(mother_idx < 0) cout << "mother_index: " << mother_idx << ",  particle id: " << pid << endl;
        
        if(abs(pid) == 5){ // b quark
            if(!is_good_b(i_gen_particle, event)) continue;
        }
        else if(abs(pid) >= 1 && abs(pid) <= 4){ // light quark
            if(!is_good_udsc(i_gen_particle, event)) continue;
        }
        else{ // leptons
            if(!is_good_lepton(i_gen_particle, event, top_indices, b_indices)) continue;
        }

        if(abs(pid) == 12 || abs(pid) == 14 || abs(pid) == 16) final_state.n_neutrinos++;
        if(abs(pid) == 11) final_state.n_electrons++;
        if(abs(pid) == 13) final_state.n_muons++;
        if(abs(pid) == 15) final_state.n_taus++;
        if(abs(pid) >= 0 && abs(pid) <= 4) final_state.n_udsc_jets++;
        if(abs(pid) == 5) final_state.n_b_jets++;
    }

    // final_state.print();
    if(!final_state.is_consistent()){
        cout<<"ERROR -- final state is not consistent with tt̄ event!"<<endl;
        final_state.print();
        return "error";
    }
    return final_state.get_short_name();
}