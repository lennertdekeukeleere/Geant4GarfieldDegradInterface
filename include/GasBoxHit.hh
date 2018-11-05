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
    
    void SetTime(G4double time){fTime=time;};
    void SetX(G4double x){fx_mm=x;};
    void SetY(G4double y){fy_mm=y;};
    void SetZ(G4double z){fz_mm=z;};
    
    G4double GetTime(){return fTime;};
    G4double GetX(){return fx_mm;};
    G4double GetY(){return fy_mm;};
    G4double GetZ(){return fz_mm;};
    
    
private:
    G4double fTime;
    G4double fx_mm;
    G4double fy_mm;
    G4double fz_mm;
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
