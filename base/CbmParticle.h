/// Class CBMParticle
/// ------------------
/// Extended TParticle with persistent pointers to mother and daughters
/// particles  (Ivana Hrivnacova, 5.4.2002)
/// Used to define particles which will be added to Geant3/4 (M. Al-Turany)


#ifndef CBM_PARTICLE_H
#define CBM_PARTICLE_H

#include "TObject.h"
#include "TRef.h"
#include "TRefArray.h"

#include "TMCParticleType.h"
class TParticle;

class CbmParticle : public TObject
{
  public:
    CbmParticle(Int_t id, TParticle* particle);
    CbmParticle(Int_t id, TParticle* particle, CbmParticle* mother);
    CbmParticle(const char* name, Int_t z, Int_t a, Double_t mass , Int_t q, Bool_t stable, Double_t decaytime);
    CbmParticle( Int_t pdg , const TString name, TMCParticleType mcType, Double_t mass, Double_t charge,
                 Double_t lifetime, const TString pType="Ion",  Double_t width=0, Int_t iSpin=0, Int_t iParity=0, 
                 Int_t iConjugation=0, Int_t iIsospin=0, Int_t iIsospinZ=0, Int_t gParity=0, Int_t lepton=0,
                 Int_t baryon=0,Bool_t stable=kFALSE);

    CbmParticle();

    virtual ~CbmParticle();     

    // methods
    void SetMother(CbmParticle* particle);
    void AddDaughter(CbmParticle* particle);
    void Print() const;
    void PrintDaughters() const;

    // get methods  
    Int_t            GetPDG() const;
    TParticle*       GetParticle() const;
    CbmParticle*     GetMother() const;
    Int_t            GetNofDaughters() const;
    CbmParticle*     GetDaughter(Int_t i) const;
    const TString&   GetName()          {return fname; }
    TMCParticleType  GetMCType()	{return fmcType;}
    Double_t         GetMass()          {return fmass;}
    Double_t         GetCharge()        {return fcharge;}
    Double_t         GetDecayTime()     {return fDecayTime;}
    const TString&   GetPType()         {return fpType;} 
    Double_t         GetWidth()         {return fwidth;} 
    Int_t            GetSpin()          {return fiSpin;} 
    Int_t            GetiParity()        {return fiParity;} 
    Int_t            GetConjugation()   {return fiConjugation;} 
    Int_t            GetIsospin()       {return fiIsospin;} 
    Int_t            GetIsospinZ()      {return fiIsospinZ;} 
    Int_t            GetgParity()        {return fgParity;}
    Int_t            GetLepton()        {return flepton;}                
    Int_t            GetBaryon()        {return fbaryon;}
    Bool_t           IsStable()         {return fstable;}

  private:
  
    CbmParticle(const CbmParticle &P);
    CbmParticle& operator= (const  CbmParticle&) {return *this;}
    // data members
    Int_t 		fpdg;
    TParticle*  	fParticle;
    TRef        	fMother;
    TRefArray   	fDaughters; 
    const TString            fname;
    TMCParticleType 	fmcType;
    Double_t 		fmass;
    Double_t 		fcharge;
    Double_t 		fDecayTime;
    const TString 	fpType; 
    Double_t 		fwidth; 
    Int_t 		fiSpin; 
    Int_t 		fiParity; 
    Int_t 		fiConjugation; 
    Int_t 		fiIsospin; 
    Int_t 		fiIsospinZ; 
    Int_t 		fgParity;
    Int_t 		flepton; 
    Int_t 		fbaryon;
    Bool_t 		fstable;

    ClassDef(CbmParticle,1) // Extended TParticle
};

#endif //CBM_PARTICLE_H   
   
