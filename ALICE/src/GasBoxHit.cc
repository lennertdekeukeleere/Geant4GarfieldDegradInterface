#include "GasBoxHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4ThreadLocal G4Allocator<GasBoxHit>* GasBoxHitAllocator;

GasBoxHit::GasBoxHit() : G4VHit(), fTime(-1),
   fPos(G4ThreeVector()){}

GasBoxHit::~GasBoxHit(){}

GasBoxHit::GasBoxHit(const GasBoxHit& rhs) : G4VHit() {
    fPos= rhs.fPos;
    fTime=rhs.fTime;
}

const GasBoxHit& GasBoxHit::operator=(const GasBoxHit& rhs){
    fPos= rhs.fPos;
    fTime=rhs.fTime;
    return *this;
}

G4int GasBoxHit::operator==(const GasBoxHit& rhs) const{
    return (this==&rhs) ? 1 : 0;
}

void GasBoxHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(1.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
    
    //G4cout<<"DRAWING "<<fPos.getY()<<G4endl;
  }
}

void GasBoxHit::Print(){
    G4cout << "Printing hits" << G4endl;
}
