//**************************************************
// \file ecalibrate.h
// \brief: Analysis #1 of ATLHECTB v1.3 
//         for e- channels 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 17 September 2021
//**************************************************

#ifndef ecalibrate_H
#define ecalibrate_H

void ecalibrate( const double& eenergy, const string& efile ){

    //Initiate objects through all the analysis
    //
    cout<<"ATLHECTB analysis of channels to be selected with e- runs"<<endl;
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
        double ecut = 9.0;
        int channels = 0;

        cout<<"List of channels with avg signal above cut"<<endl;
        for (unsigned int i = 0; i<24; i++){
            if (M2L1avg[i]>ecut){cout<<"M2L1 "<<i<<" "<<M2L1avg[i]<<endl;channels=channels+1;}
            if (M1L1avg[i]>ecut){cout<<"M1L1 "<<i<<" "<<M1L1avg[i]<<endl;channels=channels+1;}
            if (M3L1avg[i]>ecut){cout<<"M3L1 "<<i<<" "<<M3L1avg[i]<<endl;channels=channels+1;}
        }
        for (unsigned int i = 0; i<M2L2BelAr->size(); i++){
            if (M2L2avg[i]>ecut){cout<<"M2L2 "<<i<<" "<<M2L2avg[i]<<endl;channels=channels+1;}
            if (M1L2avg[i]>ecut){cout<<"M1L2 "<<i<<" "<<M1L2avg[i]<<endl;channels=channels+1;}
            if (M3L2avg[i]>ecut){cout<<"M3L2 "<<i<<" "<<M3L2avg[i]<<endl;channels=channels+1;}
        }
        for (unsigned int i = 0; i<M2L3BelAr->size(); i++){
            if (M2L3avg[i]>ecut){cout<<"M2L3 "<<i<<" "<<M2L3avg[i]<<endl;channels=channels+1;}
            if (M1L3avg[i]>ecut){cout<<"M1L3 "<<i<<" "<<M1L3avg[i]<<endl;channels=channels+1;}
            if (M3L3avg[i]>ecut){cout<<"M3L3 "<<i<<" "<<M3L3avg[i]<<endl;channels=channels+1;}
        }
        for (unsigned int i = 0; i<M2L4BelAr->size(); i++){
            if (M2L4avg[i]>ecut){cout<<"M2L4 "<<i<<" "<<M2L4avg[i]<<endl;channels=channels+1;}
            if (M1L4avg[i]>ecut){cout<<"M1L4 "<<i<<" "<<M1L4avg[i]<<endl;channels=channels+1;}
            if (M3L4avg[i]>ecut){cout<<"M3L4 "<<i<<" "<<M3L4avg[i]<<endl;channels=channels+1;}
        }
        cout<<"Number of channels above cut: "<<channels<<endl;
        cout<<"take first 7 with highest average signal"<<endl;

}

#endif

//**************************************************
