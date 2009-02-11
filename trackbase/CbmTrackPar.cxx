// -------------------------------------------------------------------------
//			Created by M. Al-Turany  06.02.2007
// -------------------------------------------------------------------------



#include <iostream>
#include "CbmTrackPar.h"
#include "TMath.h"

using namespace std;

// -----   Default constructor   -------------------------------------------
CbmTrackPar::CbmTrackPar() 
   :fX (0),
    fY (0),
    fZ (0),
	fDX (0),
    fDY (0),
    fDZ (0),
    fPx (0),
    fPy (0),
    fPz (0),
	fDPx (0),
    fDPy (0),
    fDPz (0),
    fQp (0),
	fDQp (0),
	fq  (1)
{

  
}
// -------------------------------------------------------------------------

// -----   Constructor with parameters   -----------------------------------
CbmTrackPar::CbmTrackPar(Double_t x, Double_t y, Double_t z,
				   Double_t px, Double_t py, Double_t pz, Double_t q)

  : fX (x),
    fY (y),
    fZ (z),
	fDX (x),
    fDY (y),
    fDZ (z),
    fPx (px),
    fPy (py),
    fPz (pz),
	fDPx (px),
    fDPy (py),
    fDPz (pz),
    fQp (0),
	fDQp (0),
	fq  (q)
{
  
  Double_t p=TMath::Sqrt(fPx*fPx +fPy*fPy +fPz*fPz );
  if (p!=0)fQp = q/p;
  
   
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmTrackPar::~CbmTrackPar() {}
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void CbmTrackPar::Print() {
  cout << "Position : (";
  cout.precision(2);
  cout << fX << ", " << fY << ", " << fZ << ")" << endl;
  cout << "Momentum : (";  
  cout << "Px = " << fPx << ", Py = " << fPy << " Pz = " << fPz << endl;
}

ClassImp(CbmTrackPar)

  



