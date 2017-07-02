//-------------------------------------------------
//
//   Class: DTLowQMatching
//
//   DTLowQMatching
//
//
//   Author :
//   G. Flouris               U Ioannina    Mar. 2015
//--------------------------------------------------

#ifndef DTLowQMatching_H
#define DTLowQMatching_H

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"

#include "CondFormats/L1TObjects/interface/L1TwinMuxParams.h"
#include "CondFormats/DataRecord/interface/L1TwinMuxParamsRcd.h"


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include <iostream>

class DTLowQMatching  {
public:
  DTLowQMatching(L1MuDTChambPhContainer* , L1MuDTChambPhContainer );
  ~DTLowQMatching() {};

  void run(const edm::EventSetup& c);

 edm::ESHandle< L1TwinMuxParams > tmParamsHandle;

  static int noRPCHits(L1MuDTChambPhContainer inCon, int bx, int wh, int sec, int st);

private:
  int deltaPhi(int dt_phi, int rpc_strip );

  void Matching(int track_seg);

  L1MuDTChambPhContainer* m_phiDTDigis;
  L1MuDTChambPhContainer m_phiRPCDigis;
  int m_DphiWindow;

};
#endif
