//**************************************************
// \file picalibrate.h
// \brief: Analysis #1 of ATLHECTB v2.6
//         for pi- channels 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 10 July 2023
//**************************************************

#ifndef picalibrate_H
#define picalibrate_H

void picalibrate( const double& pienergy, const string& pifile ){

    //Initiate objects through all the analysis
    //
    cout<<"ATLHECTB analysis of channels to be selected with pi- runs"<<endl;
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
    double el1 = 0.0;
    double el2 = 0.0;
    double el3 = 0.0;
    double el4 = 0.0;
    const double SF = 44.9;

    cout<<"List of channels with avg signal above cut"<<endl;
    cout<<"Layer 1"<<endl;
    for (unsigned int i = 0; i<M2L1BelAr->size(); i++){
        if (M2L1avg[i]>picut){cout<<"M2L1 "<<i<<" "<<M2L1avg[i]/SF<<endl;channels=channels+1; el1+=M2L1avg[i]/SF;}
        if (M1L1avg[i]>picut){cout<<"M1L1 "<<i<<" "<<M1L1avg[i]/SF<<endl;channels=channels+1; el1+=M1L1avg[i]/SF;}
        if (M3L1avg[i]>picut){cout<<"M3L1 "<<i<<" "<<M3L1avg[i]/SF<<endl;channels=channels+1; el1+=M3L1avg[i]/SF;}
    }
    cout<<"energy l1: "<<el1<<" diff "<<(36.41-el1)/36.41<<endl;
    cout<<"Layer2 "<<endl;
    for (unsigned int i = 0; i<M2L2BelAr->size(); i++){
        if (M2L2avg[i]>picut){cout<<"M2L2 "<<i<<" "<<M2L2avg[i]/SF<<endl;channels=channels+1; el2+=M2L2avg[i]/SF;}
        if (M1L2avg[i]>picut){cout<<"M1L2 "<<i<<" "<<M1L2avg[i]/SF<<endl;channels=channels+1; el2+=M1L2avg[i]/SF;}
        if (M3L2avg[i]>picut){cout<<"M3L2 "<<i<<" "<<M3L2avg[i]/SF<<endl;channels=channels+1; el2+=M3L2avg[i]/SF;}
    }
    cout<<"energy l2: "<<el2<<" diff "<<(82.73-el2)/82.73<<endl;
    cout<<"Layer 3"<<endl;
    for (unsigned int i = 0; i<M2L3BelAr->size(); i++){
        if (M2L3avg[i]>(picut/2.)){cout<<"M2L3 "<<i<<" "<<2.*M2L3avg[i]/SF<<endl;channels=channels+1; el3+=2.*M2L3avg[i]/SF;}
        if (M1L3avg[i]>(picut/2.)){cout<<"M1L3 "<<i<<" "<<2.*M1L3avg[i]/SF<<endl;channels=channels+1; el3+=2.*M1L3avg[i]/SF;}
        if (M3L3avg[i]>(picut/2.)){cout<<"M3L3 "<<i<<" "<<2.*M3L3avg[i]/SF<<endl;channels=channels+1; el3+=2.*M3L3avg[i]/SF;}
    }
    cout<<"energy l3: "<<el3<<" diff "<<(25.37-el3)/25.37<<endl;
    cout<<"Layer 4"<<endl;
    for (unsigned int i = 0; i<M2L4BelAr->size(); i++){
        if (M2L4avg[i]>(picut/2.)){cout<<"M2L4 "<<i<<" "<<2.*M2L4avg[i]/SF<<endl;channels=channels+1; el4+=2.*M2L4avg[i]/SF;}
        if (M1L4avg[i]>(picut/2.)){cout<<"M1L4 "<<i<<" "<<2.*M1L4avg[i]/SF<<endl;channels=channels+1; el4+=2.*M1L4avg[i]/SF;}
        if (M3L4avg[i]>(picut/2.)){cout<<"M3L4 "<<i<<" "<<2.*M3L4avg[i]/SF<<endl;channels=channels+1; el4+=2.*M3L4avg[i]/SF;}
    }
    cout<<"energy l4: "<<el4<<" diff "<<(5.73-el4)/5.73<<endl;
    cout<<"Number of channels above cut: "<<channels<<endl;

        /*cout<<"List of channels with avg signal"<<endl;
        cout<<"Layer 1"<<endl;
        double l1 = 0.;
        cout<<"-->Module 2"<<endl;
        for (unsigned int i = 0; i<24; i++){
            cout<<"-->M2L1 "<<i<<" "<<M2L1avg[i]/44.9513<<endl;
            l1+=M2L1avg[i]/44.9513;
        }
        cout<<"-->Module 1"<<endl;
        for (unsigned int i = 0; i<24; i++){
            cout<<"-->M1L1 "<<i<<" "<<M1L1avg[i]/44.9513<<endl;
            l1+=M1L1avg[i]/44.9513;
        }
        cout<<"-->Module 3"<<endl;
        for (unsigned int i = 0; i<24; i++){
            cout<<"-->M3L1 "<<i<<" "<<M3L1avg[i]/44.9513<<endl;
            l1+=M3L1avg[i]/44.9513;
        }
        cout<<"energy L1: "<<l1<<endl;
        cout<<"Layer 2"<<endl;
        double l2 = 0.;
        cout<<"-->Module 2"<<endl;
        for (unsigned int i = 0; i<M2L2BelAr->size(); i++){
            cout<<"-->M2L2 "<<i<<" "<<M2L2avg[i]/44.9513<<endl;
            l2+=M2L2avg[i]/44.9513;
        }
        cout<<"-->Module 1"<<endl;
        for (unsigned int i = 0; i<M2L2BelAr->size(); i++){
            cout<<"-->M1L2 "<<i<<" "<<M1L2avg[i]/44.9513<<endl;
            l2+=M1L2avg[i]/44.9513;
        }
        cout<<"-->Module 3"<<endl;
        for (unsigned int i = 0; i<M2L2BelAr->size(); i++){
            cout<<"-->M3L2 "<<i<<" "<<M3L2avg[i]/44.9513<<endl;
            l2+=M3L2avg[i]/44.9513;
        }
        cout<<"energy L2: "<<l2<<endl;
        cout<<"Layer 3"<<endl;
        double l3 = 0;
        for (unsigned int i = 0; i<M2L3BelAr->size(); i++){
            cout<<"M2L3 "<<i<<" "<<2.*M2L3avg[i]/44.9513<<endl;
            l3+=2.*M2L3avg[i]/44.9513;
            cout<<"M1L3 "<<i<<" "<<2.*M1L3avg[i]/44.9513<<endl;
            l3+=2.*M1L3avg[i]/44.9513;
            cout<<"M3L3 "<<i<<" "<<2.*M3L3avg[i]/44.9513<<endl;
            l3+=2.*M3L3avg[i]/44.9513;
        }
        cout<<"energy L3: "<<l3<<endl;
        cout<<"Layer 4"<<endl;
        double l4 = 0.;
        cout<<"-->Module 2"<<endl;
        for (unsigned int i = 0; i<M2L4BelAr->size(); i++){
            cout<<"-->M2L4 "<<i<<" "<<2.*M2L4avg[i]/44.9513<<endl;
            l4+=2.*M2L4avg[i]/44.9513;
        }
        cout<<"-->Module 1"<<endl;
        for (unsigned int i = 0; i<M2L4BelAr->size(); i++){
            cout<<"-->M1L4 "<<i<<" "<<2.*M1L4avg[i]/44.9513<<endl;
            l4+=2.*M1L4avg[i]/44.9513;
        }
        cout<<"-->Module 3"<<endl;
        for (unsigned int i = 0; i<M2L4BelAr->size(); i++){
            cout<<"-->M3L4 "<<i<<" "<<2.*M3L4avg[i]/44.9513<<endl;
            l4+=2.*M3L4avg[i]/44.9513;
        }
        cout<<"energy L4: "<<l4<<endl;*/
 
}

#endif

//**************************************************
