#ifndef SiliconHit_HH
#define SiliconHit_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class SiliconHit : public G4VHit {
    
    
public:
    SiliconHit();
    virtual ~SiliconHit();
    SiliconHit(const SiliconHit &);
    
    const SiliconHit& operator=(const SiliconHit&);
    G4int operator==(const SiliconHit&) const;
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
	
	virtual void Draw();
    virtual void Print();
    
    G4ThreeVector GetPos(){return fPos;};
    G4double GetTime(){return fTime;};
    
    void SetPos(G4ThreeVector xyz){ fPos = xyz; };
    void SetTime(G4double t){ fTime = t; };
    
    
private:
    G4double      fTime;
    G4ThreeVector fPos;
};

using SiliconHitsCollection=G4THitsCollection<SiliconHit>;

extern G4ThreadLocal G4Allocator<SiliconHit>* SiliconHitAllocator;

inline void* SiliconHit::operator new(size_t){
  if (!SiliconHitAllocator) {
         SiliconHitAllocator = new G4Allocator<SiliconHit>;
  }
  return (void*)SiliconHitAllocator->MallocSingle();
}

inline void SiliconHit::operator delete(void *aHit){
    SiliconHitAllocator->FreeSingle((SiliconHit*) aHit);
}

#endif
