

void plotComparison()
{
    auto input_file = TFile::Open("lhe_comparison.root");

    auto canvas = new TCanvas("canvas", "canvas", 2000, 2000);
    canvas->Divide(2, 3);

    canvas->cd(1);
    auto our_pt = (TH1D*)input_file->Get("our_alp_single_pt");
    auto their_pt = (TH1D*)input_file->Get("their_alp_single_pt");

    their_pt->SetLineColor(kRed);

    our_pt->Rebin(5);
    their_pt->Rebin(5);

    our_pt->DrawNormalized();
    their_pt->DrawNormalized("same");

    canvas->cd(2);
    auto our_pz = (TH1D*)input_file->Get("our_alp_single_pz");
    auto their_pz = (TH1D*)input_file->Get("their_alp_single_pz");

    their_pz->SetLineColor(kRed);

    our_pz->Rebin(5);
    their_pz->Rebin(5);

    our_pz->DrawNormalized();
    their_pz->DrawNormalized("same");

    canvas->cd(3);
    auto our_eta = (TH1D*)input_file->Get("our_alp_single_eta");
    auto their_eta = (TH1D*)input_file->Get("their_alp_single_eta");

    their_eta->SetLineColor(kRed);

    our_eta->Rebin(1);
    their_eta->Rebin(1);

    our_eta->DrawNormalized();
    their_eta->DrawNormalized("same");

    canvas->cd(4);
    auto our_phi = (TH1D*)input_file->Get("our_alp_single_phi");
    auto their_phi = (TH1D*)input_file->Get("their_alp_single_phi");

    their_phi->SetLineColor(kRed);

    our_phi->Rebin(1);
    their_phi->Rebin(1);

    our_phi->DrawNormalized();
    their_phi->DrawNormalized("same");



}