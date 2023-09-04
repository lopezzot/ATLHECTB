//**************************************************
// \file SpectrumAnalyzer.hh
// \brief: Declaration of SpectrumAnalyzer class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 28 August 2023
//**************************************************

// A portable Geant4-based particle spectrum analyzer
// to be used within a Geant4 simulation without affecting it.
// Instead of coding it in the simulation, create a singleton
// and manage its usage with (#ifdef) compiler definition.

#ifdef ATLHECTB_LEAKANALYSIS

#  ifndef SpectrumAnalyzer_h
#    define SpectrumAnalyzer_h

// Includers from Geant4
//
#    include "G4Step.hh"
#    include "G4ThreadLocalSingleton.hh"

// Includers from C++
//
#    include <functional>

class SpectrumAnalyzer
{
    friend class G4ThreadLocalSingleton<SpectrumAnalyzer>;

  public:
    // Return pointer to class instance
    static SpectrumAnalyzer* GetInstance()
    {
      static G4ThreadLocalSingleton<SpectrumAnalyzer> instance{};
      return instance.Instance();
    }

    // Methods
    //
    // Run-wise methods
    void CreateNtupleAndScorer(const G4String scName = "te");
    inline void ClearNtupleID() { ntupleID = 99; }
    // Event-wise methods
    inline void ClearEventFields()
    {
      neutronScore = 0.;
      protonScore = 0., pionScore = 0., gammaScore = 0., electronScore = 0., othersScore = 0.;
    }
    void FillEventFields() const;
    // Step-wise methods
    void Analyze(const G4Step* step);

  private:
    // Members
    //
    // Run-wise members
    G4int ntupleID;
    std::function<G4double(const G4Step* step)> scorer;
    G4String scorerName{};
    // Event-wise members
    G4double neutronScore;
    G4double protonScore;
    G4double pionScore;
    G4double gammaScore;
    G4double electronScore;
    G4double othersScore;

    // Scoring quantities
    inline static G4double GetMomentum(const G4Step* step)
    {
      return step->GetTrack()->GetMomentum().mag();
    };
    inline static G4double GetKE(const G4Step* step)
    {
      return step->GetTrack()->GetKineticEnergy();
    };
    inline static G4double GetTE(const G4Step* step) { return step->GetTrack()->GetTotalEnergy(); };

  private:
    // Private constructor
    SpectrumAnalyzer() = default;

  public:
    SpectrumAnalyzer(SpectrumAnalyzer const&) = delete;
    void operator=(SpectrumAnalyzer const&) = delete;
};

#  endif  // SpectrumAnalyzer_h
#endif  // ATLHECTB_LEAKANALYSIS

//**************************************************
