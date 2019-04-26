#ifndef GasModelParameters_hh
#define GasModelParameters_hh

#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include <map>

class HeedDeltaElectronModel;
class HeedNewTrackModel;
class GasModelParametersMessenger;
class DetectorConstruction;
class G4String;

typedef std::pair<double, double> EnergyRange_keV;
typedef std::multimap<const G4String, EnergyRange_keV> MapParticlesEnergy;

class GasModelParameters{
	public:
	
	GasModelParameters();
	~GasModelParameters();
    
    
    
    void AddParticleNameHeedNewTrack(const G4String particleName,double ekin_min_keV,double ekin_max_keV);
    void AddParticleNameHeedDeltaElectron(const G4String particleName,double ekin_min_keV,double ekin_max_keV);
    
    /*Getters and Setters*/
    //Name of the Magboltz file to be used (if needed)
    inline void SetGasFile(G4String s) { gasFile = s;};
    inline G4String GetGasFile() {return gasFile;};
    //Name of the Ion mobility file (if needed)
    inline void SetIonMobilityFile(G4String s) { ionMobFile = s; };
    inline G4String GetIonMobilityFile() {return  ionMobFile; };
    //Determines if the electrons are drifted, or only primary ionization is simulated
    inline void SetDriftElectrons(G4bool b) { driftElectrons = b; };
    inline bool GetDriftElectrons(){return driftElectrons;};
    inline void SetVoltagePlaneHV(G4double v){vPlaneHV = v;};
    inline double GetVoltagePlaneHV(){return vPlaneHV;};
    inline void SetVoltagePlaneLow(G4double v){vPlaneLow = v;};
    inline double GetVoltagePlaneLow(){return vPlaneLow;};
    inline void SetVoltageAnodeWires(G4double v){vAnodeWires = v;};
    inline double GetVoltageAnodeWires(){return vAnodeWires;};
    inline void SetVoltageCathodeWires(G4double v){vCathodeWires = v;};
    inline double GetVoltageCathodeWires(){return vCathodeWires;};
    inline void SetVoltageGate(G4double v){vGate = v;};
    inline double GetVoltageGate(){return vGate;};
    inline void SetVoltageDeltaGate(G4double v){vDeltaGate = v;};
    inline double GetVoltageDeltaGate(){return vDeltaGate;};
    inline void SetTrackMicroscopic(bool b){trackMicro=b;};
    inline bool GetTrackMicroscopic(){return trackMicro;};
    inline void SetCreateAvalancheMC(bool b){createAval=b;};
    inline bool GetCreateAvalancheMC(){return createAval;};
    inline void SetVisualizeChamber(bool b){fVisualizeChamber = b;};
    inline bool GetVisualizeChamber(){return fVisualizeChamber;};
    inline void SetVisualizeSignals(bool b){fVisualizeSignal = b;};
    inline bool GetVisualizeSignals(){return fVisualizeSignal;};
    inline void SetVisualizeField(bool b){fVisualizeField = b;};
    inline bool GetVisualizeField(){return fVisualizeField;};
    inline void SetDriftRKF(bool b){driftRKF=b;};
    inline bool GetDriftRKF(){return driftRKF;};
    
    inline MapParticlesEnergy GetParticleNamesHeedNewTrack(){return fMapParticlesEnergyHeedNewTrack;};
    inline MapParticlesEnergy GetParticleNamesHeedDeltaElectron(){return fMapParticlesEnergyHeedDeltaElectron;};

	
	private:
	GasModelParametersMessenger* fMessenger;
    MapParticlesEnergy fMapParticlesEnergyHeedNewTrack;
    MapParticlesEnergy fMapParticlesEnergyHeedDeltaElectron;
    
    G4String gasFile;
    G4String ionMobFile;
    
    bool driftElectrons;
    bool trackMicro;
    bool createAval;
    bool fVisualizeChamber;
    bool fVisualizeSignal;
    bool fVisualizeField;
    bool driftRKF;
    
    double vPlaneHV;
    double vPlaneLow;
    double vAnodeWires;
    double vCathodeWires;
    double vGate;
    double vDeltaGate;
};

#endif
