//**************************************************
// \file ATLHECTBanalysis_v1p0.cc
// \brief: Analysis #0 of ATLHECTB data from v1.0 
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

    //For loop over Runs (energies)
    //
    for (unsigned RunNo = 0; RunNo<emfiles.size(); RunNo++ ){
        cout<<"---> Analysis run # "<<RunNo<<", energy(GeV) "<<emenergies[RunNo]<<endl;  
        
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
                                   nBins, 0., 100. );

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
        } //end for loop events

        energies[RunNo] = emenergies[RunNo];
        ratiomaxtotS[RunNo] = 0;//H1MaxS->GetMean() / H1TotS->GetMean();
        responses[RunNo] = H1Response->GetMean();
        erresponses[RunNo] = H1Response->GetMeanError();

        outputfile->cd();
        H1Leak->Write();
        delete H1Leak;
        H2LeakvsEdep->Write();
        delete H2LeakvsEdep; 
        H1Econt->Write();
        delete H1Econt;
        H1TotS->Write();
        delete H1TotS;
        H1Etot->Write();
        delete H1Etot;
       
        H1TotCutSignal->Write();
        H1Channels->Write();
        H1Signals->Write();
        H1Response->Write();
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
    //G1ratiomaxtotS->Write();
    G1responses->Write();
    delete G1responses;
    //delete G1ratiomaxtotS;
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
void ATLHECTBanalysis_v1p0(){
    
    // Analysis of e- data
    // energies 6, 20, 50, 100, 200 GeV
    //
    vector<double> emenergies = {20.,40.,50.,80.,100.,119.1};
    vector<string> emfiles;
    for ( unsigned int i=0; i<6; i++ ){
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
