#ifndef DriftLineHit_HH
#define DriftLineHit_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class DriftLineHit : public G4VHit {
    
    
public:
    DriftLineHit();
    virtual ~DriftLineHit();
    DriftLineHit(const DriftLineHit &);
    
    const DriftLineHit& operator=(const DriftLineHit&);
    G4int operator==(const DriftLineHit&) const;
    
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

using DriftLineHitsCollection=G4THitsCollection<DriftLineHit>;

extern G4ThreadLocal G4Allocator<DriftLineHit>* DriftLineHitAllocator;

inline void* DriftLineHit::operator new(size_t){
  if (!DriftLineHitAllocator) {
         DriftLineHitAllocator = new G4Allocator<DriftLineHit>;
  }
  return (void*)DriftLineHitAllocator->MallocSingle();
}

inline void DriftLineHit::operator delete(void *aHit){
    DriftLineHitAllocator->FreeSingle((DriftLineHit*) aHit);
}

#endif
