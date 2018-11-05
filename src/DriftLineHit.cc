#include "DriftLineHit.hh"

G4ThreadLocal G4Allocator<DriftLineHit>* DriftLineHitAllocator;

DriftLineHit::DriftLineHit() : G4VHit(), fx_mm(0), fy_mm(0), fz_mm(0), fTime(0){}

DriftLineHit::~DriftLineHit(){}

DriftLineHit::DriftLineHit(const DriftLineHit& rhs) : G4VHit() {
    fx_mm=rhs.fx_mm;
    fy_mm=rhs.fy_mm;
    fz_mm=rhs.fz_mm;
    fTime=rhs.fTime;
}

const DriftLineHit& DriftLineHit::operator=(const DriftLineHit& rhs){
    fx_mm=rhs.fx_mm;
    fy_mm=rhs.fy_mm;
    fz_mm=rhs.fz_mm;
    fTime=rhs.fTime;
    return *this;
}

G4int DriftLineHit::operator==(const DriftLineHit& rhs) const{
    return (this==&rhs) ? 1 : 0;
}
