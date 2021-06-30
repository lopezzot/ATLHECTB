//**************************************************
// \file ATLHECTBanalysis1_v1p1.cc
// \brief: Analysis #1 of ATLHECTB data from v1.1 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 14 June 2021
//**************************************************

#include <string>
#include <array>

void emanalysis( const vector<double>& emenergies, const vector<string>& emfiles ){

    //Initiate objects for the entire analysis
    //
    cout<<"ATLHECTB analysis of e- runs"<<endl;
    auto outputfile = new TFile( "ATLHECTBanalysise-.root", "RECREATE" );
    double energies[emenergies.size()];
    double ratiomaxtotS[emenergies.size()];
    double responses[emenergies.size()];
    double erresponses[emenergies.size()];
    double zeros[emenergies.size()];
    memset( zeros, 0., emenergies.size()*sizeof(double));
    double Sampfraction[emenergies.size()];
    double ersampfraction[emenergies.size()];
    double recenergies[emenergies.size()];
    double errecenergies[emenergies.size()];
    double energyresolution[emenergies.size()];
    double erenergyresolution[emenergies.size()];

    //For loop over Runs (energies)
    //
    for (unsigned RunNo = 0; RunNo<emfiles.size(); RunNo++ ){
        cout<<"---> Analysis run # "<<RunNo<<", energy(GeV) "<<emenergies[RunNo]<<endl;  
        //Initiate objects through single Run
        //   
        string filename = "Data1/"+emfiles[RunNo];
        double energy = emenergies[RunNo];
        TFile* file = TFile::Open( filename.c_str(), "READ" );
        TTree* tree = (TTree*)file->Get( "ATLHECTBout" );
    
        int pdg; tree->SetBranchAddress( "PDGID", &pdg );
        double venergy; tree->SetBranchAddress( "vertexkenergy", &venergy );
        double lenergy; tree->SetBranchAddress( "eleakage", &lenergy );
        double cenergy; tree->SetBranchAddress( "ecryostat", &cenergy );
        double edep; tree->SetBranchAddress( "edep", &edep );
        double elAr; tree->SetBranchAddress( "elAr", &elAr );
        double BelAr; tree->SetBranchAddress( "BirkelAr", &BelAr );
        vector<double>* M1L1BelAr = NULL; 
        tree->SetBranchAddress( "M1L1BirkeLayer", &M1L1BelAr );
        vector<double>* M1L2BelAr = NULL; 
        tree->SetBranchAddress( "M1L2BirkeLayer", &M1L2BelAr );
        vector<double>* M1L3BelAr = NULL; 
        tree->SetBranchAddress( "M1L3BirkeLayer", &M1L3BelAr );
        vector<double>* M1L4BelAr = NULL; 
        tree->SetBranchAddress( "M1L4BirkeLayer", &M1L4BelAr );
        vector<double>* M2L1BelAr = NULL; 
        tree->SetBranchAddress( "M2L1BirkeLayer", &M2L1BelAr );
        vector<double>* M2L2BelAr = NULL; 
        tree->SetBranchAddress( "M2L2BirkeLayer", &M2L2BelAr );
        vector<double>* M2L3BelAr = NULL; 
        tree->SetBranchAddress( "M2L3BirkeLayer", &M2L3BelAr );
        vector<double>* M2L4BelAr = NULL; 
        tree->SetBranchAddress( "M2L4BirkeLayer", &M2L4BelAr );
        vector<double>* M3L1BelAr = NULL; 
        tree->SetBranchAddress( "M3L1BirkeLayer", &M3L1BelAr );
        vector<double>* M3L2BelAr = NULL; 
        tree->SetBranchAddress( "M3L2BirkeLayer", &M3L2BelAr );
        vector<double>* M3L3BelAr = NULL; 
        tree->SetBranchAddress( "M3L3BirkeLayer", &M3L3BelAr );
        vector<double>* M3L4BelAr = NULL; 
        tree->SetBranchAddress( "M3L4BirkeLayer", &M3L4BelAr );
        
        int nBins = 100;
        //int MaxSignalIndex;

        //total leak (world+cryostat) vs. vis energy deposited
        //
        auto H2LeakvsEdep =
            new TH2F("e-LeakVsEdep", "e-LeakVsEdep",
                    nBins*10, 0., emenergies[RunNo]/20,
                    nBins*10, 0., emenergies[RunNo]*1.1 );
        //total leak (world+crostat)
        //
        auto H1Leak = new TH1F("e-ELeak", "e-ELeak",
                              nBins*10, 0., emenergies[RunNo]/20 );
        //total signal no cuts
        //
        auto H1TotS = new TH1F("e-TotS", "e-TotS",
                              nBins*20, 0., emenergies[RunNo]*100 );
        //total vis energy deposited
        //
        auto H1Econt = new TH1F("e-Econt", "e-Econt", 
                              nBins*10, 0., emenergies[RunNo]*2 );
        //total energy (vis+leak)
        //
        auto H1Etot = new TH1F("e-Etot", "e-Etot",
                               nBins*10, 0., emenergies[RunNo]*2 );
        //number of channels over cut
        //
        auto H1Channels = new TH1I("e-Channels", "e-Channels",
                                   88, 0, 88 ); 
        //disribution of signals per channel over cut
        //
        auto H1Signals = new TH1F("e-Signals", "e-Signal",
                                   nBins*2, 0., 5000. );
        //sum of signals over cut
        //
        auto H1TotCutSignal = new TH1F("e-CutTotS", "e-CutTotS",
                                      nBins*20, 0., emenergies[RunNo]*100 ); 
        //response
        //
        auto H1Response = new TH1F("e-Response","e-Response",
                                   nBins, 30, 70. );

        //reconstructed energy
        //
        auto H1Recenergy = new TH1F("e-Reconstructedenergy",
                "e-Reconstructedenergy", nBins*10, 0., 200. );

        //sampling fraction
        //
        auto H1Sampfraction = new TH1F("e-SamplingFraction",
                "e-SamplingFraction", nBins, 0., 10.);

        //auto H1MaxS = new TH1F("e-MaxS", "e-MaxS", 
                             // nBins, 0., emenergies[RunNo]*100. );

        //For loop over events
        //
        for ( unsigned int eventNo = 0; eventNo<tree->GetEntries(); eventNo++ ){
            tree->GetEntry(eventNo);

            H2LeakvsEdep->Fill(lenergy/1000.+cenergy/1000.,edep/1000.);
            H1Leak->Fill((lenergy+cenergy)/1000.);
            H1TotS->Fill(BelAr);
            H1Econt->Fill(emenergies[RunNo]-lenergy/1000.-cenergy/1000.);
            H1Etot->Fill( (edep+lenergy+cenergy)/1000. );
            H1Sampfraction->Fill((elAr/edep)*100.); //percent value

            //MaxSignalIndex = std::max_element(M2L1BelAr->begin(),M2L1BelAr->end())
            //                                -M2L1BelAr->begin();
            
            double addchannels=0;
            int channels = 0;

            for (unsigned int i = 0; i<M2L1BelAr->size(); i++){
                if ( i==2 || i==3 || i==4 || i==5 ) { 
                    channels += 1;
                    addchannels += M2L1BelAr->at(i);
                    H1Signals->Fill( M2L1BelAr->at(i)) ;
                }
            }
            for (unsigned int i = 0; i<M2L2BelAr->size(); i++){
                if ( i== 4 || i== 5 || i==6 ) { 
                    channels += 1;
                    addchannels+= M2L2BelAr->at(i);
                    H1Signals->Fill( M2L2BelAr->at(i)); 
                }
            }
            
            //H1MaxS->Fill(M2L1BelAr->at(MaxSignalIndex));
            H1TotCutSignal->Fill( addchannels );
            H1Channels->Fill(channels);
            H1Response->Fill( addchannels / (edep/1000.) ); 
            // average response 
            H1Recenergy->Fill( addchannels / 44.7987 ) ; 
        } //end for loop events

        energies[RunNo] = emenergies[RunNo];
        //ratiomaxtotS[RunNo] = 0;//H1MaxS->GetMean() / H1TotS->GetMean();
        responses[RunNo] = H1Response->GetMean();
        erresponses[RunNo] = H1Response->GetMeanError();
        Sampfraction[RunNo] = H1Sampfraction->GetMean();
        ersampfraction[RunNo] = H1Sampfraction->GetMeanError();

        double xfitmin = H1Recenergy->GetXaxis()->
            GetBinCenter(H1Recenergy->GetMaximumBin())-2.*H1Recenergy->GetStdDev();
        double xfitmax = H1Recenergy->GetXaxis()->
            GetBinCenter(H1Recenergy->GetMaximumBin())+2.*H1Recenergy->GetStdDev();
        auto F1Recenergy = new TF1("gaus","gaus(0)",xfitmin,xfitmax);
        H1Recenergy->Fit( F1Recenergy ,"QR");
        recenergies[RunNo] = H1Recenergy->GetFunction("gaus")->GetParameter(1);
        errecenergies[RunNo] = H1Recenergy->GetFunction("gaus")->GetParError(1);
        double res =(H1Recenergy->GetFunction("gaus")->GetParameter(2)/H1Recenergy->
                    GetFunction("gaus")->GetParameter(1))*sqrt(energies[RunNo])*100.;
        energyresolution[RunNo] = res;
        erenergyresolution[RunNo] = (H1Recenergy->GetFunction("gaus")->
                        GetParError(2)/H1Recenergy->GetFunction("gaus")->
                        GetParameter(2) + H1Recenergy->GetFunction("gaus")->
                        GetParError(1)/H1Recenergy->
                        GetFunction("gaus")->GetParameter(1))*res;

        outputfile->cd();
        H1Leak->Write();
        delete H1Leak;
        H2LeakvsEdep->Write();
        if ( RunNo == 6 ){
            auto C1LeakvsEdep = new TCanvas("e-Canvas_LeakvsEdep", "", 600, 600);
            //gPad->SetLeftMargin(0.15);
            H2LeakvsEdep->SetMarkerSize(0.5);
            H2LeakvsEdep->SetMarkerStyle(8);
            H2LeakvsEdep->GetXaxis()->SetTitle("E_{Leak} [GeV]");
            H2LeakvsEdep->GetYaxis()->SetTitle("E_{Vis} [GeV]");
            H2LeakvsEdep->GetYaxis()->SetRangeUser(100.,162.);
            H2LeakvsEdep->Draw("P");
            auto LeakvsEdeplegend = new TLegend(0.14,0.2,0.6,0.29);
            gPad->SetLeftMargin(0.13);
            LeakvsEdeplegend->SetLineWidth(0);
            LeakvsEdeplegend->SetHeader("Beam: e- 147.8 GeV", "C");
            LeakvsEdeplegend->AddEntry(H2LeakvsEdep,
                    "#splitline{ATLHECTB v1.1 }{Geant4.10.7.p01 FTFP_BERT }","ep");
            LeakvsEdeplegend->Draw("same");
            C1LeakvsEdep->Write();
            delete C1LeakvsEdep;
            delete LeakvsEdeplegend;
        }
        delete H2LeakvsEdep; 
        H1Econt->Write();
        delete H1Econt;
        H1TotS->Write();
        delete H1TotS;
        H1Etot->Write();
        delete H1Etot;
        H1Sampfraction->Write();
        delete H1Sampfraction;
       
        H1TotCutSignal->Write();
        H1Channels->Write();
        H1Signals->Write();
        H1Response->Write();
        H1Recenergy->Write();
       // H1MaxS->Write();
       // delete H1MaxS;
    }

    // Finalize objects over multiple runs
    //
    outputfile->cd();

    //auto G1ratiomaxtotS = new TGraph( emenergies.size(), energies, ratiomaxtotS ); 
    //G1ratiomaxtotS->SetName("e-ratiomaxtotS");
    //G1ratiomaxtotS->SetTitle("e-ratiomaxtotS");
    //G1ratiomaxtotS->Write();
    //delete G1ratiomaxtotS;

    auto G1responses = new TGraphErrors( emenergies.size(), energies, responses, 
                                         zeros, erresponses );
    G1responses->GetYaxis()->SetRangeUser(44.2,45.8);
    G1responses->SetMarkerStyle(8); 
    G1responses->SetName("e-responses");
    G1responses->SetTitle("e-responses");
    G1responses->GetYaxis()->SetTitle("Response [a.u./GeV]");
    G1responses->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1responses->Write();
    delete G1responses;

    auto G1Sampfraction = new TGraphErrors( emenergies.size(), energies, Sampfraction,
                                            zeros, ersampfraction );
    G1Sampfraction->GetYaxis()->SetRangeUser(4.5,4.53);
    G1Sampfraction->SetName("e-samplingFraction");
    G1Sampfraction->SetTitle("e-samplingFraction");
    G1Sampfraction->GetYaxis()->SetTitle("f_{samp} [%]");
    G1Sampfraction->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1Sampfraction->SetMarkerStyle(8);
    G1Sampfraction->Write();
    auto C1Sampfraction = new TCanvas("e-Canvas_Sampfraction", "", 600, 600);
    gPad->SetLeftMargin(0.15);
    G1Sampfraction->Draw("AP");
    auto Sampfractionlegend = new TLegend(0.2,0.8,0.6,0.89);
    Sampfractionlegend->SetLineWidth(0);
    //Sampfractionlegend->SetHeader("Sampling fraction e-", "C");
    Sampfractionlegend->AddEntry(G1Sampfraction,
            "#splitline{ATLHECTB v1.1 }{Geant4.10.7.p01 FTFP_BERT }","ep");
    Sampfractionlegend->Draw("same");
    C1Sampfraction->Write();
    delete C1Sampfraction;
    delete G1Sampfraction;

    auto G1recenergy = new TGraphErrors( emenergies.size(), energies, recenergies,
                                         zeros, errecenergies );
    G1recenergy->SetMarkerStyle(8);
    G1recenergy->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1recenergy->GetYaxis()->SetTitle("Energy [GeV]");
    G1recenergy->SetTitle("e-reconstructedenergy");
    G1recenergy->SetName("e-reconstructedenergy");
    G1recenergy->Write();
    delete G1recenergy;

    auto G1energyresolution = new TGraphErrors( emenergies.size(), energies, 
                                energyresolution, zeros, erenergyresolution );
    G1energyresolution->SetMarkerStyle(8);
    G1energyresolution->SetMarkerColor(kRed);
    G1energyresolution->SetLineColor(kRed);
    G1energyresolution->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1energyresolution->GetYaxis()->SetTitle(
            "#sigma_{0}/E_{0} #sqrt{E_{Beam}} [% #sqrt{GeV}]");
    G1energyresolution->GetYaxis()->SetRangeUser(19.,25.);
    G1energyresolution->GetXaxis()->SetRangeUser(0.,170.);
    G1energyresolution->SetTitle("e-energyresolution");
    G1energyresolution->SetName("e-energyresolution");
    auto F1energyres = new TF1("e-energyres","[0]",20.,150.);
    F1energyres->SetLineWidth(3);
    F1energyres->SetLineColor(kRed);
    G1energyresolution->Fit(F1energyres, "QR");
    cout<<"->Average a term in resolution "<<F1energyres->GetParameter(0)<<
        " +- "<<F1energyres->GetParError(0)<<" % GeV^0.5"<<endl;
    F1energyres->Write();
    G1energyresolution->Write();

    double ATLASenres[7] = {22.2, 21.6, 21.5, 22.30, 22.0, 21.2, 21.3};
    double erATLASenres[7] = {22.57-22.2, 21.89-21.6, 21.78-21.5, 22.6-22.30,
                              22.32-22.0, 21.46-21.2, 21.57-21.3};
    auto G1ATLASenres = new TGraphErrors( emenergies.size(), energies, 
            ATLASenres, zeros, erATLASenres );
    G1ATLASenres->SetMarkerStyle(8);
    G1ATLASenres->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1ATLASenres->GetYaxis()->SetTitle(
            "#sigma_{0}/E_{0} #sqrt{E_{Beam}} [% #sqrt{GeV}]");
    G1ATLASenres->GetYaxis()->SetRangeUser(19.,25.);
    G1ATLASenres->GetXaxis()->SetRangeUser(0.,170.);
    G1ATLASenres->SetTitle("");
    G1ATLASenres->SetName("e-ATLASenergyresolution");
    auto F1ATLASenergyres = new TF1("e-ATLASenergyres","21.68",20.,150.);
    F1ATLASenergyres->SetLineWidth(3);
    F1ATLASenergyres->SetLineColor(kBlack);
    F1ATLASenergyres->Write();
    G1ATLASenres->Write();

    //Create canvas e- energy resolution comparison
    //
    auto C1eneres = new TCanvas("e-Canvas_eneres", "", 700, 900);
    auto *p2 = new TPad("p2","p2",0.,0.02,1.,0.32); p2->Draw();
    auto *p1 = new TPad("p1","p1",0.,0.3,1.,1.);  p1->Draw();
    p1->cd();
    gPad->SetLeftMargin(0.15);
    G1ATLASenres->Draw("AP");
    G1energyresolution->Draw("P SAME");
    F1ATLASenergyres->Draw("L SAME");
    auto legend = new TLegend(0.15,0.7,0.58,0.89);
    legend->AddEntry(G1ATLASenres,
        "#splitline{ATLAS HEC }{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}", "ep");
    legend->AddEntry(G1energyresolution,
            "#splitline{ATLHECTB v1.1 }{Geant4.10.7.p01 FTFP_BERT }","ep");
    legend->SetLineWidth(0);
    legend->Draw("same");
    p2->cd();
    gPad->SetLeftMargin(0.15);
    TGraphErrors* ratio = new TGraphErrors(emenergies.size()); ratio->SetTitle("");
    for (unsigned int i=0; i<emenergies.size(); i++){
        ratio->SetPoint(i, emenergies[i], energyresolution[i]/ATLASenres[i]);
        ratio->SetPointError(i, 0.,
                (erenergyresolution[i]/energyresolution[i]+
                 erATLASenres[i]/ATLASenres[i])
                *energyresolution[i]/ATLASenres[i]);
    }
    ratio->GetYaxis()->SetTitle("MC/Data");
    ratio->SetMarkerStyle(8);
    ratio->GetYaxis()->SetLabelSize(0.09); ratio->GetXaxis()->SetLabelSize(0.09);
    ratio->GetYaxis()->SetTitleSize(0.09); ratio->GetYaxis()->SetTitleOffset(0.65);
    ratio->GetYaxis()->SetRangeUser(0.8,1.2);
    ratio->Draw("AP");
    C1eneres->Write(); 
    delete ratio; delete p1; delete p2;
    delete C1eneres;
    delete F1ATLASenergyres;
    delete F1energyres;
    delete G1energyresolution;
    delete G1ATLASenres;
    outputfile->Close();
    delete outputfile;

    // Final print out
    //
    double k;
    for (unsigned int i = 0; i<emenergies.size(); i++){
        k += responses[i];
    }
    cout<<"->Average response to e-: "<<k/double(emenergies.size())<<" a.u./GeV"<<endl;
    cout<<"ATLHECTB end of analysis of e- runs"<<endl;
};

////////////////////////////////////////////////////////////////////////////////////
void pianalysis( const vector<double>& pienergies, const vector<string>& emfiles ){

    //Initiate objects through all the analysis
    //
    cout<<"ATLHECTB analysis of pi- runs"<<endl;
    auto outputfile = new TFile( "ATLHECTBanalysispi.root", "RECREATE" );
    double energies[pienergies.size()];
    double responses[pienergies.size()];
    double erresponses[pienergies.size()];
    double zeros[pienergies.size()];
    memset( zeros, 0., pienergies.size()*sizeof(double));
    double recenergies[pienergies.size()];
    double errecenergies[pienergies.size()];
    double energyresolution[pienergies.size()];
    double erenergyresolution[pienergies.size()];
    double F1[pienergies.size()];
    double erF1[pienergies.size()];
    double F2[pienergies.size()];
    double erF2[pienergies.size()];
    double F3[pienergies.size()];
    double erF3[pienergies.size()];
    double F4[pienergies.size()];
    double erF4[pienergies.size()];
    double L0[pienergies.size()];
    double sigmaL0[pienergies.size()];
    
    //For loop over Runs (energies)
    //
    for (unsigned RunNo = 0; RunNo<emfiles.size(); RunNo++ ){
        cout<<"---> Analysis run # "<<RunNo<<", energy(GeV) "<<pienergies[RunNo]<<endl;  
        //Initiate objects through single Run
        //   
        string filename = "Data1/"+emfiles[RunNo];
        double energy = pienergies[RunNo];
        TFile* file = TFile::Open( filename.c_str(), "READ" );
        TTree* tree = (TTree*)file->Get( "ATLHECTBout" );
    
        double venergy; tree->SetBranchAddress( "vertexkenergy", &venergy );
        double lenergy; tree->SetBranchAddress( "eleakage", &lenergy );
        double cenergy; tree->SetBranchAddress( "ecryostat", &cenergy );
        double edep; tree->SetBranchAddress( "edep", &edep );
        double elAr; tree->SetBranchAddress( "elAr", &elAr );
        double BelAr; tree->SetBranchAddress( "BirkelAr", &BelAr );
        vector<double>* M1L1BelAr = NULL; 
        tree->SetBranchAddress( "M1L1BirkeLayer", &M1L1BelAr );
        vector<double>* M1L2BelAr = NULL; 
        tree->SetBranchAddress( "M1L2BirkeLayer", &M1L2BelAr );
        vector<double>* M1L3BelAr = NULL; 
        tree->SetBranchAddress( "M1L3BirkeLayer", &M1L3BelAr );
        vector<double>* M1L4BelAr = NULL; 
        tree->SetBranchAddress( "M1L4BirkeLayer", &M1L4BelAr );
        vector<double>* M2L1BelAr = NULL; 
        tree->SetBranchAddress( "M2L1BirkeLayer", &M2L1BelAr );
        vector<double>* M2L2BelAr = NULL; 
        tree->SetBranchAddress( "M2L2BirkeLayer", &M2L2BelAr );
        vector<double>* M2L3BelAr = NULL; 
        tree->SetBranchAddress( "M2L3BirkeLayer", &M2L3BelAr );
        vector<double>* M2L4BelAr = NULL; 
        tree->SetBranchAddress( "M2L4BirkeLayer", &M2L4BelAr );
        vector<double>* M3L1BelAr = NULL; 
        tree->SetBranchAddress( "M3L1BirkeLayer", &M3L1BelAr );
        vector<double>* M3L2BelAr = NULL; 
        tree->SetBranchAddress( "M3L2BirkeLayer", &M3L2BelAr );
        vector<double>* M3L3BelAr = NULL; 
        tree->SetBranchAddress( "M3L3BirkeLayer", &M3L3BelAr );
        vector<double>* M3L4BelAr = NULL; 
        tree->SetBranchAddress( "M3L4BirkeLayer", &M3L4BelAr );
         
        int nBins = 100;

        //total leak (world+crostat)
        //
        auto H1Leak = new TH1F("pi-ELeak", "pi-ELeak",
                              nBins*10, 0., pienergies[RunNo]/5. );
        //total signal no cuts
        //
        auto H1TotS = new TH1F("pi-TotS", "pi-TotS",
                              nBins*20, 0., pienergies[RunNo]*100. );
        //total signal no Birks
        //
        auto H1TotnoBS = new TH1F("pi-noBTotS", "pi-noBTotS",
                              nBins*20, 0., pienergies[RunNo]*100. );
        //total vis energy deposited
        //
        auto H1Econt = new TH1F("pi-Econt", "pi-Econt", 
                              nBins*10, 0., pienergies[RunNo]*2 );
        //total energy (vis+leak)
        //
        auto H1Etot = new TH1F("pi-Etot", "pi-Etot",
                               nBins*10, 0., pienergies[RunNo]*2 );
        //number of channels over cut
        //
        auto H1Channels = new TH1I("pi-Channels", "pi-Channels",
                                   88, 0, 88 ); 
        //disribution of signals per channel over cut
        //
        auto H1Signals = new TH1F("pi-Signals", "pi-Signal",
                                   nBins*2, 0., 5000. );
        //sum of signals over cut
        //
        auto H1TotCutSignal = new TH1F("pi-CutTotS", "pi-CutTotS",
                                      nBins*20, 0., pienergies[RunNo]*100 ); 
        //response
        //
        auto H1Response = new TH1F("pi-Response","pi-Response",
                                   2*120, 0., 1.0 );
        //reconstructed energy
        //
        auto H1Recenergy = new TH1F( "pi-Reconstructedenergy",
                "pi-Reconstructedenergy", nBins*10, 0., 200. );

        //F1 fraction of energy first layer, same for F2, F3 and F4
        //
        auto H1F1 = new TH1F("pi-F1","pi-F1",nBins,0.,1.2);
        auto H1F2 = new TH1F("pi-F2","pi-F2",nBins,0.,1.2);
        auto H1F3 = new TH1F("pi-F3","pi-F3",nBins,0.,1.2);
        auto H1F4 = new TH1F("pi-F4","pi-F4",nBins,0.,1.2);

        //For loop over events
        //
        for ( unsigned int eventNo = 0; eventNo<tree->GetEntries(); eventNo++ ){
            tree->GetEntry(eventNo);

            H1Leak->Fill((lenergy+cenergy)/1000.);
            H1TotS->Fill(BelAr);
            H1TotnoBS->Fill(elAr);
            H1Econt->Fill(pienergies[RunNo]-lenergy/1000.-cenergy/1000.);
            H1Etot->Fill( (edep+lenergy+cenergy)/1000. );

            double addchannels= 0 ;
            double addchannelsF1 = 0;
            double addchannelsF2 = 0;
            double addchannelsF3 = 0;
            double addchannelsF4 = 0;
            int channels = 0;

            for (unsigned int i = 0; i<M2L1BelAr->size(); i++){
                if ( i==0 || i==1 || i==2 || i==3 || i==4 || i==5 ) { 
                    channels += 1;
                    addchannels += M2L1BelAr->at(i);
                    addchannelsF1 += M2L1BelAr->at(i);
                    H1Signals->Fill( M2L1BelAr->at(i)) ;
                }
                if ( i==2 || i==4 ) { 
                    channels += 1;
                    addchannels += M1L1BelAr->at(i);
                    addchannelsF1 += M1L1BelAr->at(i);
                    H1Signals->Fill( M1L1BelAr->at(i)) ;
                }
                if ( i==3 || i==5 ) { 
                    channels += 1;
                    addchannels += M3L1BelAr->at(i);
                    addchannelsF1 += M3L1BelAr->at(i);
                    H1Signals->Fill( M3L1BelAr->at(i)) ;
                }
            }
            for (unsigned int i = 0; i<M2L2BelAr->size(); i++){
                if ( i==2 || i==3 || i==4 || i==5 || i==6 || i==7 || 
                     i==8 || i==9 ) { 
                    channels += 1;
                    addchannels += M2L2BelAr->at(i);
                    addchannelsF2 += M2L2BelAr->at(i);
                    H1Signals->Fill( M2L2BelAr->at(i)); 
                }
                if ( i==2 || i==4 || i==6 || i==8 ) { 
                    channels += 1;
                    addchannels+= M1L2BelAr->at(i);
                    addchannelsF2 += M1L2BelAr->at(i);
                    H1Signals->Fill( M1L2BelAr->at(i)); 
                }
                if ( i==3 || i==5 || i==7 || i==9 ) { 
                    channels += 1;
                    addchannels += M3L2BelAr->at(i);
                    addchannelsF2 += M3L2BelAr->at(i);
                    H1Signals->Fill( M3L2BelAr->at(i)); 
                }
            }
            for (unsigned int i = 0; i<M2L3BelAr->size(); i++){
                if ( i==4 || i==5 || i==6 || i==7 || i==8 || i==9 ||
                     i==10 || i==11 ) { 
                    channels += 1;
                    addchannels+= 2.*M2L3BelAr->at(i);
                    addchannelsF3 += 2.*M2L3BelAr->at(i);
                    H1Signals->Fill( M2L3BelAr->at(i)); 
                }
                if ( i==4 || i==6 || i==8 || i==10 ) { 
                    channels += 1;
                    addchannels+= 2.*M1L3BelAr->at(i);
                    addchannelsF3 += 2.*M1L3BelAr->at(i);
                    H1Signals->Fill( M1L3BelAr->at(i)); 
                }
                if ( i==5 || i==7 || i==9 || i==11 ) { 
                    channels += 1;
                    addchannels+= 2.*M3L3BelAr->at(i);
                    addchannelsF3 += 2.*M3L3BelAr->at(i);
                    H1Signals->Fill( M3L3BelAr->at(i)); 
                }
            }
            for (unsigned int i = 0; i<M2L4BelAr->size(); i++){
                if ( i==6 || i==7 || i==8 || i==9 || i==10 || i==11 ) { 
                    channels += 1;
                    addchannels+= 2.*M2L4BelAr->at(i);
                    addchannelsF4 += 2.*M2L4BelAr->at(i);
                    H1Signals->Fill( M2L4BelAr->at(i)); 
                }
                if ( i==8 ) { 
                    channels += 1;
                    addchannels+= 2.*M1L4BelAr->at(i);
                    addchannelsF4 += 2.*M1L4BelAr->at(i);
                    H1Signals->Fill( M1L4BelAr->at(i)); 
                }
                if ( i==9 ) { 
                    channels += 1;
                    addchannels+= 2.*M3L4BelAr->at(i);
                    addchannelsF4 += 2.*M3L4BelAr->at(i);
                    H1Signals->Fill( M3L4BelAr->at(i)); 
                }
            }
            
            H1TotCutSignal->Fill( addchannels );
            H1F1->Fill( addchannelsF1/addchannels );
            H1F2->Fill( addchannelsF2/addchannels );
            H1F3->Fill( addchannelsF3/addchannels );
            H1F4->Fill( addchannelsF4/addchannels );
            H1Channels->Fill(channels);
            H1Response->Fill( (addchannels / pienergies[RunNo])/44.7987 ); //pi/e
            // average response 44.7987 a.u./GeV
            H1Recenergy->Fill( addchannels / 44.7987 ); 
        } //end for loop events

        energies[RunNo] = pienergies[RunNo];

        responses[RunNo] = H1Response->GetMean();
        erresponses[RunNo] = 10.*H1Response->GetMeanError();
        
        double xfitmin = H1Recenergy->GetMean()-2.*H1Recenergy->GetStdDev();
        double xfitmax = H1Recenergy->GetMean()+2.*H1Recenergy->GetStdDev();
        auto F1Recenergy = new TF1("rgaus","gaus(0)",xfitmin,xfitmax);
        H1Recenergy->Fit(F1Recenergy,"QR");
        recenergies[RunNo] = H1Recenergy->
            GetFunction("rgaus")->GetParameter(1)/pienergies[RunNo];
        errecenergies[RunNo] = H1Recenergy->
            GetFunction("rgaus")->GetParError(1)/pienergies[RunNo];
        energyresolution[RunNo] =100.*H1Recenergy->GetFunction("rgaus")->
            GetParameter(2)/H1Recenergy->GetFunction("rgaus")->GetParameter(1);
        erenergyresolution[RunNo] = (H1Recenergy->GetFunction("rgaus")->
                GetParError(2)/H1Recenergy->GetFunction("rgaus")->GetParameter(2) +
                H1Recenergy->GetFunction("rgaus")->GetParError(1)/
                H1Recenergy->GetFunction("rgaus")->GetParameter(1))
                                                    *energyresolution[RunNo];

        F1[RunNo] = H1F1->GetMean();
        erF1[RunNo] = H1F1->GetMeanError();
        F2[RunNo] = H1F2->GetMean();
        erF2[RunNo] = H1F2->GetMeanError();
        F3[RunNo] = H1F3->GetMean();
        erF3[RunNo] = H1F3->GetMeanError();
        F4[RunNo] = H1F4->GetMean();
        erF4[RunNo] = H1F4->GetMeanError();
        L0[RunNo] = (28.05/2.)*F1[RunNo]+(28.05+53.6/2.)*F2[RunNo]+
                    (28.05+53.6+53.35/2.)*F3[RunNo]+
                    (28.05+53.6+53.35+46.8/2)*F4[RunNo];
        double depths[4] = {28.05/2.,28.05+53.6/2.,
                         28.05+53.6+53.35/2.,
                         28.05+53.6+53.35+46.8/2};
        double residual = std::pow(depths[0]*F1[RunNo],2.)+
                          std::pow(depths[1]*F2[RunNo],2.)+
                          std::pow(depths[2]*F3[RunNo],2.)+
                          std::pow(depths[3]*F4[RunNo],2.);
        sigmaL0[RunNo] = 2.*std::sqrt(residual/4.); 

        outputfile->cd();
        H1Leak->Write();
        delete H1Leak;
        H1Econt->Write();
        delete H1Econt;
        H1TotS->Write();
        delete H1TotS;
        H1TotnoBS->Write();
        delete H1TotnoBS;
        H1Etot->Write();
        delete H1Etot;
       
        H1TotCutSignal->Write();
        H1F1->Write();
        H1F2->Write();
        H1F3->Write();
        H1F4->Write();
        H1Channels->Write();
        H1Signals->Write();
        H1Response->Write();
        H1Recenergy->Write();
    }

    // Finalize objects over multiple runs
    //
    outputfile->cd();
   
    //Part for reconstructed energy and energy resolution
    //
    auto G1recenergies = new TGraphErrors( pienergies.size(), energies, recenergies,
                                           zeros, errecenergies);
    G1recenergies->GetYaxis()->SetRangeUser(0.75,0.9);
    G1recenergies->GetXaxis()->SetRangeUser(0.,220.);
    G1recenergies->SetMarkerStyle(8); 
    G1recenergies->SetMarkerColor(kRed);
    G1recenergies->SetLineColor(kRed);
    G1recenergies->SetName("pi-recenergies");
    G1recenergies->SetTitle("pi-recenergies");
    G1recenergies->GetYaxis()->SetTitle("#pi / e");
    G1recenergies->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1recenergies->Draw("AP");
    G1recenergies->Write();
    delete G1recenergies;

    auto G1energyresolution = new TGraphErrors( pienergies.size(), energies,
            energyresolution, zeros, erenergyresolution );
    G1energyresolution->SetMarkerStyle(8);
    G1energyresolution->SetMarkerColor(kRed);
    G1energyresolution->SetLineColor(kRed);
    G1energyresolution->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1energyresolution->GetYaxis()->SetTitle("#sigma_{0}/E_{0} [%]");
    G1energyresolution->GetYaxis()->SetRangeUser(0.,20.);
    G1energyresolution->GetXaxis()->SetRangeUser(0.,220.);
    G1energyresolution->SetTitle("pi-energyresolution");
    G1energyresolution->SetName("pi-energyresolution");
    G1energyresolution->Write();
    double ATLASres[11] = {13.465587044534415,13.465587044534415,11.546558704453442,
                           11.279352226720649,11.060728744939272,10.137651821862349,
                           9.433198380566802,8.777327935222674,8.000000000000002,
                           7.465587044534413,7.684210526315789};
    double erATLASres[11] = {14.47-13.47,13.89-13.47, 11.85-11.55, 11.52-11.28,
                            11.27-11.06, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001};
    auto G1ATLASres =new TGraphErrors( pienergies.size(), energies,
                                      ATLASres, zeros, erATLASres );
    G1ATLASres->SetMarkerStyle(8); 
    G1ATLASres->GetYaxis()->SetRangeUser(0.,20.);
    G1ATLASres->GetXaxis()->SetRangeUser(0.,220.);
    G1ATLASres->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1ATLASres->GetYaxis()->SetTitle("#sigma_{0}/E_{0} [%]");
    auto C1res = new TCanvas("pi-Canvas_resolution", "", 700, 900);
    auto *p2res = new TPad("p2","p2",0.,0.02,1.,0.32); p2res->Draw();
    auto *p1res = new TPad("p1","p1",0.,0.3,1.,1.);  p1res->Draw();
    p1res->cd();
    G1ATLASres->SetTitle("");
    G1ATLASres->Draw("AP");
    G1energyresolution->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto Freslegend = new TLegend(1.-0.18,0.7,1.-0.61,0.89);
    Freslegend->AddEntry(G1ATLASres,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}",
    "ep");
    Freslegend->AddEntry(G1energyresolution,
    "#splitline{ATLHECTB v1.1 }{#splitline{Geant4.10.7.p01 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    Freslegend->SetLineWidth(0);
    Freslegend->Draw("same");
    p2res->cd();
    gPad->SetLeftMargin(0.15);
    TGraphErrors* ratiores = new TGraphErrors(pienergies.size());ratiores->SetTitle("");
    for (unsigned int i=0; i<pienergies.size(); i++){
        ratiores->SetPoint(i, energies[i], energyresolution[i]/ATLASres[i]);
        ratiores->SetPointError(i, 0.,
                (erenergyresolution[i]/energyresolution[i]+
                 erATLASres[i]/ATLASres[i])*energyresolution[i]/ATLASres[i]);
    }
    ratiores->GetYaxis()->SetTitle("MC/Data");
    ratiores->SetMarkerStyle(8);
    ratiores->GetYaxis()->SetLabelSize(0.09);ratiores->GetXaxis()->SetLabelSize(0.09);
    ratiores->GetYaxis()->SetTitleSize(0.09);ratiores->GetYaxis()->SetTitleOffset(0.65);
    ratiores->GetYaxis()->SetRangeUser(0.7,1.2);
    ratiores->Draw("AP");
    C1res->Write();
    delete p1res; delete p2res, delete ratiores;
    delete C1res;
    delete G1energyresolution;
    delete G1ATLASres;

    //Part for pi/e
    //
    auto G1responses = new TGraphErrors( pienergies.size(), energies, responses, 
                                         zeros, erresponses );
    G1responses->GetYaxis()->SetRangeUser(0.75,0.9);
    G1responses->GetXaxis()->SetRangeUser(0.,220.);
    G1responses->SetMarkerStyle(8); 
    G1responses->SetMarkerColor(kRed);
    G1responses->SetLineColor(kRed);
    G1responses->SetName("pi-responses");
    G1responses->SetTitle("pi-responses");
    G1responses->GetYaxis()->SetTitle("#pi / e");
    G1responses->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    double ATLASresponse[7] = {0.777, 0.796, 0.8026, 0.819, 0.825, 0.829, 0.840};
    double erATLASresponse[7] = {0.79-0.777, 0.8095-0.796, 0.816-0.8026, 
                                0.833-0.819, 0.84-0.825, 0.844-0.829, 0.854-0.840};
    double ATLASresenergies[7] = {20.,40.,50.,80.,100.,120.,150.};
    double ATLASzeros[7]; memset( ATLASzeros, 0., 7*sizeof(double));
    auto G1ATLASresponse = new TGraphErrors( 7, ATLASresenergies, ATLASresponse,
                                                ATLASzeros, erATLASresponse );
    G1ATLASresponse->SetMarkerStyle(8);
    G1ATLASresponse->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1ATLASresponse->GetYaxis()->SetTitle("#pi/e");
    G1ATLASresponse->GetYaxis()->SetRangeUser(0.75,0.9);
    G1ATLASresponse->GetXaxis()->SetRangeUser(0.,220.);
    G1ATLASresponse->SetTitle("");
    G1ATLASresponse->SetName("pi-ATLASresponse");
    G1ATLASresponse->Write();
    auto C1piresponse = new TCanvas("pi-Canvas_response", "", 700, 900);
    auto *p2resp = new TPad("p2","p2",0.,0.02,1.,0.32); p2resp->Draw();
    auto *p1resp = new TPad("p1","p1",0.,0.3,1.,1.);  p1resp->Draw();
    p1resp->cd();
    G1ATLASresponse->Draw("AP");
    G1responses->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto legend = new TLegend(0.18,0.7,0.61,0.89);
    legend->AddEntry(G1ATLASresponse,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}",
    "ep");
    legend->AddEntry(G1responses,
    "#splitline{ATLHECTB v1.1 }{#splitline{Geant4.10.7.p01 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    legend->SetLineWidth(0);
    legend->Draw("same");
    p2resp->cd();
    gPad->SetLeftMargin(0.15);
    TGraphErrors* ratioresp = new TGraphErrors(pienergies.size());
    ratioresp->SetTitle("");
    int modindex[7] = {0,2,3,5,6,7,8};
    int index = 0;
    for (unsigned int i=0; i<7; i++){
        index = modindex[i];
        ratioresp->SetPoint(i, ATLASresenergies[i], 
                responses[index]/ATLASresponse[i]);
        ratioresp->SetPointError(i, 0.,
                (erresponses[index]/responses[index]+
                erATLASresponse[i]/ATLASresponse[i])*responses[index]/ATLASresponse[i]);
    }
    ratioresp->GetYaxis()->SetTitle("MC/Data");
    ratioresp->SetMarkerStyle(8);
    ratioresp->GetYaxis()->SetLabelSize(0.09);ratioresp->GetXaxis()->SetLabelSize(0.09);
    ratioresp->GetYaxis()->SetTitleSize(0.09);
    ratioresp->GetYaxis()->SetTitleOffset(0.65);
    ratioresp->GetYaxis()->SetRangeUser(0.7,1.2);
    ratioresp->Draw("AP");
    C1piresponse->Write();
    delete p1resp; delete p2resp; delete ratioresp;
    delete C1piresponse;
    G1responses->Write();
    delete G1responses;
    delete G1ATLASresponse;
    
    //Graphs and canvases for spatial development
    //
    auto G1F1 = new TGraphErrors( pienergies.size(), energies,                                                             F1, zeros, erF1 );
    G1F1->GetYaxis()->SetRangeUser(0.2,0.4);
    G1F1->SetMarkerStyle(8); 
    G1F1->SetMarkerColor(kRed);
    G1F1->SetLineColor(kRed);
    G1F1->SetName("pi-F1");
    G1F1->SetTitle("pi-F1");
    G1F1->GetYaxis()->SetTitle("Signal Layer 1 / Signal");
    G1F1->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1F1->Write();
    double ATLASF1[11] = {0.3604477611940299, 0.3171641791044777, 0.3067164179104478, 
        0.29179104477611945, 0.2776119402985075, 0.26268656716417915, 
        0.2492537313432836, 0.2373134328358209, 0.232089552238806, 0.2201492537313433, 
        0.21716417910447766};
    double erATLASF1[11] = {0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 
                            0.001, 0.001, 0.001, 0.001, 0.001};
    auto G1ATLASF1 =new TGraphErrors( pienergies.size(), energies,
                                      ATLASF1, zeros, erATLASF1 );
    G1ATLASF1->SetMarkerStyle(8); 
    G1ATLASF1->GetYaxis()->SetRangeUser(0.2,0.4);
    G1ATLASF1->GetYaxis()->SetTitle("Signal Layer 1 / Signal");
    G1ATLASF1->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    auto C1F1 = new TCanvas("pi-Canvas_F1", "", 700, 900);
    auto *p2F1 = new TPad("p2","p2",0.,0.02,1.,0.32); p2F1->Draw();
    auto *p1F1 = new TPad("p1","p1",0.,0.3,1.,1.);  p1F1->Draw();
    p1F1->cd();
    G1ATLASF1->SetTitle("Layer 1");
    G1ATLASF1->Draw("AP");
    G1F1->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto F1legend = new TLegend(1.-0.18,0.7,1.-0.61,0.89);
    F1legend->AddEntry(G1ATLASF1,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}",
    "ep");
    F1legend->AddEntry(G1F1,
    "#splitline{ATLHECTB v1.1 }{#splitline{Geant4.10.7.p01 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    F1legend->SetLineWidth(0);
    F1legend->Draw("same");
    p2F1->cd();
    gPad->SetLeftMargin(0.15);
    TGraphErrors* ratioF1 = new TGraphErrors(pienergies.size()); ratioF1->SetTitle("");
    for (unsigned int i=0; i<pienergies.size(); i++){
        ratioF1->SetPoint(i, energies[i], F1[i]/ATLASF1[i]);
        ratioF1->SetPointError(i, 0.,
                (erF1[i]/F1[i]+erATLASF1[i]/ATLASF1[i])*F1[i]/ATLASF1[i]);
    }
    ratioF1->GetYaxis()->SetTitle("MC/Data");
    ratioF1->SetMarkerStyle(8);
    ratioF1->GetYaxis()->SetLabelSize(0.09); ratioF1->GetXaxis()->SetLabelSize(0.09);
    ratioF1->GetYaxis()->SetTitleSize(0.09); ratioF1->GetYaxis()->SetTitleOffset(0.65);
    ratioF1->GetYaxis()->SetRangeUser(0.8,1.2);
    ratioF1->Draw("AP");
    C1F1->Write();
    delete ratioF1; delete p1F1; delete p2F1;
    delete C1F1;
    delete G1F1;
    delete G1ATLASF1;

    auto G1F2 = new TGraphErrors( pienergies.size(), energies,                                                             F2, zeros, erF2 );
    G1F2->GetYaxis()->SetRangeUser(0.50,0.6);
    G1F2->SetMarkerStyle(8); 
    G1F2->SetMarkerColor(kRed);
    G1F2->SetLineColor(kRed);
    G1F2->SetName("pi-F2");
    G1F2->SetTitle("pi-F2");
    G1F2->GetYaxis()->SetTitle("F");
    G1F2->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1F2->Write();
    double ATLASF2[11] = {0.5226759644691121,0.5413080964409817,0.5420448049714781,
        0.5453379347967886,0.547879650383653,0.5519099631171358,0.5500750702671934,
        0.5521509469764354,0.5542111692223760,0.5543160066887254,0.5547372540648239};
    double erATLASF2[11] = {0.005,0.005, 0.005, 0.005, 0.005, 0.005, 
                            0.005, 0.005, 0.005, 0.005, 0.005};
    auto G1ATLASF2 =new TGraphErrors( pienergies.size(), energies,
                                      ATLASF2, zeros, erATLASF2 );
    G1ATLASF2->SetMarkerStyle(8); 
    G1ATLASF2->GetYaxis()->SetRangeUser(0.5,0.6);
    G1ATLASF2->GetYaxis()->SetTitle("Signal Layer 2 / Signal");
    G1ATLASF2->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    auto C1F2 = new TCanvas("pi-Canvas_F2", "", 700, 900);
    auto *p2F2 = new TPad("p2","p2",0.,0.02,1.,0.32); p2F2->Draw();
    auto *p1F2 = new TPad("p1","p1",0.,0.3,1.,1.);  p1F2->Draw();
    p1F2->cd();
    G1ATLASF2->SetTitle("Layer 2");
    G1ATLASF2->Draw("AP");
    G1F2->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto F2legend = new TLegend(1.-0.18,0.7,1.-0.61,0.89);
    F2legend->AddEntry(G1ATLASF2,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}",
    "ep");
    F2legend->AddEntry(G1F2,
    "#splitline{ATLHECTB v1.1 }{#splitline{Geant4.10.7.p01 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    F2legend->SetLineWidth(0);
    F2legend->Draw("same");
    p2F2->cd();
    gPad->SetLeftMargin(0.15);
    TGraphErrors* ratioF2 = new TGraphErrors(pienergies.size()); ratioF2->SetTitle("");
    for (unsigned int i=0; i<pienergies.size(); i++){
        ratioF2->SetPoint(i, energies[i], F2[i]/ATLASF2[i]);
        ratioF2->SetPointError(i, 0.,
                (erF2[i]/F2[i]+erATLASF2[i]/ATLASF2[i])*F2[i]/ATLASF2[i]);
    }
    ratioF2->GetYaxis()->SetTitle("MC/Data");
    ratioF2->SetMarkerStyle(8);
    ratioF2->GetYaxis()->SetLabelSize(0.09); ratioF2->GetXaxis()->SetLabelSize(0.09);
    ratioF2->GetYaxis()->SetTitleSize(0.09); ratioF2->GetYaxis()->SetTitleOffset(0.65);
    ratioF2->GetYaxis()->SetRangeUser(0.8,1.2);
    ratioF2->Draw("AP");
    C1F2->Write();
    delete p2F2; delete p1F2;
    delete C1F2;
    delete G1F2;
    delete G1ATLASF2;
    delete ratioF2;

    auto G1F3 = new TGraphErrors( pienergies.size(), energies,                                                             F3, zeros, erF3 );
    G1F3->GetYaxis()->SetRangeUser(0.08,0.2);
    G1F3->SetMarkerStyle(8); 
    G1F3->SetMarkerColor(kRed);
    G1F3->SetLineColor(kRed);
    G1F3->SetName("pi-F3");
    G1F3->SetTitle("pi-F3");
    G1F3->GetYaxis()->SetTitle("F");
    G1F3->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1F3->Write();
    double ATLASF3[11] = {0.10217391304347825,0.12260869565217392,0.1317391304347826,
        0.1382608695652174,0.14739130434782607,0.1547826086956522,0.1656521739130435,
        0.17347826086956522,0.1739130434782609,0.18304347826086959,0.1839130434782609};
    double erATLASF3[11] = {0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,
                            0.0001,0.0001,0.0001,0.0001};
    auto G1ATLASF3 =new TGraphErrors( pienergies.size(), energies,
                                      ATLASF3, zeros, erATLASF3 );
    G1ATLASF3->SetMarkerStyle(8); 
    G1ATLASF3->GetYaxis()->SetRangeUser(0.08,0.2);
    G1ATLASF3->GetYaxis()->SetTitle("Signal Layer 3 / Signal");
    G1ATLASF3->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    auto C1F3 = new TCanvas("pi-Canvas_F3", "", 700, 900);
    auto *p2F3 = new TPad("p2","p2",0.,0.02,1.,0.32); p2F3->Draw();
    auto *p1F3 = new TPad("p1","p1",0.,0.3,1.,1.);  p1F3->Draw();
    p1F3->cd();
    G1ATLASF3->SetTitle("Layer 3");
    G1ATLASF3->Draw("AP");
    G1F3->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto F3legend = new TLegend(1.-0.18,1.05-0.7,1.-0.61,1.05-0.89);
    F3legend->AddEntry(G1ATLASF3,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}",
    "ep");
    F3legend->AddEntry(G1F3,
    "#splitline{ATLHECTB v1.1 }{#splitline{Geant4.10.7.p01 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    F3legend->SetLineWidth(0);
    F3legend->Draw("same");
    p2F3->cd();
    gPad->SetLeftMargin(0.15);
    TGraphErrors* ratioF3 = new TGraphErrors(pienergies.size()); ratioF3->SetTitle("");
    for (unsigned int i=0; i<pienergies.size(); i++){
        ratioF3->SetPoint(i, energies[i], F3[i]/ATLASF3[i]);
        ratioF3->SetPointError(i, 0.,
                (erF3[i]/F3[i]+erATLASF3[i]/ATLASF3[i])*F3[i]/ATLASF3[i]);
    }
    ratioF3->GetYaxis()->SetTitle("MC/Data");
    ratioF3->SetMarkerStyle(8);
    ratioF3->GetYaxis()->SetLabelSize(0.09); ratioF3->GetXaxis()->SetLabelSize(0.09);
    ratioF3->GetYaxis()->SetTitleSize(0.09); ratioF3->GetYaxis()->SetTitleOffset(0.65);
    ratioF3->GetYaxis()->SetRangeUser(0.8,1.2);
    ratioF3->Draw("AP");
    C1F3->Write();
    delete p1F3; delete p2F3;
    delete C1F3;
    delete ratioF3; 
    delete G1F3;
    delete G1ATLASF3;

    auto G1F4 = new TGraphErrors( pienergies.size(), energies,                                                             F4, zeros, erF4 );
    G1F4->GetYaxis()->SetRangeUser(0.,0.06);
    G1F4->SetMarkerStyle(8); 
    G1F4->SetMarkerColor(kRed);
    G1F4->SetLineColor(kRed);
    G1F4->SetName("pi-F4");
    G1F4->SetTitle("pi-F4");
    G1F4->GetYaxis()->SetTitle("F");
    G1F4->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1F4->Write();
    double ATLASF4[11] = {0.015242718446601938,0.019223300970873783,
        0.02029126213592232,0.025728155339805818,0.029126213592233004,
        0.03223300970873785,0.03601941747572815,0.03796116504854368,
        0.04106796116504853,0.043495145631067954,0.04436893203883494};
    double erATLASF4[11] = {0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,
                            0.0001,0.0001,0.0001,0.0001};
    auto G1ATLASF4 = new TGraphErrors( pienergies.size(), energies,
                                      ATLASF4, zeros, erATLASF4 );
    G1ATLASF4->SetMarkerStyle(8); 
    G1ATLASF4->SetTitle("Layer 4");
    G1ATLASF4->GetYaxis()->SetRangeUser(0.0,0.06);
    G1ATLASF4->GetYaxis()->SetTitle("Signal Layer4 / Signal");
    G1ATLASF4->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    auto C1F4 = new TCanvas("pi-Canvas_F4", "", 700, 900);
    auto *p2 = new TPad("p2","p2",0.,0.02,1.,0.32); p2->Draw();
    auto *p1 = new TPad("p1","p1",0.,0.3,1.,1.);  p1->Draw();
    p1->cd();
    G1ATLASF4->Draw("AP");
    G1F4->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto F4legend = new TLegend(1.-0.18,1.05-0.7,1.-0.61,1.05-0.89);
    F4legend->AddEntry(G1ATLASF4,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}",
    "ep");
    F4legend->AddEntry(G1F4,
    "#splitline{ATLHECTB v1.1 }{#splitline{Geant4.10.7.p01 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    F4legend->SetLineWidth(0);
    F4legend->Draw("same");
    p2->cd();
    gPad->SetLeftMargin(0.15);
    TGraphErrors* ratio = new TGraphErrors(pienergies.size()); ratio->SetTitle("");
    for (unsigned int i=0; i<pienergies.size(); i++){
        ratio->SetPoint(i, energies[i], F4[i]/ATLASF4[i]);
        ratio->SetPointError(i, 0.,
                (erF4[i]/F4[i]+erATLASF4[i]/ATLASF4[i])*F4[i]/ATLASF4[i]);
    }
    ratio->GetYaxis()->SetTitle("MC/Data");
    ratio->SetMarkerStyle(8);
    ratio->GetYaxis()->SetLabelSize(0.09); ratio->GetXaxis()->SetLabelSize(0.09);
    ratio->GetYaxis()->SetTitleSize(0.09); ratio->GetYaxis()->SetTitleOffset(0.65);
    ratio->GetYaxis()->SetRangeUser(0.8,1.2);
    ratio->Draw("AP");
    C1F4->Write();
    delete ratio; 
    delete p2;
    delete p1;
    delete C1F4;
    delete G1F4;
    delete G1ATLASF4;

    //Graph and canvas L0 and sigmaL0
    //
    auto G1L0 = new TGraphErrors( pienergies.size(), energies,                                                            L0, zeros, zeros );
    G1L0->GetYaxis()->SetRangeUser(42.,62.);
    G1L0->SetMarkerStyle(8); 
    G1L0->SetMarkerColor(kRed);
    G1L0->SetLineColor(kRed);
    G1L0->SetName("pi-L0");
    G1L0->SetTitle("pi-L0");
    G1L0->GetYaxis()->SetTitle("L_{0} [cm]");
    G1L0->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1L0->Write();
    double ATLASL0[11] = {47.20989874689059,50.415242855641324,51.444637789482286,
        52.951825942751704,54.405759865463,55.72198021652049,57.223749518259424,
        58.30097983112803,58.86865124318264,60.04659745173835,60.27474978686629};
    //double ATLASL0[11] = {2.443793103448276,2.613312390414962,2.66645675043834,
    //    2.746760666277031,2.8231808883693748,2.8919739918176504,2.972400642898889,
    //    3.0295558153126825,3.0622735242548216,3.1260257159555813,3.1379178842781994};
    double erATLASL0[11] = {0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,
                            0.0001,0.0001,0.0001,0.0001};
    auto G1ATLASL0 = new TGraphErrors( pienergies.size(), energies,
                                      ATLASL0, zeros, erATLASL0 );
    G1ATLASL0->SetMarkerStyle(8); 
    G1ATLASL0->GetYaxis()->SetRangeUser(42.,62.);
    G1ATLASL0->GetYaxis()->SetTitle("L_{0} [cm]");
    G1ATLASL0->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    auto C1L0 = new TCanvas("pi-Canvas_L0", "", 700, 900);
    auto *p2L0 = new TPad("p2","p2",0.,0.02,1.,0.32); p2L0->Draw();
    auto *p1L0 = new TPad("p1","p1",0.,0.3,1.,1.);  p1L0->Draw();
    p1L0->cd();
    G1ATLASL0->SetTitle("");
    G1ATLASL0->Draw("AP");
    G1L0->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto L0legend = new TLegend(1.-0.18,1.05-0.7,1.-0.61,1.05-0.89);
    L0legend->AddEntry(G1ATLASL0,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}",
    "ep");
    L0legend->AddEntry(G1L0,
    "#splitline{ATLHECTB v1.1 }{#splitline{Geant4.10.7.p01 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    L0legend->SetLineWidth(0);
    L0legend->Draw("same");
    p2L0->cd();
    gPad->SetLeftMargin(0.15);
    TGraph* ratioL0 = new TGraph(pienergies.size()); ratioL0->SetTitle("");
    for (unsigned int i=0; i<pienergies.size(); i++){
        ratioL0->SetPoint(i, energies[i], L0[i]/ATLASL0[i]);
    }
    ratioL0->GetYaxis()->SetTitle("MC/Data");
    ratioL0->SetMarkerStyle(8);
    ratioL0->GetYaxis()->SetLabelSize(0.09); ratioL0->GetXaxis()->SetLabelSize(0.09);
    ratioL0->GetYaxis()->SetTitleSize(0.09); ratioL0->GetYaxis()->SetTitleOffset(0.65);
    ratioL0->GetYaxis()->SetRangeUser(0.8,1.5);
    ratioL0->Draw("AP");
    C1L0->Write();
    delete ratioL0; delete p1L0; delete p2L0;
    delete C1L0;
    delete G1L0;
    delete G1ATLASL0;

    auto G1sigmaL0 = new TGraphErrors( pienergies.size(), energies,                                                            sigmaL0, zeros, zeros );
    G1sigmaL0->GetYaxis()->SetRangeUser(30.,38.);
    G1sigmaL0->SetMarkerStyle(8); 
    G1sigmaL0->SetMarkerColor(kRed);
    G1sigmaL0->SetLineColor(kRed);
    G1sigmaL0->SetName("pi-sigmaL0");
    G1sigmaL0->SetTitle("pi-sigmaL0");
    G1sigmaL0->GetYaxis()->SetTitle("#sigma_{L} [cm]");
    G1sigmaL0->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1sigmaL0->Write();
    double ATLASsigmaL0[11] = {31.561680356622446,32.623646745172536,
        33.04349844031647,33.923354237050305,34.50614698802983,
        34.94316059883212,35.591168629313415,35.798147808347814,
        36.09916133280085,36.380940559327335,36.45373298649713};
    double erATLASsigmaL0[11]; memset( erATLASsigmaL0, 0., 11*sizeof(double));
    auto G1ATLASsigmaL0 = new TGraphErrors( pienergies.size(), energies,
                                      ATLASsigmaL0, zeros, erATLASsigmaL0 );
    G1ATLASsigmaL0->SetMarkerStyle(8); 
    G1ATLASsigmaL0->GetYaxis()->SetRangeUser(30.,38.);
    G1ATLASsigmaL0->GetYaxis()->SetTitle("#sigma_{L} [cm]");
    G1ATLASsigmaL0->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    auto C1sigmaL0 = new TCanvas("pi-Canvas_sigmaL0", "", 700, 900);
    auto *p2sigmaL0 = new TPad("p2","p2",0.,0.02,1.,0.32); p2sigmaL0->Draw();
    auto *p1sigmaL0 = new TPad("p1","p1",0.,0.3,1.,1.);  p1sigmaL0->Draw();
    p1sigmaL0->cd();
    G1ATLASsigmaL0->Draw("AP");
    G1sigmaL0->Draw("same P");
    gPad->SetLeftMargin(0.15);
    auto sigmaL0legend = new TLegend(1.-0.18,1.05-0.7,1.-0.61,1.05-0.89);
    sigmaL0legend->AddEntry(G1ATLASsigmaL0,
    "#splitline{ATLAS HEC}{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}",
    "ep");
    sigmaL0legend->AddEntry(G1sigmaL0,
    "#splitline{ATLHECTB v1.1 }{#splitline{Geant4.10.7.p01 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    sigmaL0legend->SetLineWidth(0);
    sigmaL0legend->Draw("same");
    p2sigmaL0->cd();
    gPad->SetLeftMargin(0.15);
    TGraph* ratiosigmaL0 = new TGraph(pienergies.size()); ratiosigmaL0->SetTitle("");
    for (unsigned int i=0; i<pienergies.size(); i++){
        ratiosigmaL0->SetPoint(i, energies[i], sigmaL0[i]/ATLASsigmaL0[i]);
    }
    ratiosigmaL0->GetYaxis()->SetTitle("MC/Data");
    ratiosigmaL0->SetMarkerStyle(8);
    ratiosigmaL0->GetYaxis()->SetLabelSize(0.09);
    ratiosigmaL0->GetXaxis()->SetLabelSize(0.09);
    ratiosigmaL0->GetYaxis()->SetTitleSize(0.09); 
    ratiosigmaL0->GetYaxis()->SetTitleOffset(0.65);
    ratiosigmaL0->GetYaxis()->SetRangeUser(0.8,1.2);
    ratiosigmaL0->Draw("AP");
    C1sigmaL0->Write();
    delete ratiosigmaL0, delete p1sigmaL0, delete p2sigmaL0;
    delete C1sigmaL0;
    delete G1sigmaL0;
    delete G1ATLASsigmaL0;

    outputfile->Close();
    delete outputfile;
    // Final print out
    //
    double k;
    for (unsigned int i = 0; i<pienergies.size(); i++){
        k += responses[i];
    }
    cout<<"->Average response to pi-: "<<k/double(pienergies.size())<<" a.u./GeV"<<endl;
    cout<<"ATLHECTB end of analysis of pi- runs"<<endl;
};

////////////////////////////////////////////////////////////////////////////////////
void picalibrate( const double& pienergy, const string& pifile ){

    //Initiate objects through all the analysis
    //
    cout<<"ATLHECTB analysis of channels to be selected with pi- runs"<<endl;
    auto outputfile = new TFile( "ATLHECTBchannelspi.root", "RECREATE" );
    cout<<"---> Analysis at energy(GeV) "<<pienergy<<endl;  
        
    string filename = "Data1/"+pifile;
    TFile* file = TFile::Open( filename.c_str(), "READ" );
    TTree* tree = (TTree*)file->Get( "ATLHECTBout" );

    vector<double>* M1L1BelAr = NULL; 
    tree->SetBranchAddress( "M1L1BirkeLayer", &M1L1BelAr );
    vector<double>* M1L2BelAr = NULL; 
    tree->SetBranchAddress( "M1L2BirkeLayer", &M1L2BelAr );
    vector<double>* M1L3BelAr = NULL; 
    tree->SetBranchAddress( "M1L3BirkeLayer", &M1L3BelAr );
    vector<double>* M1L4BelAr = NULL; 
    tree->SetBranchAddress( "M1L4BirkeLayer", &M1L4BelAr );
    vector<double>* M2L1BelAr = NULL; 
    tree->SetBranchAddress( "M2L1BirkeLayer", &M2L1BelAr );
    vector<double>* M2L2BelAr = NULL; 
    tree->SetBranchAddress( "M2L2BirkeLayer", &M2L2BelAr );
    vector<double>* M2L3BelAr = NULL; 
    tree->SetBranchAddress( "M2L3BirkeLayer", &M2L3BelAr );
    vector<double>* M2L4BelAr = NULL; 
    tree->SetBranchAddress( "M2L4BirkeLayer", &M2L4BelAr );
    vector<double>* M3L1BelAr = NULL; 
    tree->SetBranchAddress( "M3L1BirkeLayer", &M3L1BelAr );
    vector<double>* M3L2BelAr = NULL; 
    tree->SetBranchAddress( "M3L2BirkeLayer", &M3L2BelAr );
    vector<double>* M3L3BelAr = NULL; 
    tree->SetBranchAddress( "M3L3BirkeLayer", &M3L3BelAr );
    vector<double>* M3L4BelAr = NULL; 
    tree->SetBranchAddress( "M3L4BirkeLayer", &M3L4BelAr );

    double M1L1avg[24]; memset( M1L1avg, 0., 24*sizeof(double));
    double M2L1avg[24]; memset( M2L1avg, 0., 24*sizeof(double));
    double M3L1avg[24]; memset( M3L1avg, 0., 24*sizeof(double));
         
    double M1L2avg[23]; memset( M1L2avg, 0., 23*sizeof(double));
    double M2L2avg[23]; memset( M2L2avg, 0., 23*sizeof(double));
    double M3L2avg[23]; memset( M3L2avg, 0., 23*sizeof(double));

    double M1L3avg[21]; memset( M1L3avg, 0., 21*sizeof(double));
    double M2L3avg[21]; memset( M2L3avg, 0., 21*sizeof(double));
    double M3L3avg[21]; memset( M3L3avg, 0., 21*sizeof(double));

    double M1L4avg[20]; memset( M1L4avg, 0., 20*sizeof(double));
    double M2L4avg[20]; memset( M2L4avg, 0., 20*sizeof(double));
    double M3L4avg[20]; memset( M3L4avg, 0., 20*sizeof(double));

    //loop over events
    //
    for (unsigned int evtNo = 0; evtNo<tree->GetEntries(); evtNo++){
        tree->GetEntry(evtNo);

        for (unsigned int i = 0; i<24; i++){
            M2L1avg[i] += M2L1BelAr->at(i)/tree->GetEntries();
            M1L1avg[i] += M1L1BelAr->at(i)/tree->GetEntries();
            M3L1avg[i] += M3L1BelAr->at(i)/tree->GetEntries();
        }
        for (unsigned int i = 0; i<23; i++){
            M2L2avg[i] += M2L2BelAr->at(i)/tree->GetEntries();
            M1L2avg[i] += M1L2BelAr->at(i)/tree->GetEntries();
            M3L2avg[i] += M3L2BelAr->at(i)/tree->GetEntries();
        }
        for (unsigned int i = 0; i<21; i++){
            M2L3avg[i] += M2L3BelAr->at(i)/tree->GetEntries();
            M1L3avg[i] += M1L3BelAr->at(i)/tree->GetEntries();
            M3L3avg[i] += M3L3BelAr->at(i)/tree->GetEntries();
        }
        for (unsigned int i = 0; i<20; i++){
            M2L4avg[i] += M2L4BelAr->at(i)/tree->GetEntries();
            M1L4avg[i] += M1L4BelAr->at(i)/tree->GetEntries();
            M3L4avg[i] += M3L4BelAr->at(i)/tree->GetEntries();
        }
    }
        double picut = 2.1;
        int channels = 0;

        cout<<"List of channels with avg signal above cut"<<endl;
        for (unsigned int i = 0; i<24; i++){
            if (M2L1avg[i]>picut){cout<<"M2L1 "<<i<<endl; channels=channels+1;}
            if (M1L1avg[i]>picut){cout<<"M1L1 "<<i<<endl;channels=channels+1;}
            if (M3L1avg[i]>picut){cout<<"M3L1 "<<i<<endl;channels=channels+1;}
        }
        for (unsigned int i = 0; i<M2L2BelAr->size(); i++){
            if (M2L2avg[i]>picut){cout<<"M2L2 "<<i<<endl;channels=channels+1;}
            if (M1L2avg[i]>picut){cout<<"M1L2 "<<i<<endl;channels=channels+1;}
            if (M3L2avg[i]>picut){cout<<"M3L2 "<<i<<endl;channels=channels+1;}
        }
        for (unsigned int i = 0; i<M2L3BelAr->size(); i++){
            if (M2L3avg[i]>picut){cout<<"M2L3 "<<i<<endl;channels=channels+1;}
            if (M1L3avg[i]>picut){cout<<"M1L3 "<<i<<endl;channels=channels+1;}
            if (M3L3avg[i]>picut){cout<<"M3L3 "<<i<<endl;channels=channels+1;}
        }
        for (unsigned int i = 0; i<M2L4BelAr->size(); i++){
            if (M2L4avg[i]>picut){cout<<"M2L4 "<<i<<endl;channels=channels+1;}
            if (M1L4avg[i]>picut){cout<<"M1L4 "<<i<<endl;channels=channels+1;}
            if (M3L4avg[i]>picut){cout<<"M3L4 "<<i<<endl;channels=channels+1;}
        }
        cout<<"Number of channels above cut: "<<channels<<endl;
}

////////////////////////////////////////////////////////////////////////////////////
void ecalibrate( const double& eenergy, const string& efile ){

    //Initiate objects through all the analysis
    //
    cout<<"ATLHECTB analysis of channels to be selected with e- runs"<<endl;
    auto outputfile = new TFile( "ATLHECTBchannelse.root", "RECREATE" );
    cout<<"---> Analysis at energy(GeV) "<<eenergy<<endl;  
        
    string filename = "Data1/"+efile;
    TFile* file = TFile::Open( filename.c_str(), "READ" );
    TTree* tree = (TTree*)file->Get( "ATLHECTBout" );

    vector<double>* M1L1BelAr = NULL; 
    tree->SetBranchAddress( "M1L1BirkeLayer", &M1L1BelAr );
    vector<double>* M1L2BelAr = NULL; 
    tree->SetBranchAddress( "M1L2BirkeLayer", &M1L2BelAr );
    vector<double>* M1L3BelAr = NULL; 
    tree->SetBranchAddress( "M1L3BirkeLayer", &M1L3BelAr );
    vector<double>* M1L4BelAr = NULL; 
    tree->SetBranchAddress( "M1L4BirkeLayer", &M1L4BelAr );
    vector<double>* M2L1BelAr = NULL; 
    tree->SetBranchAddress( "M2L1BirkeLayer", &M2L1BelAr );
    vector<double>* M2L2BelAr = NULL; 
    tree->SetBranchAddress( "M2L2BirkeLayer", &M2L2BelAr );
    vector<double>* M2L3BelAr = NULL; 
    tree->SetBranchAddress( "M2L3BirkeLayer", &M2L3BelAr );
    vector<double>* M2L4BelAr = NULL; 
    tree->SetBranchAddress( "M2L4BirkeLayer", &M2L4BelAr );
    vector<double>* M3L1BelAr = NULL; 
    tree->SetBranchAddress( "M3L1BirkeLayer", &M3L1BelAr );
    vector<double>* M3L2BelAr = NULL; 
    tree->SetBranchAddress( "M3L2BirkeLayer", &M3L2BelAr );
    vector<double>* M3L3BelAr = NULL; 
    tree->SetBranchAddress( "M3L3BirkeLayer", &M3L3BelAr );
    vector<double>* M3L4BelAr = NULL; 
    tree->SetBranchAddress( "M3L4BirkeLayer", &M3L4BelAr );

    double M1L1avg[24]; memset( M1L1avg, 0., 24*sizeof(double));
    double M2L1avg[24]; memset( M2L1avg, 0., 24*sizeof(double));
    double M3L1avg[24]; memset( M3L1avg, 0., 24*sizeof(double));
         
    double M1L2avg[23]; memset( M1L2avg, 0., 23*sizeof(double));
    double M2L2avg[23]; memset( M2L2avg, 0., 23*sizeof(double));
    double M3L2avg[23]; memset( M3L2avg, 0., 23*sizeof(double));

    double M1L3avg[21]; memset( M1L3avg, 0., 21*sizeof(double));
    double M2L3avg[21]; memset( M2L3avg, 0., 21*sizeof(double));
    double M3L3avg[21]; memset( M3L3avg, 0., 21*sizeof(double));

    double M1L4avg[20]; memset( M1L4avg, 0., 20*sizeof(double));
    double M2L4avg[20]; memset( M2L4avg, 0., 20*sizeof(double));
    double M3L4avg[20]; memset( M3L4avg, 0., 20*sizeof(double));

    //loop over events
    //
    for (unsigned int evtNo = 0; evtNo<tree->GetEntries(); evtNo++){
        tree->GetEntry(evtNo);

        for (unsigned int i = 0; i<24; i++){
            M2L1avg[i] += M2L1BelAr->at(i)/tree->GetEntries();
            M1L1avg[i] += M1L1BelAr->at(i)/tree->GetEntries();
            M3L1avg[i] += M3L1BelAr->at(i)/tree->GetEntries();
        }
        for (unsigned int i = 0; i<23; i++){
            M2L2avg[i] += M2L2BelAr->at(i)/tree->GetEntries();
            M1L2avg[i] += M1L2BelAr->at(i)/tree->GetEntries();
            M3L2avg[i] += M3L2BelAr->at(i)/tree->GetEntries();
        }
        for (unsigned int i = 0; i<21; i++){
            M2L3avg[i] += M2L3BelAr->at(i)/tree->GetEntries();
            M1L3avg[i] += M1L3BelAr->at(i)/tree->GetEntries();
            M3L3avg[i] += M3L3BelAr->at(i)/tree->GetEntries();
        }
        for (unsigned int i = 0; i<20; i++){
            M2L4avg[i] += M2L4BelAr->at(i)/tree->GetEntries();
            M1L4avg[i] += M1L4BelAr->at(i)/tree->GetEntries();
            M3L4avg[i] += M3L4BelAr->at(i)/tree->GetEntries();
        }
    }
        double ecut = 10.0;
        int channels = 0;

        cout<<"List of channels with avg signal above cut"<<endl;
        for (unsigned int i = 0; i<24; i++){
            if (M2L1avg[i]>ecut){cout<<"M2L1 "<<i<<endl;channels=channels+1;}
            if (M1L1avg[i]>ecut){cout<<"M1L1 "<<i<<endl;channels=channels+1;}
            if (M3L1avg[i]>ecut){cout<<"M3L1 "<<i<<endl;channels=channels+1;}
        }
        for (unsigned int i = 0; i<M2L2BelAr->size(); i++){
            if (M2L2avg[i]>ecut){cout<<"M2L2 "<<i<<endl;channels=channels+1;}
            if (M1L2avg[i]>ecut){cout<<"M1L2 "<<i<<endl;channels=channels+1;}
            if (M3L2avg[i]>ecut){cout<<"M3L2 "<<i<<endl;channels=channels+1;}
        }
        for (unsigned int i = 0; i<M2L3BelAr->size(); i++){
            if (M2L3avg[i]>ecut){cout<<"M2L3 "<<i<<endl;channels=channels+1;}
            if (M1L3avg[i]>ecut){cout<<"M1L3 "<<i<<endl;channels=channels+1;}
            if (M3L3avg[i]>ecut){cout<<"M3L3 "<<i<<endl;channels=channels+1;}
        }
        for (unsigned int i = 0; i<M2L4BelAr->size(); i++){
            if (M2L4avg[i]>ecut){cout<<"M2L4 "<<i<<endl;channels=channels+1;}
            if (M1L4avg[i]>ecut){cout<<"M1L4 "<<i<<endl;channels=channels+1;}
            if (M3L4avg[i]>ecut){cout<<"M3L4 "<<i<<endl;channels=channels+1;}
        }
        cout<<"Number of channels above cut: "<<channels<<endl;
        cout<<"take first 7"<<endl;
}

////////////////////////////////////////////////////////////////////////////////////
void ATLHECTBanalysis1_v1p1(){
     
    // Analysis of e- data
    // energies 6, 20, 50, 100, 200 GeV
    //
    vector<double> emenergies = {20.,40.,50.,80.,100.,119.1,147.8};
    vector<string> emfiles;
    for ( unsigned int i=11; i<18; i++ ){
        emfiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }
    emanalysis( emenergies, emfiles );
     
    // Analysis of pi- data
    // energies 6, 20, 50, 100, 200 GeV
    //
    vector<double> pienergies = {20.,30.,40.,50.,60.,80.,100.,120.,150.,180.,200.};
    vector<string> pifiles;
    for ( unsigned int i=0; i<11; i++ ){
        pifiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }
    pianalysis( pienergies, pifiles );

    //Analysis of channels pi
    //
    //picalibrate(180., "ATLHECTBout_Run9.root");
    
    //Analysis of channels e-
    //
    //ecalibrate(147.8,"ATLHECTBout_Run17.root");    

}

//**************************************************
