#include "GasBoxHit.hh"

G4ThreadLocal G4Allocator<GasBoxHit>* GasBoxHitAllocator;

GasBoxHit::GasBoxHit() : G4VHit(), fEkin_MeV(0), fx_mm(0), fy_mm(0), fz_mm(0), fTime(0), id(0), parent_id(0){}

GasBoxHit::~GasBoxHit(){}

GasBoxHit::GasBoxHit(const GasBoxHit& rhs) : G4VHit() {
    fEkin_MeV=rhs.fEkin_MeV;
    fx_mm=rhs.fx_mm;
    fy_mm=rhs.fy_mm;
    fz_mm=rhs.fz_mm;
    fTime=rhs.fTime;
    id=rhs.id;
    parent_id=rhs.parent_id;
    process=rhs.process;
}

const GasBoxHit& GasBoxHit::operator=(const GasBoxHit& rhs){
    fEkin_MeV=rhs.fEkin_MeV;
    fx_mm=rhs.fx_mm;
    fy_mm=rhs.fy_mm;
    fz_mm=rhs.fz_mm;
    fTime=rhs.fTime;
    id=rhs.id;
    parent_id=rhs.parent_id;
    process=rhs.process;
    return *this;
}

G4int GasBoxHit::operator==(const GasBoxHit& rhs) const{
    return (this==&rhs) ? 1 : 0;
}
