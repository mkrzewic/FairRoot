
// Extended TParticle with pointers to mother and daughters
// particles

#include "CbmParticle.h"

#include "TParticle.h"
#include "TMCParticleType.h"
#include <iostream>

using std::cout;
using std::endl;

ClassImp(CbmParticle)
const Double_t kProtonMass = 0.93827231;
//_____________________________________________________________________________
CbmParticle::CbmParticle(Int_t id, TParticle* particle)
  : fpdg(id),
    fParticle(particle),
    fMother(0),
    fDaughters(0),
    fname(particle->GetName()),
	fmcType (kPTIon),
    fmass  (0),
    fcharge(0),
    fDecayTime(0),
    fpType("Ion"),
    fwidth(0),
    fiSpin(0),
    fiParity(0),
    fiConjugation(0),
    fiIsospin(0),
    fiIsospinZ(0),
    fgParity(0),
    flepton(0),
    fbaryon(0),
    fstable(kTRUE)
{

 //  fname=particle->GetName();
   fmcType= kPTIon;
   fmass= particle->GetMass();
   fcharge= particle->GetPDG()->Charge();
   fDecayTime= particle->GetPDG()->Lifetime();
 //  fpType=   "Ion";
   fwidth=   particle-> GetPDG()->Width();
   fiSpin= (Int_t)  particle->GetPDG()->Spin();
   fiParity=particle->GetPDG()->Parity();
   fiConjugation= 0;
   fiIsospin= (Int_t) particle->GetPDG()->Isospin();
   fiIsospinZ= 0;
   fgParity= 0;           
   flepton=  0;
   fbaryon=  0;                            
   fstable= particle->GetPDG()->Stable();

//
}
//_____________________________________________________________________________

CbmParticle::CbmParticle(const char* name, Int_t z, Int_t a, Double_t mass ,Int_t q, Bool_t stable, Double_t decaytime)
 :  fpdg(0),
    fParticle(0),
    fMother(0),
    fDaughters(0),
    fname(name),
	fmcType (kPTIon),
    fmass  (0),
    fcharge(0),
    fDecayTime(0),
    fpType("Ion"),
    fwidth(0),
    fiSpin(0),
    fiParity(0),
    fiConjugation(0),
    fiIsospin(0),
    fiIsospinZ(0),
    fgParity(0),
    flepton(0),
    fbaryon(0),
    fstable(kTRUE)
{
   fpdg= 10000000+10000* z +10 * a;
   fDecayTime= decaytime;
   
   if (!TDatabasePDG::Instance()->GetParticle(fpdg)){
        if(mass ==0 )mass = a*kProtonMass;
    	TDatabasePDG::Instance()
      		->AddParticle(name, name, mass, stable, 0, q, "kPTHadron", fpdg);

                /*   AddParticle(const char *name, const char *title,
                                        Double_t mass, Bool_t stable,
                                        Double_t width, Double_t charge,
                                        const char* ParticleClass,
                                        Int_t PDGcode,
                                        Int_t Anti,
                                        Int_t TrackingCode)
                  */
   }
   fParticle = new TParticle();
   fParticle->SetPdgCode(fpdg);
  
   fmcType= kPTHadron;
   fmass= mass;
   fcharge= fParticle->GetPDG()->Charge();
   fDecayTime=decaytime ;
   fwidth=   fParticle->GetPDG()->Width();
   fiSpin=   (Int_t)fParticle->GetPDG()->Spin();
   fiParity=  fParticle->GetPDG()->Parity();
   fiConjugation= 0;
   fiIsospin= (Int_t) fParticle->GetPDG()->Isospin();
   fiIsospinZ= 0;
   fgParity= 0;
   flepton=  0;
   fbaryon=  0;
   fstable= fParticle->GetPDG()->Stable();


}

//_____________________________________________________________________________
CbmParticle::CbmParticle(Int_t id, TParticle* particle, CbmParticle* mother)
  : fpdg(id),
    fParticle(particle),
    fMother(mother),
    fDaughters(),
    fname(particle->GetName()),
	fmcType (kPTIon),
    fmass  (0),
    fcharge(0),
    fDecayTime(0),
    fpType("Ion"),
    fwidth(0),
    fiSpin(0),
    fiParity(0),
    fiConjugation(0),
    fiIsospin(0),
    fiIsospinZ(0),
    fgParity(0),
    flepton(0),
    fbaryon(0),
    fstable(kTRUE)
  
{
//
}

//_____________________________________________________________________________
CbmParticle::CbmParticle( Int_t pdg,
                 const TString name,
                 TMCParticleType mcType,
                 Double_t mass,    
		 Double_t charge,
                 Double_t lifetime,
                 const TString pType, 
                 Double_t width, 
                 Int_t iSpin, 
                 Int_t iParity, 
                 Int_t iConjugation, 
                 Int_t iIsospin, 
                 Int_t iIsospinZ, 
                 Int_t gParity,
                 Int_t lepton, 
                 Int_t baryon,
		 Bool_t stable )
 : fpdg  (pdg),
   fParticle( new TParticle()),
   fMother(0),
   fDaughters(0),
   fname(name),
   fmcType (mcType),
   fmass  (mass),
   fcharge( charge),
   fDecayTime( lifetime),
   fpType(   pType),
   fwidth(   width),
   fiSpin(  iSpin),
   fiParity(   iParity),
   fiConjugation( iConjugation),
   fiIsospin(   iIsospin),
   fiIsospinZ(  iIsospinZ),
   fgParity( gParity),
   flepton(  lepton),
   fbaryon(  baryon),
   fstable(  stable)
{
  
   if (!TDatabasePDG::Instance()->GetParticle(fpdg)){
    	TDatabasePDG::Instance()
      		->AddParticle(fname, fname, fmass, fstable, fwidth,fcharge ,pType, fpdg);

   }
   
   fParticle->SetPdgCode(fpdg);

}

//_____________________________________________________________________________
CbmParticle::CbmParticle()
  : fpdg(0),
    fParticle(0),
    fMother(0),
    fDaughters(),
    fname("ion"),
    fmcType (kPTIon),
    fmass  (0),
    fcharge(0),
    fDecayTime(0),
    fpType("Ion"),
    fwidth(0),
    fiSpin(0),
    fiParity(0),
    fiConjugation(0),
    fiIsospin(0),
    fiIsospinZ(0),
    fgParity(0),
    flepton(0),
    fbaryon(0),
    fstable(kTRUE)
{
//   
}

//_____________________________________________________________________________
CbmParticle::~CbmParticle() 
{
//
  delete fParticle;
}

//
// public methods
//

//_____________________________________________________________________________
void CbmParticle::SetMother(CbmParticle* particle)
{
// Adds particles daughter
// ---

  fMother.SetObject(particle);
}  

//_____________________________________________________________________________
void CbmParticle::AddDaughter(CbmParticle* particle)
{
// Adds particles daughter
// ---

  fDaughters.Add(particle);
}  

//_____________________________________________________________________________
void CbmParticle::Print() const
{
// Prints particle properties.
// ---
//  return; 

  cout << "Particle: " << fParticle->GetName()  << "  with ID:  " << fpdg << endl;

  // fParticle->Print();  
  
  if (GetMother()) {
    cout << "Mother:    " << GetMother()->GetParticle()->GetName() 
                          << "  with ID: " << GetMother()->GetPDG() << endl;
  }
  else			    
    cout << "Primary    " << endl; 
    
  cout << "Number of daughters: " << GetNofDaughters() << endl;
  cout << endl;			  
}  

//_____________________________________________________________________________
void CbmParticle::PrintDaughters() const
{
// Prints particles daughters.
// ---

  for (Int_t i=0; i<GetNofDaughters(); i++)  {
    cout << i << "th daughter: " << endl;
    GetDaughter(i)->Print();
  }  
}  

//_____________________________________________________________________________
Int_t  CbmParticle::GetPDG() const
{
// Returs particle ID.
// ---

  return fpdg;
}  


//_____________________________________________________________________________
TParticle*  CbmParticle::GetParticle() const
{
// Returns particle definition (TParticle).
// ---

  return fParticle;
}  

//_____________________________________________________________________________
CbmParticle* CbmParticle::GetMother() const
{
// Returns particle definition (TParticle).
// ---

  return (CbmParticle*) fMother.GetObject();
}  

//_____________________________________________________________________________
Int_t CbmParticle::GetNofDaughters() const
{
// Returns number of daughters.
// ---

  return fDaughters.GetEntriesFast();
}  

//_____________________________________________________________________________
CbmParticle* CbmParticle::GetDaughter(Int_t i) const
{
// Returns i-th daughter.
// ---

  if (i < 0 || i >= GetNofDaughters())
    Fatal("GetDaughter", "Index out of range"); 

  return (CbmParticle*) fDaughters.At(i);
}  
