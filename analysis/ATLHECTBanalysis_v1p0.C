//**************************************************
// \file ATLHECTBanalysis_v1p0.cc
// \brief: Analysis #0 of ATLHECTB data from v1.0 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 4 June 2021
//**************************************************

#include <string>

void emanalysis( const vector<double>& emenergies, const vector<string>& emfiles ){

    //Initiate objects through all the analysis
    //
    auto outputfile = new TFile( "ATLHECTBanalysis.root", "RECREATE" );
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
            new TH2F("e-LeakVsEdep", "e-LeakVsEdep",
                    nBins, 0., emenergies[RunNo],
                    nBins, 0., emenergies[RunNo] );
    
        auto H1Leak = new TH1F("e-ELeak", "e-ELeak",
                              nBins, 0., emenergies[RunNo] );

        auto H1TotS = new TH1F("e-TotS", "e-TotS",
                              nBins, 0., emenergies[RunNo]*100. );

        auto H1Econt = new TH1F("e-Econt", "e-Econt", 
                              nBins, 0., emenergies[RunNo] );

        auto H1MaxS = new TH1F("e-MaxS", "e-MaxS", 
                              nBins, 0., emenergies[RunNo]*100. );

        auto H1IndexMaxSignal = new TH1I("e-IndexMaxSignal_Run",
                                        "e-IndexMaxSignal", nBins, 0, 40);

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
        responses[RunNo] = H1TotS->GetMean() / H1Econt->GetMean();

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
    G1ratiomaxtotS->SetTitle("e-ratiomaxtotS");
    G1responses->SetTitle("e-responses");
    G1ratiomaxtotS->Write();
    G1responses->Write();
    delete G1responses;
    delete G1ratiomaxtotS;
    outputfile->Close();
    delete outputfile;

};

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
        responses[RunNo] = H1TotS->GetXaxis()->GetBinCenter( H1TotS->GetMaximumBin() ) / emenergies[RunNo];
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

void ATLHECTBanalysis_v1p0(){
   /* 
    // Analysis of e- data
    // energies 6, 20, 50, 100, 200 GeV
    //
    vector<double> emenergies = {6.,20.,50.,100.};
    vector<string> emfiles;
    for ( unsigned int i=5; i<9; i++ ){
        emfiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }

    emanalysis( emenergies, emfiles );
*/
    // Analysis of pi- data
    // energies 6, 20, 50, 100, 200 GeV
    //
    vector<double> pienergies = {6.,20.,50.,100.,200.};
    vector<string> pifiles;
    for ( unsigned int i=0; i<5; i++ ){
        pifiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }

    pianalysis( pienergies, pifiles );

}

//**************************************************
