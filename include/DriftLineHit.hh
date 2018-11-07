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
	
	  virtual void Draw(){};
    virtual void Print(){};
    
    G4ThreeVector GetPos(){return fPos;};
    G4double GetTime(){return fTime;};
    
    void SetPos(G4ThreeVector xyz){ fPos = xyz; };
    void SetTime(G4double t){ fTime = t; };
    
    
private:
    G4double fTime;
    G4ThreeVector fPos;
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
