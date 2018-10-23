#ifndef GASBOXHIT_HH
#define GASBOXHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class GasBoxHit : public G4VHit {
    
    
public:
    GasBoxHit();
    virtual ~GasBoxHit();
    GasBoxHit(const GasBoxHit &);
    
    const GasBoxHit& operator=(const GasBoxHit&);
    G4int operator==(const GasBoxHit&) const;
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
	
	void Draw() const{};
    void Print() const{};
    
    void SetEkin(G4double ekin){fEkin_MeV=ekin;};
    void SetTime(G4double time){fTime=time;};
    void SetX(G4double x){fx_mm=x;};
    void SetY(G4double y){fy_mm=y;};
    void SetZ(G4double z){fz_mm=z;};
//    void SetXmomentum(G4double dx){fdx=dx;};
//    void SetYmomentum(G4double dx){fdy=dx;};
//    void SetZmomentum(G4double dx){fdz=dx;};
    void SetID(G4int i){id=i;};
    void SetParentID(G4int i){parent_id=i;};
    void SetProcess(G4int i){process=i;};
    
    G4double GetEkin(){return fEkin_MeV;};
    G4double GetTime(){return fTime;};
    G4double GetX(){return fx_mm;};
    G4double GetY(){return fy_mm;};
    G4double GetZ(){return fz_mm;};
//    void GetXmomentum(){return fdx;};
//    void GetYmomentum(){return fdy;};
//    void GetZmomentum(){return fdz;};
    G4int GetID(){return id;};
    G4int GetParentID(){return parent_id;};
    G4int GetProcess(){return process;};
    
    
private:
    G4double fEkin_MeV;
    G4double fTime;
    G4double fx_mm;
    G4double fy_mm;
    G4double fz_mm;
//    G4double fdx;
//    G4double fdy;
//    G4double fdz;
    G4int id;
    G4int parent_id;
    G4int process;
};

using GasBoxHitsCollection=G4THitsCollection<GasBoxHit>;

extern G4ThreadLocal G4Allocator<GasBoxHit>* GasBoxHitAllocator;

inline void* GasBoxHit::operator new(size_t){
  if (!GasBoxHitAllocator) {
         GasBoxHitAllocator = new G4Allocator<GasBoxHit>;
  }
  return (void*)GasBoxHitAllocator->MallocSingle();
}

inline void GasBoxHit::operator delete(void *aHit){
    GasBoxHitAllocator->FreeSingle((GasBoxHit*) aHit);
}

#endif
