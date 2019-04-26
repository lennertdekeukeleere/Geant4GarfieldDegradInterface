#include "SiliconHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4ThreadLocal G4Allocator<SiliconHit>* SiliconHitAllocator;

SiliconHit::SiliconHit() : G4VHit(), fTime(-1),
   fPos(G4ThreeVector()){}

SiliconHit::~SiliconHit(){}

SiliconHit::SiliconHit(const SiliconHit& rhs) : G4VHit() {
    fPos= rhs.fPos;
    fTime=rhs.fTime;
}

const SiliconHit& SiliconHit::operator=(const SiliconHit& rhs){
    fPos= rhs.fPos;
    fTime=rhs.fTime;
    return *this;
}

G4int SiliconHit::operator==(const SiliconHit& rhs) const{
    return (this==&rhs) ? 1 : 0;
}

void SiliconHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(1.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(0.,1.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
    
    //G4cout<<"DRAWING "<<fPos.getY()<<G4endl;
  }
}

void SiliconHit::Print(){
    G4cout << "Printing hits" << G4endl;
}