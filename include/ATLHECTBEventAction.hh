//**************************************************
// \file ATLHECTBEventAction.hh
// \brief: Definition of ATLHECTBEventAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Prevent including header multiple times
//
#ifndef ATLHECTBEventAction_h
#  define ATLHECTBEventAction_h 1

#  include <vector>

// Includers from Gean4
//
#  include "G4UserEventAction.hh"
#  include "globals.hh"

class ATLHECTBEventAction : public G4UserEventAction
{
  public:
    ATLHECTBEventAction();
    virtual ~ATLHECTBEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void SavePDGID(G4int ID);
    void Savevertexkenergy(G4double kenergy);
    void Addeleak(G4double eleak);
    void Addedep(G4double stepedep);
    void Addecryostat(G4double stepecryostat);
    void AddelAr(G4double stepelAr);
    void AddBirkelAr(G4double stepBirkelAr);
    void AddL1BirkeLayer(G4int moduleNo, G4int index, G4double stepBirkeSlice);
    void AddL2BirkeLayer(G4int moduleNo, G4int index, G4double stepBirkeSlice);
    void AddL3BirkeLayer(G4int moduleNo, G4int index, G4double stepBirkeSlice);
    void AddL4BirkeLayer(G4int moduleNo, G4int index, G4double stepBirkeSlice);

    std::vector<G4double>& GetM1L1BirkeLayer() { return M1L1BirkeLayer; };
    std::vector<G4double>& GetM2L1BirkeLayer() { return M2L1BirkeLayer; };
    std::vector<G4double>& GetM3L1BirkeLayer() { return M3L1BirkeLayer; };
    std::vector<G4double>& GetM1L2BirkeLayer() { return M1L2BirkeLayer; };
    std::vector<G4double>& GetM2L2BirkeLayer() { return M2L2BirkeLayer; };
    std::vector<G4double>& GetM3L2BirkeLayer() { return M3L2BirkeLayer; };
    std::vector<G4double>& GetM1L3BirkeLayer() { return M1L3BirkeLayer; };
    std::vector<G4double>& GetM2L3BirkeLayer() { return M2L3BirkeLayer; };
    std::vector<G4double>& GetM3L3BirkeLayer() { return M3L3BirkeLayer; };
    std::vector<G4double>& GetM1L4BirkeLayer() { return M1L4BirkeLayer; };
    std::vector<G4double>& GetM2L4BirkeLayer() { return M2L4BirkeLayer; };
    std::vector<G4double>& GetM3L4BirkeLayer() { return M3L4BirkeLayer; };

  private:
    G4int PDGID;  // primary particle PDGID
    G4double vertexkenergy;  // primary particle kenergy (MeV)
    G4double eleakage;  // out of world leakage (MeV)
    G4double edep;  // energy deposited in HEC (MeV)
    G4double ecryostat;  // energy deposited in cryostat (MeV)
    G4double elAr;  // energy deposited in lAr (MeV)
    G4double BirkelAr;  // Birk corrected elAr (a.u.)
                        //  signal from <75*ns hits

    std::vector<G4double> M1L1BirkeLayer;
    std::vector<G4double> M2L1BirkeLayer;
    std::vector<G4double> M3L1BirkeLayer;
    std::vector<G4double> M1L2BirkeLayer;
    std::vector<G4double> M2L2BirkeLayer;
    std::vector<G4double> M3L2BirkeLayer;
    std::vector<G4double> M1L3BirkeLayer;
    std::vector<G4double> M2L3BirkeLayer;
    std::vector<G4double> M3L3BirkeLayer;
    std::vector<G4double> M1L4BirkeLayer;
    std::vector<G4double> M2L4BirkeLayer;
    std::vector<G4double> M3L4BirkeLayer;
};

inline void ATLHECTBEventAction::SavePDGID(G4int ID)
{
  PDGID = ID;
}

inline void ATLHECTBEventAction::Savevertexkenergy(G4double kenergy)
{
  vertexkenergy = kenergy;
}

inline void ATLHECTBEventAction::Addeleak(G4double eleak)
{
  eleakage += eleak;
}

inline void ATLHECTBEventAction::Addedep(G4double stepedep)
{
  edep += stepedep;
}

inline void ATLHECTBEventAction::Addecryostat(G4double stepecryostat)
{
  ecryostat += stepecryostat;
}

inline void ATLHECTBEventAction::AddelAr(G4double stepelAr)
{
  elAr += stepelAr;
}

inline void ATLHECTBEventAction::AddBirkelAr(G4double stepBirkelAr)
{
  BirkelAr += stepBirkelAr;
}

inline void ATLHECTBEventAction::AddL1BirkeLayer(G4int moduleNo, G4int index,
                                                 G4double stepBirkeSlice)
{
  if (moduleNo == 101) {
    M1L1BirkeLayer[index] += stepBirkeSlice;
  }
  if (moduleNo == 102) {
    M2L1BirkeLayer[index] += stepBirkeSlice;
  }
  if (moduleNo == 103) {
    M3L1BirkeLayer[index] += stepBirkeSlice;
  }
}

inline void ATLHECTBEventAction::AddL2BirkeLayer(G4int moduleNo, G4int index,
                                                 G4double stepBirkeSlice)
{
  if (moduleNo == 101) {
    M1L2BirkeLayer[index] += stepBirkeSlice;
  }
  if (moduleNo == 102) {
    M2L2BirkeLayer[index] += stepBirkeSlice;
  }
  if (moduleNo == 103) {
    M3L2BirkeLayer[index] += stepBirkeSlice;
  }
}

inline void ATLHECTBEventAction::AddL3BirkeLayer(G4int moduleNo, G4int index,
                                                 G4double stepBirkeSlice)
{
  if (moduleNo == 101) {
    M1L3BirkeLayer[index] += stepBirkeSlice;
  }
  if (moduleNo == 102) {
    M2L3BirkeLayer[index] += stepBirkeSlice;
  }
  if (moduleNo == 103) {
    M3L3BirkeLayer[index] += stepBirkeSlice;
  }
}

inline void ATLHECTBEventAction::AddL4BirkeLayer(G4int moduleNo, G4int index,
                                                 G4double stepBirkeSlice)
{
  if (moduleNo == 101) {
    M1L4BirkeLayer[index] += stepBirkeSlice;
  }
  if (moduleNo == 102) {
    M2L4BirkeLayer[index] += stepBirkeSlice;
  }
  if (moduleNo == 103) {
    M3L4BirkeLayer[index] += stepBirkeSlice;
  }
}

#endif

//**************************************************
