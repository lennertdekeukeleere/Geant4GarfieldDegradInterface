#include "GasBoxHit.hh"

G4ThreadLocal G4Allocator<GasBoxHit>* GasBoxHitAllocator;

GasBoxHit::GasBoxHit() : G4VHit(), fTime(0), fx_mm(0), fy_mm(0), fz_mm(0){}

GasBoxHit::~GasBoxHit(){}

GasBoxHit::GasBoxHit(const GasBoxHit& rhs) : G4VHit() {
    fx_mm=rhs.fx_mm;
    fy_mm=rhs.fy_mm;
    fz_mm=rhs.fz_mm;
    fTime=rhs.fTime;
}

const GasBoxHit& GasBoxHit::operator=(const GasBoxHit& rhs){
    fx_mm=rhs.fx_mm;
    fy_mm=rhs.fy_mm;
    fz_mm=rhs.fz_mm;
    fTime=rhs.fTime;
    return *this;
}

G4int GasBoxHit::operator==(const GasBoxHit& rhs) const{
    return (this==&rhs) ? 1 : 0;
}
