 /*
 *  $Id: DmpAlgBgoMip.h, 2014-10-11 19:45:05 DAMPE $
 *  Author(s):
 *    yong(zhzhy@mail.ustc.edu.cn) 11/10/2014
*/
#include "DmpAlgBgoCutped.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "DmpDataBuffer.h"
#include "DmpBgoBase.h"
#include "TMath.h"
#include "DmpCore.h"

//-------------------------------------------------------------------
DmpAlgBgoCutped::DmpAlgBgoCutped()
 :DmpVAlg("BgoCutped"),
	fEvtHeader(0),
	fBgoRaw(0),
	fBgoPed(0)
{
        bool preparePedPar=GetPedPar();
	if(!preparePedPar){
	  std::cout<<"Error:Can not read PedPar!"<<std::endl;
	  }
}

//-------------------------------------------------------------------
DmpAlgBgoCutped::~DmpAlgBgoCutped(){
 }

bool DmpAlgBgoCutped::GetPedPar(){
  TFile *fPedPar=new TFile("./Pedestal/PedPar.root");
  TTree *Pedtree=(TTree*)fPedPar->Get("Calibration/Bgo");
  TBranch *b_fBgoPed;
  Pedtree->SetBranchAddress("Pedestal",&fBgoPed,&b_fBgoPed);
  Pedtree->GetEntry(0);
  //prepare parameters
short nDys=(short)fBgoPed->GlobalDynodeID.size();
   for(short i=0;i<nDys;++i){
    fGidOrder.insert(std::make_pair(fBgoPed->GlobalDynodeID[i],i));
  } 
//  delete Dactree;
//  delete fDacPar;
//  delete b_fBgoDac;
  //usage: QdcCoe[fGidOrder[gid]];//Quadratic Coefficients
  //       Slope[...],Cst[...] are same.
  return true;
}


//-------------------------------------------------------------------
bool DmpAlgBgoCutped::Initialize(){
  fEvtHeader = dynamic_cast<DmpEvtHeader*>(gDataBuffer->ReadObject("Event/Dacor/EventHeader"));
  if(fEvtHeader==0){
    fEvtHeader= new DmpEvtHeader();
    gDataBuffer->LinkRootFile("Event/Rdc/EventHeader",fEvtHeader);
  }

  fBgoRaw = dynamic_cast<DmpEvtBgoRaw*>(gDataBuffer->ReadObject("Event/Dacor/Bgo"));
  if(fBgoRaw==0){
    fBgoRaw= new DmpEvtBgoRaw();
    gDataBuffer->LinkRootFile("Event/Rdc/Bgo",fBgoRaw);
  }
  
  gDataBuffer->RegisterObject("Event/Cutped/EventHeader",fEvtHeader,"DmpEvtHeader");
  gDataBuffer->RegisterObject("Event/Cutped/Bgo",fBgoRaw,"DmpEvtBgoRaw");
  return true;
}

//-------------------------------------------------------------------
bool DmpAlgBgoCutped::ProcessThisEvent(){
 if(fEvtHeader->EnabledPeriodTrigger()&&fEvtHeader->GeneratedPeriodTrigger()){
  return false;
  }
  short nSignal = fBgoRaw->fGlobalDynodeID.size();
  short gid = 0;
  double adc =0.;
  for (short i=0;i<nSignal;++i){
    gid=fBgoRaw->fGlobalDynodeID[i];
    adc=fBgoRaw->fADC[i]; 
    int n=(int)fGidOrder[gid];
    if(adc>(fBgoPed->Mean[n]+3*fBgoPed->Sigma[n])){
      adc=adc-fBgoPed->Mean[n];
      fBgoRaw->fADC[i]=adc;
    }
    else {
      fBgoRaw->fGlobalDynodeID.erase(fBgoRaw->fGlobalDynodeID.begin()+i);
      fBgoRaw->fADC.erase(fBgoRaw->fADC.begin()+i);
      nSignal--;
      i--;
     }
  }
  return true;
}

//-------------------------------------------------------------------
bool DmpAlgBgoCutped::Finalize(){
  return true;
}

