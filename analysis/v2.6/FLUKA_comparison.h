//**************************************************
// \file FLUKA_comparison.h
// \brief: Analysis #1 of ATLHECTB v2.6 
//         for pi-   
// \author: Michele D'Andrea (CERN EP-SFT-sim)
// \start date: 3 August 2023
//**************************************************

#ifndef FLUKA_comparison_h
#define FLUKA_comparison_h

void FLUKA_comparison(){

    //Initialize the plots
    cout << "Initializing plots for comparison..." << endl;

    auto outputfile = new TFile("FLUKA_comparison.root","RECREATE");
    outputfile->cd();

    //Energy resolution plot------------------------------------------
    //
    //#######---------Importing data from files-----------#######
    auto FLUKA_file = new TFile("ATLHECTBanalysispi_FLUKA.root");
    auto G4_file = new TFile("ATLHECTBanalysispi.root");
    
    //------FLUKA & RATIO---------
    auto FLUKA_EnergyRes = (TGraphErrors*)FLUKA_file->Get("piMinus_energyresolution");
    FLUKA_EnergyRes -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyRes -> SetLineColor(kCyan -3);
    auto FLUKA_EnergyRes_ratio = (TGraphErrors*)FLUKA_file->Get("piMinus_ATLASenergyresolutionRatio");
    FLUKA_EnergyRes_ratio -> SetMarkerColor(kCyan -3);
    FLUKA_EnergyRes_ratio -> SetLineColor(kCyan -3);

    //------G4.11.1.ref05 / ATLAS & RATIO---------
    auto G4_EnergyRes = (TGraphErrors*)G4_file->Get("piMinus_energyresolution");
    auto ATLAS_EnergyRes = (TGraphErrors*)G4_file->Get("piMinus_ATLASenergyresolution");
    auto G4_EnergyRes_ratio = (TGraphErrors*)G4_file->Get("piMinus_ATLASenergyresolutionRatio");
    G4_EnergyRes_ratio -> SetMarkerColor(kRed);
    G4_EnergyRes_ratio -> SetLineColor(kRed);

    outputfile->cd();

    //#######---------Plotting-----------#######

    auto C1res = new TCanvas("piMinus_Canvas_resolution", "", 700, 900);
    auto *p2res = new TPad("p2","p2",0.,0.02,1.,0.32); p2res->Draw();
    auto *p1res = new TPad("p1","p1",0.,0.3,1.,1.);  p1res->Draw();
    
    p1res->cd();
    ATLAS_EnergyRes->SetTitle("");
    ATLAS_EnergyRes->Draw("AP");
    G4_EnergyRes->Draw("same P");
    FLUKA_EnergyRes->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto Freslegend = new TLegend(1.-0.12,0.55,1.-0.51,0.89);
    Freslegend->AddEntry(ATLAS_EnergyRes,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-LARG-PUB-2022-001}}",
    "ep");
    Freslegend->AddEntry(G4_EnergyRes,
    "#splitline{ATLHECTB v2.6 }{#splitline{Geant4.11.1.ref05 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    Freslegend->AddEntry(FLUKA_EnergyRes,
    "#splitline{ATLHECTB v2.6 }{#splitline{GH}{w/ Birks Law}}",
    "ep");
    Freslegend->SetLineWidth(0);
    Freslegend->Draw("same");
    
    p2res->cd();
    gPad->SetLeftMargin(0.15);
    G4_EnergyRes_ratio->Draw("AP");
    FLUKA_EnergyRes_ratio->Draw("same P");

    C1res->Write();
    //C1res->SaveAs("plots/piMinus_EnergyRes_Comparison.pdf");
    delete p1res, p2res;
    delete G4_EnergyRes, ATLAS_EnergyRes, FLUKA_EnergyRes, G4_EnergyRes_ratio, FLUKA_EnergyRes_ratio;
    delete C1res;

    //Response plot------------------------------------------
    //
    //#######---------Importing data from files-----------#######

    //------FLUKA & RATIO---------
    auto FLUKA_Response = (TGraphErrors*)FLUKA_file->Get("piMinus_responses");
    FLUKA_Response -> SetMarkerColor(kCyan -3);
    FLUKA_Response -> SetLineColor(kCyan -3);
    auto FLUKA_Response_ratio = (TGraphErrors*)FLUKA_file->Get("piMinus_ATLASresponseRatio");
    FLUKA_Response_ratio -> SetMarkerColor(kCyan -3);
    FLUKA_Response_ratio -> SetLineColor(kCyan -3);

    //------G4.11.1.ref05 / ATLAS & RATIO---------
    auto G4_Response = (TGraphErrors*)G4_file->Get("piMinus_responses");
    auto ATLAS_Response = (TGraphErrors*)G4_file->Get("piMinus_ATLASresponse");
    auto G4_Response_ratio = (TGraphErrors*)G4_file->Get("piMinus_ATLASresponseRatio");
    G4_Response_ratio -> SetMarkerColor(kRed);
    G4_Response_ratio -> SetLineColor(kRed);
    
    outputfile->cd();

    //#######---------Plotting-----------#######

    auto C1resp = new TCanvas("piMinus_Canvas_response", "", 700, 900);
    auto *p2resp = new TPad("p2","p2",0.,0.02,1.,0.32); p2resp->Draw();
    auto *p1resp = new TPad("p1","p1",0.,0.3,1.,1.);  p1resp->Draw();
    
    p1resp->cd();
    ATLAS_Response->SetTitle("");
    ATLAS_Response->Draw("AP");
    G4_Response->Draw("same P");
    FLUKA_Response->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto Fresplegend = new TLegend(0.59,0.18,0.89,0.48);
    Fresplegend->AddEntry(ATLAS_Response,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-LARG-PUB-2022-001}}",
    "ep");
    Fresplegend->AddEntry(G4_Response,
    "#splitline{ATLHECTB v2.6 }{#splitline{Geant4.11.1.ref05 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    Fresplegend->AddEntry(FLUKA_Response,
    "#splitline{ATLHECTB v2.6 }{#splitline{GH}{w/ Birks Law}}",
    "ep");
    Fresplegend->SetLineWidth(0);
    Fresplegend->Draw("same");

    p2resp->cd();
    gPad->SetLeftMargin(0.15);
    G4_Response_ratio->Draw("AP");
    FLUKA_Response_ratio->Draw("same P");

    C1resp->Write();
    //C1resp->SaveAs("plots/piMinus_Response_Comparison.pdf");
    delete p1resp, p2resp;
    delete G4_Response, ATLAS_Response, FLUKA_Response, G4_Response_ratio, FLUKA_Response_ratio;
    delete C1resp;

    //L0 plot------------------------------------------
    //
    //#######---------Importing data from files-----------#######

    //------FLUKA & RATIO---------
    auto FLUKA_L0 = (TGraphErrors*)FLUKA_file->Get("piMinus_L0");
    FLUKA_L0 -> SetMarkerColor(kCyan -3);
    FLUKA_L0 -> SetLineColor(kCyan -3);
    auto FLUKA_L0_ratio = (TGraphErrors*)FLUKA_file->Get("piMinus_ATLASL0Ratio");
    FLUKA_L0_ratio -> SetMarkerColor(kCyan -3);
    FLUKA_L0_ratio -> SetLineColor(kCyan -3);

    //------G4.11.1.ref05 / ATLAS & RATIO---------
    //auto G4_L0_file = new TFile("ATLHECTBanalysispi.root");
    auto G4_L0 = (TGraphErrors*)G4_file->Get("piMinus_L0");
    auto ATLAS_L0 = (TGraphErrors*)G4_file->Get("piMinus_ATLASL0");
    auto G4_L0_ratio = (TGraphErrors*)G4_file->Get("piMinus_ATLASL0Ratio");
    G4_L0_ratio -> SetMarkerColor(kRed);
    G4_L0_ratio -> SetLineColor(kRed);

    outputfile->cd();

    //#######---------Plotting-----------#######

    auto C1L0 = new TCanvas("piMinus_Canvas_L0", "", 700, 900);
    auto *p2L0 = new TPad("p2","p2",0.,0.02,1.,0.32); p2L0->Draw();
    auto *p1L0 = new TPad("p1","p1",0.,0.3,1.,1.);  p1L0->Draw();

    p1L0->cd();
    ATLAS_L0->SetTitle("");
    ATLAS_L0->Draw("AP");
    G4_L0->Draw("same P");
    FLUKA_L0->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto FL0legend = new TLegend(0.59,0.18,0.89,0.48);
    FL0legend->AddEntry(ATLAS_L0,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-LARG-PUB-2022-001}}",
    "ep");
    FL0legend->AddEntry(G4_L0,
    "#splitline{ATLHECTB v2.6 }{#splitline{Geant4.11.1.ref05 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    FL0legend->AddEntry(FLUKA_L0,
    "#splitline{ATLHECTB v2.6 }{#splitline{GH}{w/ Birks Law}}",
    "ep");
    FL0legend->SetLineWidth(0);
    FL0legend->Draw("same");

    p2L0->cd();
    gPad->SetLeftMargin(0.15);
    G4_L0_ratio->Draw("AP");
    FLUKA_L0_ratio->Draw("same P");

    C1L0->Write();
    //C1L0->SaveAs("plots/piMinus_L0_Comparison.pdf");
    delete p1L0, p2L0;
    delete G4_L0, ATLAS_L0, FLUKA_L0, G4_L0_ratio, FLUKA_L0_ratio;
    delete C1L0;

    //sigmaL0 plot------------------------------------------
    //
    //#######---------Importing data from files-----------#######

    //------FLUKA & RATIO---------
    auto FLUKA_sigmaL0 = (TGraphErrors*)FLUKA_file->Get("piMinus_sigmaL0");
    FLUKA_sigmaL0 -> SetMarkerColor(kCyan -3);
    FLUKA_sigmaL0 -> SetLineColor(kCyan -3);
    auto FLUKA_sigmaL0_ratio = (TGraphErrors*)FLUKA_file->Get("piMinus_ATLASsigmaL0Ratio");
    FLUKA_sigmaL0_ratio -> SetMarkerColor(kCyan -3);
    FLUKA_sigmaL0_ratio -> SetLineColor(kCyan -3);

    //------G4.11.1.ref05 / ATLAS & RATIO---------
    //auto G4_sigmaL0_file = new TFile("ATLHECTBanalysispi.root");
    auto G4_sigmaL0 = (TGraphErrors*)G4_file->Get("piMinus_sigmaL0");
    auto ATLAS_sigmaL0 = (TGraphErrors*)G4_file->Get("piMinus_ATLASsigmaL0");
    auto G4_sigmaL0_ratio = (TGraphErrors*)G4_file->Get("piMinus_ATLASsigmaL0Ratio");
    G4_sigmaL0_ratio -> SetMarkerColor(kRed);
    G4_sigmaL0_ratio -> SetLineColor(kRed);

    outputfile->cd();

    //#######---------Plotting-----------#######

    auto C1sigmaL0 = new TCanvas("piMinus_Canvas_sigmaL0", "", 700, 900);
    auto *p2sigmaL0 = new TPad("p2","p2",0.,0.02,1.,0.32); p2sigmaL0->Draw();
    auto *p1sigmaL0 = new TPad("p1","p1",0.,0.3,1.,1.);  p1sigmaL0->Draw();

    p1sigmaL0->cd();
    ATLAS_sigmaL0->SetTitle("");
    ATLAS_sigmaL0->Draw("AP");
    G4_sigmaL0->Draw("same P");
    FLUKA_sigmaL0->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto FsigmaL0legend = new TLegend(0.59,0.18,0.89,0.48);
    FsigmaL0legend->AddEntry(ATLAS_sigmaL0,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-LARG-PUB-2022-001}}",
    "ep");
    FsigmaL0legend->AddEntry(G4_sigmaL0,
    "#splitline{ATLHECTB v2.6 }{#splitline{Geant4.11.1.ref05 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    FsigmaL0legend->AddEntry(FLUKA_sigmaL0,
    "#splitline{ATLHECTB v2.6 }{#splitline{GH}{w/ Birks Law}}",
    "ep");
    FsigmaL0legend->SetLineWidth(0);
    FsigmaL0legend->Draw("same");
    
    p2sigmaL0->cd();
    gPad->SetLeftMargin(0.15);
    G4_sigmaL0_ratio->Draw("AP");
    FLUKA_sigmaL0_ratio->Draw("same P");

    C1sigmaL0->Write();
    //C1sigmaL0->SaveAs("plots/piMinus_sigmaL0_Comparison.pdf");
    delete p1sigmaL0, p2sigmaL0;
    delete G4_sigmaL0, ATLAS_sigmaL0, FLUKA_sigmaL0, G4_sigmaL0_ratio, FLUKA_sigmaL0_ratio;
    delete C1sigmaL0;

    outputfile->Close();

}

#endif

//**************************************************
