//**************************************************
// \file ATLHECTBanalysis_v1p0.cc
// \brief: Analysis #1 of ATLHECTB data from v1.0 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 4 June 2021
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
                if ( i== 4 || i== 5 ) { 
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
            H1Recenergy->Fill( addchannels / 44.7411 ); 
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
                    "#splitline{ATLHECTB v1.0 }{Geant4.10.7.p01 FTFP_BERT }","ep");
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
            "#splitline{ATLHECTB v1.0 }{Geant4.10.7.p01 FTFP_BERT }","ep");
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
    G1ATLASenres->SetTitle("e-ATLASenergyresolution");
    G1ATLASenres->SetName("e-ATLASenergyresolution");
    auto F1ATLASenergyres = new TF1("e-ATLASenergyres","21.68",20.,150.);
    F1ATLASenergyres->SetLineWidth(3);
    F1ATLASenergyres->SetLineColor(kBlack);
    F1ATLASenergyres->Write();
    G1ATLASenres->Write();

    //Create canvas e- energy resolution comparison
    //
    auto C1eneres = new TCanvas("e-Canvas_eneres", "", 600, 600);
    G1ATLASenres->Draw("AP");
    G1energyresolution->Draw("P SAME");
    F1ATLASenergyres->Draw("L SAME");
    auto legend = new TLegend(0.15,0.7,0.58,0.89);
    legend->AddEntry(G1ATLASenres,
        "#splitline{ATLAS HEC }{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}", "ep");
    legend->AddEntry(G1energyresolution,
            "#splitline{ATLHECTB v1.0 }{Geant4.10.7.p01 FTFP_BERT }","ep");
    legend->SetLineWidth(0);
    legend->Draw("same");
    C1eneres->Write();
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
    //double recenergies[pienergies.size()];
    //double errecenergies[pienergies.size()];
    //double energyresolution[pienergies.size()];
    //double erenergyresolution[pienergies.size()];
    double F1[pienergies.size()];
    double erF1[pienergies.size()];
    double F2[pienergies.size()];
    double erF2[pienergies.size()];
    double F3[pienergies.size()];
    double erF3[pienergies.size()];
    double F4[pienergies.size()];
    double erF4[pienergies.size()];
    
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
                                   2*120, 0., 1.2 );
        //response no Birks
        //
        auto H1ResponsenoB = new TH1F("pi-ResnoB","pi-ResnoB",
                                      nBins,0.,1.2 );
        //reconstructed energy
        //
        //auto H1Recenergy = new TH1F( "pi-Reconstructedenergy",
        //        "pi-Reconstructedenergy", nBins*10, 0., 200. );

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
            H1Response->Fill( (addchannels / pienergies[RunNo])/44.7411 ); //pi/e
            H1ResponsenoB->Fill( (elAr / pienergies[RunNo])/44.7411 ); //pi/e
            // average response xxx a.u./GeV
            //H1Recenergy->Fill( addchannels / 44.8059 ); 
        } //end for loop events

        energies[RunNo] = pienergies[RunNo];

        responses[RunNo] = H1Response->GetMean();
        erresponses[RunNo] = 10.*H1Response->GetMeanError();
        
        //H1Recenergy->Fit("gaus","Q");
        //recenergies[RunNo] = H1Recenergy->GetFunction("gaus")->GetParameter(1);
        //errecenergies[RunNo] = H1Recenergy->GetFunction("gaus")->GetParError(1);
        //double res =(H1Recenergy->GetFunction("gaus")->GetParameter(2)/H1Recenergy->GetFunction("gaus")->GetParameter(1))*sqrt(energies[RunNo])*100.;
        //energyresolution[RunNo] = res;
        //erenergyresolution[RunNo] = (H1Recenergy->GetFunction("gaus")->GetParError(2)/H1Recenergy->GetFunction("gaus")->GetParameter(2) + H1Recenergy->GetFunction("gaus")->GetParError(1)/H1Recenergy->GetFunction("gaus")->GetParameter(1))*res;

        F1[RunNo] = H1F1->GetMean();
        erF1[RunNo] = H1F1->GetMeanError();
        F2[RunNo] = H1F2->GetMean();
        erF2[RunNo] = H1F2->GetMeanError();
        F3[RunNo] = H1F3->GetMean();
        erF3[RunNo] = H1F3->GetMeanError();
        F4[RunNo] = H1F4->GetMean();
        erF4[RunNo] = H1F4->GetMeanError();

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
        H1ResponsenoB->Write();
        //H1Recenergy->Write();
    }

    // Finalize objects over multiple runs
    //
    outputfile->cd();
    //auto G1ratiomaxtotS = new TGraph( emenergies.size(), energies, ratiomaxtotS ); 
    auto G1responses = new TGraphErrors( pienergies.size(), energies, responses, 
                                         zeros, erresponses );
    G1responses->GetYaxis()->SetRangeUser(0.75,0.9);
    G1responses->SetMarkerStyle(8); 
    //G1ratiomaxtotS->SetName("pi-ratiomaxtotS");
    //G1ratiomaxtotS->SetTitle("pi-ratiomaxtotS");
    G1responses->SetName("pi-responses");
    G1responses->SetTitle("pi-responses");
    G1responses->GetYaxis()->SetTitle("#pi / e");
    G1responses->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    //G1ratiomaxtotS->Write();

    //Create canvas pi- response
    //
    auto C1piresponse = new TCanvas("pi-Canvas_response", "", 600, 600);
    G1responses->Draw("AP");
    gPad->SetLeftMargin(0.15);
    auto legend = new TLegend(0.18,0.7,0.61,0.89);
    legend->AddEntry(G1responses,
    "#splitline{ATLHECTB v1.0 }{#splitline{Geant4.10.7.p01 FTFP_BERT }{w/ Birks Law}}",
    "ep");
    legend->SetLineWidth(0);
    legend->Draw("same");
    C1piresponse->Write();
    delete C1piresponse;
    G1responses->Write();
    delete G1responses;
    //delete G1ratiomaxtotS;

    auto G1F1 = new TGraphErrors( pienergies.size(), energies,                                                             F1, zeros, erF1 );
    G1F1->GetYaxis()->SetRangeUser(0.2,0.4);
    G1F1->SetMarkerStyle(8); 
    G1F1->SetName("pi-F1");
    G1F1->SetTitle("pi-F1");
    G1F1->GetYaxis()->SetTitle("F");
    G1F1->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1F1->Write();
    delete G1F1;
    auto G1F2 = new TGraphErrors( pienergies.size(), energies,                                                             F2, zeros, erF2 );
    G1F2->GetYaxis()->SetRangeUser(0.50,0.57);
    G1F2->SetMarkerStyle(8); 
    G1F2->SetName("pi-F2");
    G1F2->SetTitle("pi-F2");
    G1F2->GetYaxis()->SetTitle("F");
    G1F2->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1F2->Write();
    delete G1F2;
    auto G1F3 = new TGraphErrors( pienergies.size(), energies,                                                             F3, zeros, erF3 );
    G1F3->GetYaxis()->SetRangeUser(0.08,0.22);
    G1F3->SetMarkerStyle(8); 
    G1F3->SetName("pi-F3");
    G1F3->SetTitle("pi-F3");
    G1F3->GetYaxis()->SetTitle("F");
    G1F3->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1F3->Write();
    delete G1F3;
    auto G1F4 = new TGraphErrors( pienergies.size(), energies,                                                             F4, zeros, erF4 );
    G1F4->GetYaxis()->SetRangeUser(0.,0.06);
    G1F4->SetMarkerStyle(8); 
    G1F4->SetName("pi-F4");
    G1F4->SetTitle("pi-F4");
    G1F4->GetYaxis()->SetTitle("F");
    G1F4->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1F4->Write();
    delete G1F4;
   /*
    auto G1recenergy = new TGraphErrors( pienergies.size(), energies, recenergies, zeros, errecenergies );
    G1recenergy->SetMarkerStyle(8);
    G1recenergy->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1recenergy->GetYaxis()->SetTitle("Energy [GeV]");
    G1recenergy->SetTitle("pi-reconstructedenergy");
    G1recenergy->SetName("pi-reconstructedenergy");
    G1recenergy->Write();
    delete G1recenergy;
    
    auto G1energyresolution = new TGraphErrors( pienergies.size(), energies, energyresolution, zeros, erenergyresolution );
    G1energyresolution->SetMarkerStyle(8);
    G1energyresolution->SetMarkerColor(kRed);
    G1energyresolution->SetLineColor(kRed);
    G1energyresolution->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1energyresolution->GetYaxis()->SetTitle("#sigma_{0}/E_{0} #sqrt{E_{Beam}} [% #sqrt{GeV}]");
    G1energyresolution->GetYaxis()->SetRangeUser(19.,25.);
    G1energyresolution->GetXaxis()->SetRangeUser(0.,170.);
    G1energyresolution->SetTitle("pi-energyresolution");
    G1energyresolution->SetName("pi-energyresolution");
    auto F1energyres = new TF1("pi-energyres","[0]",20.,150.);
    F1energyres->SetLineWidth(3);
    F1energyres->SetLineColor(kRed);
    G1energyresolution->Fit(F1energyres, "QR");
    cout<<"->Average a term in resolution "<<F1energyres->GetParameter(0)<<" +- "<<F1energyres->GetParError(0)<<" % GeV^0.5"<<endl;
    F1energyres->Write();
    G1energyresolution->Write();
    
    double ATLASenres[7] = {22.2, 21.6, 21.5, 22.30, 22.0, 21.2, 21.3};
    double erATLASenres[7] = {22.57-22.2, 21.89-21.6, 21.78-21.5, 22.6-22.30, 22.32-22.0, 21.46-21.2, 21.57-21.3};
    auto G1ATLASenres = new TGraphErrors( pienergies.size(), energies, ATLASenres, zeros, erATLASenres );
    G1ATLASenres->SetMarkerStyle(8);
    G1ATLASenres->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1ATLASenres->GetYaxis()->SetTitle("#sigma_{0}/E_{0} #sqrt{E_{Beam}} [% #sqrt{GeV}]");
    G1ATLASenres->GetYaxis()->SetRangeUser(19.,25.);
    G1ATLASenres->GetXaxis()->SetRangeUser(0.,170.);
    G1ATLASenres->SetTitle("pi-ATLASenergyresolution");
    G1ATLASenres->SetName("pi-ATLASenergyresolution");
    auto F1ATLASenergyres = new TF1("pi-ATLASenergyres","21.68",20.,150.);
    F1ATLASenergyres->SetLineWidth(3);
    F1ATLASenergyres->SetLineColor(kBlack);
    F1ATLASenergyres->Write();
    G1ATLASenres->Write();
    */
    /*
    //Create canvas pi- energy resolution comparison
    //
    auto C1eneres = new TCanvas("pi-Canvas_eneres", "", 600, 600);
    G1ATLASenres->Draw("AP");
    G1energyresolution->Draw("P SAME");
    F1ATLASenergyres->Draw("L SAME");
    auto legend = new TLegend(0.15,0.7,0.58,0.89);
    legend->AddEntry(G1ATLASenres,"#splitline{ATLAS HEC }{#splitline{Test beam 2000/2001}{ATL-COM-LARG-2021-005}}","ep");
    legend->AddEntry(G1energyresolution,"#splitline{ATLHECTB v1.0 }{Geant4.10.7.p01 FTFP_BERT }","ep");
    legend->SetLineWidth(0);
    legend->Draw("same");
    C1eneres->Write();
    delete C1eneres;

    delete F1ATLASenergyres;
    delete F1energyres;
    delete G1energyresolution;
    delete G1ATLASenres;
    */
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
        double ecut = 28.;
        int channels = 0;

        cout<<"List of channels with avg signal above cut"<<endl;
        for (unsigned int i = 0; i<24; i++){
            if (M2L1avg[i]>ecut){cout<<"M2L1 "<<i<<endl; channels=channels+1;}
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
}

////////////////////////////////////////////////////////////////////////////////////
void ATLHECTBanalysis1_v1p0(){
     
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
    //pianalysis( pienergies, pifiles );

    //Analysis of channels pi
    //
    //picalibrate(180., "ATLHECTBout_Run9.root");
    
    //Analysis of channels e-
    //
    //ecalibrate(147.8,"ATLHECTBout_Run17.root");    

}

//**************************************************
