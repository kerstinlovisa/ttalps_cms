#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"

using namespace std;
using namespace ROOT;

string input_file_path = "../data/svj_tchannel_test_pfnanoaod.root";
string output_file_path = "../data/output_rdataframe_cpp.root";

int main()
{
    ROOT::RDataFrame d("Events", input_file_path.c_str());

    auto dSmall = d.Filter("nFatJet==2");
    auto dSmall_2 = dSmall.Filter("FatJet_pt[0]>400 || FatJet_pt[1]>400");
    
    auto dd = dSmall_2.Snapshot("Events", output_file_path.c_str());
}