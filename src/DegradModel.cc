#include "G4Electron.hh"
#include "G4SystemOfUnits.hh"
#include "degradModel.hh"
#include "G4Region.hh"
#include "G4ParticleDefinition.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"
#include "Randomize.hh"
#include "G4UIcommand.hh"
#include <fstream>
#include "G4TransportationManager.hh"
#include "G4DynamicParticle.hh"
#include "G4RandomDirection.hh"
#include "GasModelParameters.hh"

DegradModel::DegradModel(GasModelParameters* gmp, G4String modelName, G4Region* envelope,DetectorConstruction* dc)
    : G4VFastSimulationModel(modelName, envelope),detCon(dc)	{
      thermalE=gmp->GetThermalEnergy();
    }

DegradModel::~DegradModel() {}

G4bool DegradModel::IsApplicable(const G4ParticleDefinition& particleType) {
  if (particleType.GetParticleName()=="e-")
    return true;
  return false;
}

G4bool DegradModel::ModelTrigger(const G4FastTrack& fastTrack) {
  G4double ekin = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
  if (ekin>thermalE)
		return true;
  return false;

}

void DegradModel::DoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep) {

    fastStep.KillPrimaryTrack();
    G4ThreeVector degradPos =fastTrack.GetPrimaryTrack()->GetVertexPosition();
    G4double degradTime = fastTrack.GetPrimaryTrack()->GetGlobalTime();
    G4String particleName =
      fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();

    fastStep.KillPrimaryTrack();
    fastStep.SetPrimaryTrackPathLength(0.0);
    G4cout<<"GLOBAL TIME "<<G4BestUnit(degradTime,"Time")<<" POSITION "<<G4BestUnit(degradPos,"Length")<<G4endl;
    //CallDegrad();
    //fastStep.SetTotalEnergyDeposited(fastTrack.GetPrimaryTrack()->GetKineticEnergy());

    G4int stdout;
    G4int SEED=54217137*G4UniformRand();
    G4String seed = G4UIcommand::ConvertToString(SEED);
    G4String degradString="printf \"1,1,3,-1,"+seed+",5900.0,7.0,0.0\n7,0,0,0,0,0\n100.0,0.0,0.0,0.0,0.0,0.0,20.0,900.0\n3000.0,0.0,0.0,1,0\n100.0,0.5,1,1,1,1,1,1,1\n0,0,0,0,0,0\" > conditions_Degrad.txt";

    stdout=system(degradString.data());
    stdout=system("./a.out < conditions_Degrad.txt");
    stdout=system("./convertDegradFile.py");

    GetElectronsFromDegrad(fastStep,degradPos,degradTime);

}

void DegradModel::GetElectronsFromDegrad(G4FastStep& fastStep,G4ThreeVector degradPos,G4double degradTime)
{
    G4int eventNumber,Nep, nline, i, electronNumber; //Nep é o numero de e primarios que corresponde ao que o biagi chama de "ELECTRON CLUSTER SIZE (NCLUS)"
    G4double posX,posY,posZ,time,n;
    G4double  posXDegrad,posYDegrad,posZDegrad,timeDegrad;
    G4double  posXInitial=degradPos.getX();
    G4double  posYInitial=degradPos.getY();
    G4double  posZInitial=degradPos.getZ();
    G4double  timeInitial=degradTime;
    G4String line;
    std::vector<G4double> v;
    
    std::ifstream inFile;
    G4String fname= "DEGRAD.OUT";
    inFile.open(fname,std::ifstream::in);
    
    G4cout<< "Working in "<<fname<<G4endl;
    
    nline=1;
    G4int tester=0;
    while (getline(inFile, line,'\n'))//o '\n' indica o caracter para delimitaro fim de linha
    {
        std::istringstream iss(line);//stream de strings
        if (nline ==1) //resumo
        {
            while (iss >> n) //cada stream ira atribuir o valor a n
            {
                v.push_back(n); //o n é adicionado ao vector
            }
            
            eventNumber=v[0];
            Nep=v[1];
            //                            Nexc=v[2];
            v.clear();
        }
        if (nline ==2) //Ionizations
        {
            electronNumber=0;
            while (iss >> n) //cada stream ira atribuir o valor a n
            {
                v.push_back(n); //o n é adicionado ao vector
            }
            for (i=0;i<v.size();i=i+7){
                posXDegrad=v[i];
                posYDegrad=v[i+1];
                posZDegrad=v[i+2];
                timeDegrad=v[i+3];
                //convert from um to mm in GEANT4
                //also Y and Z axes are swaped in GEANT4 and Garfield++ relatively to Degrad
                posX=posXDegrad*0.001+posXInitial;
                posY=posZDegrad*0.001+posYInitial;
                posZ=posYDegrad*0.001+posZInitial;
                //convert ps to ns
                time=timeDegrad*0.001+timeInitial;
                electronNumber++; //Numera os electroes primarios
                
                G4ThreeVector myPoint;
                myPoint.setX(posX);
                myPoint.setY(posY);
                myPoint.setZ(posZ);
                
                //Check in which Physical volume the point bellongs
                G4Navigator* theNavigator= G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
                
                G4VPhysicalVolume* myVolume = theNavigator->LocateGlobalPointAndSetup(myPoint);
                
                G4String solidName=myVolume->GetName();
                
                if (solidName.contains("detectorPhysical") && tester<1){//AROUCA: APENAS PARA LIMITAR O NUMERO DE ELECTROES EM TESTES
                    //G4cout<<"INSIDE"<<G4endl;
                    
                    //AROUCA: METER AQUI O ESPECTRO DE EMISSAO DO XENON
                    
                    G4DynamicParticle electron(G4Electron::ElectronDefinition(),G4RandomDirection(), 7.0*eV);
                    
                    // Create secondary electron
                    G4Track *newTrack=fastStep.CreateSecondaryTrack(electron, myPoint, time,false);
                    
                    //G4cout<<"SECONDARY TRACKS: "<<fastStep.GetNumberOfSecondaryTracks ()<<G4endl;
                    //AROUCA TESTER INCREMENT
                    tester++;
                    G4cout<<tester<<G4endl;
                    
                }
            }
            v.clear(); //Faz reset ao vector senão vai continuar a adicionar os dadosadicionar os dados
            nline=0;
            
        }
        nline++;
        
        
    }
    inFile.close();
    
    
}


