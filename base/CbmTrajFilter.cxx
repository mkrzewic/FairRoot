// ********************************************* //
// ***        D. Kresan   2004-Sep-14        *** //
// ***        D.Kresan@gsi.de                *** //
// ********************************************* //

using namespace std;
#include <iostream>

#include "TParticle.h"

#include "CbmTrajFilter.h"
#include "CbmRootManager.h"
#include "TMath.h"

ClassImp(CbmTrajFilter)



CbmTrajFilter* CbmTrajFilter::fgInstance = NULL;

CbmTrajFilter* CbmTrajFilter::Instance()
{
  return fgInstance;
}



CbmTrajFilter::CbmTrajFilter()
: fVxMin (-2000.),
  fVxMax ( 2000.),
  fVyMin (-2000.),
  fVyMax ( 2000.),
  fVzMin (-2000.),
  fVzMax ( 2000.),
  fPMin ( 0.),
  fPMax ( 1e10),
  fThetaMin ( 0.),
  fThetaMax ( TMath::Pi()),
  fPhiMin ( 0.),
  fPhiMax ( TMath::TwoPi()),
  fPxMin (-1e10),
  fPxMax ( 1e10),
  fPyMin (-1e10),
  fPyMax ( 1e10),
  fPzMin (-1e10),
  fPzMax ( 1e10),
  fPtMin ( 0.),
  fPtMax ( 1e10),
  fRapidityMin (-1e10),
  fRapidityMax ( 1e10),
  fKinCutType ( 0), // polar system by default
  fEtotMin ( 0.),
  fEtotMax ( 1e10),
  fStorePrim ( kTRUE),
  fStoreSec ( kTRUE),
  fStepSizeMin ( 0.1), // 1mm by default
  fTrackCollection(new TClonesArray("TGeoTrack")),
  fCurrentTrk(0)

{
  if(NULL != fgInstance) {
    Fatal("CbmTrajFilter", "Singleton class already exists.");
    return;
  }
  
   fgInstance = this;
}



CbmTrajFilter::~CbmTrajFilter()
{
  fgInstance = NULL;
}

void CbmTrajFilter::Init(TString brName, TString folderName)
{
  
  CbmRootManager::Instance()->Register(brName.Data(), folderName.Data(), fTrackCollection, kTRUE);

}

void CbmTrajFilter::Reset()
{
  fTrackCollection->Delete();
}

Bool_t CbmTrajFilter::IsAccepted(const TParticle *p) const
{
  if( NULL == p ) {
    return kFALSE;
  }

  // Apply vertex cut
  if( (p->Vx()<fVxMin) || (p->Vx()>fVxMax) ||
      (p->Vy()<fVyMin) || (p->Vy()>fVyMax) ||
      (p->Vz()<fVzMin) || (p->Vz()>fVzMax) ) {
    return kFALSE;
  }

  // Apply cut on kinematics
  if( 0 == fKinCutType ) {
    if( (p->P()<fPMin) || (p->P()>fPMax) ||
	(p->Theta()<fThetaMin) || (p->Theta()>fThetaMax) ||
	(p->Phi()<fPhiMin) || (p->Phi()>fPhiMax) ) {
      return kFALSE;
    }
  } else if( 1 == fKinCutType ) {
    if( (p->Px()<fPxMin) || (p->Px()>fPxMax) ||
	(p->Py()<fPyMin) || (p->Py()>fPyMax) ||
	(p->Pz()<fPzMin) || (p->Pz()>fPzMax) ) {
      return kFALSE;
    }
  } else {
    Double_t rapidity = 0.5*TMath::Log( (p->Energy()+p->Pz()) / 
					(p->Energy()-p->Pz()) );
    if( (p->Pt()<fPtMin) || (p->Pt()>fPtMax) ||
	(rapidity<fRapidityMin) || (rapidity>fRapidityMax) ) {
      return kFALSE;
    }
  }

  // Apply energy cut
  if( (p->Energy()<fEtotMin) || (p->Energy()>fEtotMax) ) {
    return kFALSE;
  }

  // Apply generation cut
  if(-1 == p->GetFirstMother()) {
    if(kFALSE == fStorePrim) {
      return kFALSE;
    }
  } else {
    if(kFALSE == fStoreSec) {
      return kFALSE;
    }
  }

  return kTRUE;
}



void CbmTrajFilter::SetVertexCut(Double_t vxMin, Double_t vyMin, Double_t vzMin,
				 Double_t vxMax, Double_t vyMax, Double_t vzMax)
{
  if( (vxMax<vxMin) || (vyMax<vyMin) || (vzMax<vzMin) ||
      (TMath::Abs(vxMin)>2000.) || (TMath::Abs(vxMax)>2000.) ||
      (TMath::Abs(vyMin)>2000.) || (TMath::Abs(vyMax)>2000.) ||
      (TMath::Abs(vzMin)>2000.) || (TMath::Abs(vzMax)>2000.) ) {
    cout << "-E- CbmTrajFilter::SetVertexCut() : invalid region, ignoring." << endl;
    return;
  }
  fVxMin = vxMin;
  fVxMax = vxMax;
  fVyMin = vyMin;
  fVyMax = vyMax;
  fVzMin = vzMin;
  fVzMax = vzMax;
}



void CbmTrajFilter::SetMomentumCutP(Double_t pMin, Double_t thetaMin, Double_t phiMin,
				    Double_t pMax, Double_t thetaMax, Double_t phiMax)
{
  if( (pMax<pMin) || (thetaMax<thetaMin) || (phiMax<phiMin) ||
      (pMin<0.) || (pMax<0.) || (thetaMin<0.) || (thetaMax>TMath::Pi()) ||
      (phiMin<0.) || (phiMax>TMath::TwoPi()) ) {
    cout << "-E- CbmTrajFilter::SetMomentumCutP() : invalid region, ignoring." << endl;
    return;
  }
  fPMin = pMin;
  fPMax = pMax;
  fThetaMin = thetaMin;
  fThetaMax = thetaMax;
  fPhiMin = phiMin;
  fPhiMax = phiMax;
  fKinCutType = 0;
}



void CbmTrajFilter::SetMomentumCutD(Double_t pxMin, Double_t pyMin, Double_t pzMin,
				    Double_t pxMax, Double_t pyMax, Double_t pzMax)
{
  if( (pxMax<pxMin) || (pyMax<pyMin) || (pzMax<pzMin) ) {
    cout << "-E- CbmTrajFilter::SetMomentumCutD() : invalid region, ignoring." << endl;
    return;
  }
  fPxMin = pxMin;
  fPxMax = pxMax;
  fPyMin = pyMin;
  fPyMax = pyMax;
  fPzMin = pzMin;
  fPzMax = pzMax;
  fKinCutType = 1;
}



void CbmTrajFilter::SetPtRapidityCut(Double_t ptMin, Double_t ptMax,
				     Double_t rapidityMin, Double_t rapidityMax)
{
  if( (ptMax<ptMin) || (rapidityMax<rapidityMin) ||
      (ptMin<0.) || (ptMax<0.) ) {
    cout << "-E- CbmTrajFilter::SetPtRapidityCut() : invalid region, ignoring." << endl;
    return;
  }
  fPtMin = ptMin;
  fPtMax = ptMax;
  fRapidityMin = rapidityMin;
  fRapidityMax = rapidityMax;
  fKinCutType = 2;
}



void CbmTrajFilter::SetEnergyCut(Double_t etotMin, Double_t etotMax)
{
  if( (etotMax<etotMin) || (etotMin<0.) || (etotMax<0.) ) {
    cout << "-E- CbmTrajFilter::SetEnergyCut() : invalid region, ignoring." << endl;
    return;
  }
  fEtotMin = etotMin;
  fEtotMax = etotMax;
}



void CbmTrajFilter::SetStepSizeCut(Double_t stepSizeMin)
{
  if(stepSizeMin < 0.) {
    cout << "-E- CbmTrajFilter::SetStepSizeCut() : invalid value, ignoring." << endl;
    return;
  }
  fStepSizeMin = stepSizeMin;
}



void CbmTrajFilter::GetVertexCut(Double_t &vxMin, Double_t &vyMin, Double_t &vzMin,
				 Double_t &vxMax, Double_t &vyMax, Double_t &vzMax) const
{
  vxMin = fVxMin;
  vxMax = fVxMax;
  vyMin = fVyMin;
  vyMax = fVyMax;
  vzMin = fVzMin;
  vzMax = fVzMax;
}


void CbmTrajFilter::GetMomentumCutP(Double_t &pMin, Double_t &thetaMin, Double_t &phiMin,
				    Double_t &pMax, Double_t &thetaMax, Double_t &phiMax) const
{
  pMin = fPMin;
  pMax = fPMax;
  thetaMin = fThetaMin;
  thetaMax = fThetaMax;
  phiMin = fPhiMin;
  phiMax = fPhiMax;
}


void CbmTrajFilter::GetMomentumCutD(Double_t &pxMin, Double_t &pyMin, Double_t &pzMin,
				    Double_t &pxMax, Double_t &pyMax, Double_t &pzMax) const
{
  pxMin = fPxMin;
  pxMax = fPxMax;
  pyMin = fPyMin;
  pyMax = fPyMax;
  pzMin = fPzMin;
  pzMax = fPzMax;
}


void CbmTrajFilter::GetPtRapidityCut(Double_t &ptMin, Double_t &ptMax,
				     Double_t &rapidityMin, Double_t &rapidityMax) const
{
  ptMin = fPtMin;
  ptMax = fPtMax;
  rapidityMin = fRapidityMin;
  rapidityMax = fRapidityMax;
}


void CbmTrajFilter::GetEnergyCut(Double_t &etotMin, Double_t &etotMax) const
{
  etotMin = fEtotMin;
  etotMax = fEtotMax;
}


TGeoTrack *CbmTrajFilter::AddTrack(Int_t trackId, Int_t pdgCode)
{

//  cout << "CbmTrajFilter::AddTrack" << endl;
  TClonesArray& clref = *fTrackCollection;
  Int_t tsize = clref.GetEntriesFast();
  fCurrentTrk =  new(clref[tsize]) TGeoTrack(trackId,pdgCode);
  return fCurrentTrk;
}

TGeoTrack *CbmTrajFilter::AddTrack(TParticle *p)
{

  Int_t trackId=0;
//  cout << "CbmTrajFilter::AddTrack" << endl;
  if(fCurrentTrk) trackId=fCurrentTrk->GetId();
  Int_t pdgCode = p->GetPdgCode();
  TClonesArray& clref = *fTrackCollection;
  Int_t tsize = clref.GetEntriesFast();
  fCurrentTrk =  new(clref[tsize]) TGeoTrack(++trackId,pdgCode,0,p);
  return fCurrentTrk;
}



TGeoTrack *CbmTrajFilter::GetTrack(Int_t trackId)
{
                            
  return (TGeoTrack *)fTrackCollection->At(trackId);
}

