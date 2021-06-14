//**************************************************
// \file ATLHECTBanalysis_v1p0.cc
// \brief: Analysis #1 of ATLHECTB data from v1.0 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 4 June 2021
//**************************************************

#include <string>
#include <array>

void emanalysis( const vector<double>& emenergies, const vector<string>& emfiles ){

    //Initiate objects through all the analysis
    //
    cout<<"ATLHECTB analysis of e- runs"<<endl;
    auto outputfile = new TFile( "ATLHECTBanalysis.root", "RECREATE" );
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
        tree->SetBranchAddress( "M2L4BirkeLayer", &M3L4BelAr );
        
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
                              nBins*20, 0., emenergies[RunNo]*100. );
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
        auto H1Channels = new TH1I("e-Channels", "e-Challens",
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
            H1Sampfraction->Fill((elAr/edep)*100.);

            //MaxSignalIndex = std::max_element(M2L1BelAr->begin(),M2L1BelAr->end())
              //                                -M2L1BelAr->begin();
            
            double addchannels=0;
            int channels = 0;

            for (unsigned int i = 0; i<M2L1BelAr->size(); i++){
                if ( M2L1BelAr->at(i) > 5. ) { 
                    channels += 1;
                    addchannels += M2L1BelAr->at(i);
                    H1Signals->Fill( M2L1BelAr->at(i)) ;
                }
            }
            for (unsigned int i = 0; i<M2L2BelAr->size(); i++){
                if ( M2L2BelAr->at(i) > 5. ) { 
                    channels += 1;
                    addchannels+= M2L2BelAr->at(i);
                    H1Signals->Fill( M2L2BelAr->at(i)); 
                }
            }
            
            //H1MaxS->Fill(M2L1BelAr->at(MaxSignalIndex));
            H1TotCutSignal->Fill( addchannels );
            H1Channels->Fill(channels);
            H1Response->Fill( addchannels / (edep/1000.) ); 
            // average response 44.8059 a.u./GeV
            H1Recenergy->Fill( addchannels / 44.8059 ); 
        } //end for loop events

        energies[RunNo] = emenergies[RunNo];
        ratiomaxtotS[RunNo] = 0;//H1MaxS->GetMean() / H1TotS->GetMean();
        responses[RunNo] = H1Response->GetMean();
        erresponses[RunNo] = H1Response->GetMeanError();
        Sampfraction[RunNo] = H1Sampfraction->GetMean();
        ersampfraction[RunNo] = H1Sampfraction->GetMeanError();
        
        H1Recenergy->Fit("gaus","Q");
        recenergies[RunNo] = H1Recenergy->GetFunction("gaus")->GetParameter(1);
        errecenergies[RunNo] = H1Recenergy->GetFunction("gaus")->GetParError(1);
        double res =(H1Recenergy->GetFunction("gaus")->GetParameter(2)/H1Recenergy->GetFunction("gaus")->GetParameter(1))*sqrt(energies[RunNo])*100.;
        energyresolution[RunNo] = res;
        erenergyresolution[RunNo] = (H1Recenergy->GetFunction("gaus")->GetParError(2)/H1Recenergy->GetFunction("gaus")->GetParameter(2) + H1Recenergy->GetFunction("gaus")->GetParError(1)/H1Recenergy->GetFunction("gaus")->GetParameter(1))*res;

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
            LeakvsEdeplegend->AddEntry(H2LeakvsEdep,"#splitline{ATLHECTB v1.0 }{Geant4.10.7.p01 FTFP_BERT }","ep");
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
    auto G1responses = new TGraphErrors( emenergies.size(), energies, responses, 
                                         zeros, erresponses );
    G1responses->GetYaxis()->SetRangeUser(44.2,45.8);
    G1responses->SetMarkerStyle(8); 
    //G1ratiomaxtotS->SetName("e-ratiomaxtotS");
    //G1ratiomaxtotS->SetTitle("e-ratiomaxtotS");
    G1responses->SetName("e-responses");
    G1responses->SetTitle("e-responses");
    G1responses->GetYaxis()->SetTitle("Response [a.u./GeV]");
    G1responses->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    //G1ratiomaxtotS->Write();
    G1responses->Write();
    delete G1responses;
    //delete G1ratiomaxtotS;

    auto G1Sampfraction = new TGraphErrors( emenergies.size(), energies, Sampfraction, zeros, ersampfraction );
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
    Sampfractionlegend->AddEntry(G1Sampfraction,"#splitline{ATLHECTB v1.0 }{Geant4.10.7.p01 FTFP_BERT }","ep");
    Sampfractionlegend->Draw("same");
    C1Sampfraction->Write();
    delete C1Sampfraction;
    delete G1Sampfraction;

    auto G1recenergy = new TGraphErrors( emenergies.size(), energies, recenergies, zeros, errecenergies );
    G1recenergy->SetMarkerStyle(8);
    G1recenergy->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1recenergy->GetYaxis()->SetTitle("Energy [GeV]");
    G1recenergy->SetTitle("e-reconstructedenergy");
    G1recenergy->SetName("e-reconstructedenergy");
    G1recenergy->Write();
    delete G1recenergy;

    auto G1energyresolution = new TGraphErrors( emenergies.size(), energies, energyresolution, zeros, erenergyresolution );
    G1energyresolution->SetMarkerStyle(8);
    G1energyresolution->SetMarkerColor(kRed);
    G1energyresolution->SetLineColor(kRed);
    G1energyresolution->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1energyresolution->GetYaxis()->SetTitle("#sigma_{0}/E_{0} #sqrt{E_{Beam}} [% #sqrt{GeV}]");
    G1energyresolution->GetYaxis()->SetRangeUser(19.,25.);
    G1energyresolution->GetXaxis()->SetRangeUser(0.,170.);
    G1energyresolution->SetTitle("e-energyresolution");
    G1energyresolution->SetName("e-energyresolution");
    auto F1energyres = new TF1("e-energyres","[0]",20.,150.);
    F1energyres->SetLineWidth(3);
    F1energyres->SetLineColor(kRed);
    G1energyresolution->Fit(F1energyres, "QR");
    cout<<"->Average a term in resolution "<<F1energyres->GetParameter(0)<<" +- "<<F1energyres->GetParError(0)<<" % GeV^0.5"<<endl;
    F1energyres->Write();
    G1energyresolution->Write();

    double ATLASenres[7] = {22.2, 21.6, 21.5, 22.30, 22.0, 21.2, 21.3};
    double erATLASenres[7] = {22.57-22.2, 21.89-21.6, 21.78-21.5, 22.6-22.30, 22.32-22.0, 21.46-21.2, 21.57-21.3};
    auto G1ATLASenres = new TGraphErrors( emenergies.size(), energies, ATLASenres, zeros, erATLASenres );
    G1ATLASenres->SetMarkerStyle(8);
    G1ATLASenres->GetXaxis()->SetTitle("<E_{Beam}> [GeV]");
    G1ATLASenres->GetYaxis()->SetTitle("#sigma_{0}/E_{0} #sqrt{E_{Beam}} [% #sqrt{GeV}]");
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
/*
void pianalysis( const vector<double>& emenergies, const vector<string>& emfiles ){

    //Initiate objects through all the analysis
    //
    auto outputfile = new TFile( "ATLHECTBanalysis2.root", "RECREATE" );
    double energies[emenergies.size()];
    double ratiomaxtotS[emenergies.size()];
    double responses[emenergies.size()];

    //For loop over Runs (energies)
    //
    for (unsigned RunNo = 0; RunNo<emfiles.size(); RunNo++ ){
        
        //Initiate objects through single Run
        //   
        string filename = "../../"+emfiles[RunNo];
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
        vector<double>* M1BelAr = NULL; 
        tree->SetBranchAddress( "M1BirkeSlice", &M1BelAr );
        vector<double>* M2BelAr = NULL; 
        tree->SetBranchAddress( "M2BirkeSlice", &M2BelAr );
        vector<double>* M3BelAr = NULL; 
        tree->SetBranchAddress( "M3BirkeSlice", &M3BelAr );

        int nBins = 100;
        int MaxSignalIndex;

        auto H2LeakvsEdep =
            new TH2F("piLeakVsEdep", "piLeakVsEdep",
                    nBins, 0., emenergies[RunNo],
                    nBins, 0., emenergies[RunNo] );
    
        auto H1Leak = new TH1F("piELeak", "piELeak",
                              nBins, 0., emenergies[RunNo] );

        auto H1TotS = new TH1F("piTotS", "piTotS",
                              nBins, 0., emenergies[RunNo]*100. );

        auto H1Econt = new TH1F("piEcont", "piEcont", 
                              nBins, 0., emenergies[RunNo]);

        auto H1MaxS = new TH1F("piMaxS", "piMaxS", 
                              nBins, 0., emenergies[RunNo]*100. );

        auto H1IndexMaxSignal = new TH1I("piIndexMaxSignal_Run",
                                        "piIndexMaxSignal", nBins, 0, 40);

        //For loop over events
        //
        for ( unsigned int eventNo = 0; eventNo<tree->GetEntries(); eventNo++ ){
            tree->GetEntry(eventNo);
            MaxSignalIndex = std::max_element(M2BelAr->begin(),M2BelAr->end())
                                              -M2BelAr->begin();

            H1IndexMaxSignal->Fill(MaxSignalIndex);
            H1MaxS->Fill(M2BelAr->at(MaxSignalIndex));
            H1Leak->Fill((lenergy+cenergy)/1000.);
            H2LeakvsEdep->Fill(lenergy/1000., cenergy/1000.);
            H1TotS->Fill(BelAr);
            H1Econt->Fill(emenergies[RunNo]-lenergy/1000.-cenergy/1000.);
        }

        energies[RunNo] = emenergies[RunNo];
        ratiomaxtotS[RunNo] = H1MaxS->GetMean() / H1TotS->GetMean();
        responses[RunNo] = H1TotS->GetXaxis()->
            GetBinCenter( H1TotS->GetMaximumBin() ) / emenergies[RunNo];
        outputfile->cd();
        H1Econt->Write();
        delete H1Econt;
        H1MaxS->Write();
        delete H1MaxS;
        H1TotS->Write();
        delete H1TotS;
        H1IndexMaxSignal->Write();
        delete H1IndexMaxSignal;
        H1Leak->Write();
        delete H1Leak;
        H2LeakvsEdep->Write();
        delete H2LeakvsEdep;
    }

    outputfile->cd();
    auto G1ratiomaxtotS = new TGraph( emenergies.size(), energies, ratiomaxtotS ); 
    auto G1responses = new TGraph( emenergies.size(), energies, responses );
    G1ratiomaxtotS->SetTitle("piratiomaxtotS");
    G1responses->SetTitle("piresponses");
    G1ratiomaxtotS->Write();
    G1responses->Write();
    delete G1responses;
    delete G1ratiomaxtotS;
    outputfile->Close();
    delete outputfile;

};
*/
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
    /*
    // Analysis of pi- data
    // energies 6, 20, 50, 100, 200 GeV
    //
    vector<double> pienergies = {6.,20.,50.,100.,200.};
    vector<string> pifiles;
    for ( unsigned int i=0; i<5; i++ ){
        pifiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }

    pianalysis( pienergies, pifiles );
    */
}

//**************************************************
