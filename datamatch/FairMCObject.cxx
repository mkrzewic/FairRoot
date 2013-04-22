/*
 * FairMCObject.cpp
 *
 *  Created on: Dec 1, 2009
 *      Author: stockman
 */

#include "FairMCObject.h"

ClassImp(FairMCObject);

FairMCObject::FairMCObject()
  : TObject(),
    fStage(0),
    fStageId()
{
}

FairMCObject::~FairMCObject()
{
}

void FairMCObject::SetEntry(FairMCEntry entry)
{
  if (entry.GetPos() < 0) {
    std::cout << "-E- FairMCObject::SetEntry(FairMCEntry): pos " << entry.GetPos() << std::endl;
    return;
  }
  AdoptSize(entry.GetPos());
  fStage[entry.GetPos()] = entry;
}

void FairMCObject::SetEntry(std::vector<Int_t> type, std::vector<Int_t> link, int index)
{
  FairMCEntry myEntry;
  myEntry.SetPos(index);
  myEntry.SetPersistanceCheck(kFALSE);
  for (int i = 0; i < type.size(); i++) {
    myEntry.AddLink(FairLink(type[i],link[i]));
  }
  SetEntry(myEntry);
}

void FairMCObject::SetEntry(FairMultiLinkedData* data, int index)
{
  AdoptSize(index);
  fStage[index].SetLinks(data->GetLinks());
}

void FairMCObject::SetLink(FairLink link, int index)
{
  if (index < fStage.size()) {
    fStage[index].Reset();
  }
  AddLink(link, index);
}


void FairMCObject::AddLink(FairLink link, int index)
{
  AdoptSize(index);
  fStage[index].AddLink(link);
}

void FairMCObject::AdoptSize(int index)
{
  int start = fStage.size();
  while (fStage.size() < index+1) {
    FairMCEntry myVec;
    myVec.SetPos(fStage.size());
    myVec.SetSource(GetStageId());
    myVec.SetPersistanceCheck(kFALSE);
    fStage.push_back(myVec);
    start++;
  }
}

FairMultiLinkedData FairMCObject::PosInList(FairLink link)
{
  FairMultiLinkedData result;
  result.SetPersistanceCheck(kFALSE);
  for (int i = 0; i < fStage.size(); i++) {
    if (fStage[i].IsLinkInList(link.GetType(), link.GetIndex())) {
      result.AddLink(FairLink(GetStageId(), i));
    }
  }
  return result;
}

void FairMCObject::RemoveType(Int_t type)
{
  for (int i = 0; i < fStage.size(); i++) {
    fStage[i].RemoveType(type);
  }
}
