#ifndef src_PhysicsTools_MuonRefProducer_h
#define src_PhysicsTools_MuonRefProducer_h
// -*- C++ -*-
//
// Package:     PhysicsTools
// Class  :     MuonRefProducer
// 
/**\class MuonRefProducer MuonRefProducer.h src/PhysicsTools/interface/MuonRefProducer.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Valerie Halyo
//         Created:  Wed Oct  8 11:08:22 CDT 2008
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HLTReco/interface/TriggerObject.h" 
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h" 
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h" 
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"


//
// class decleration
//

class MuonRefProducer : public edm::EDProducer 
{
   public:
      explicit MuonRefProducer(const edm::ParameterSet&);
      ~MuonRefProducer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      

      // ----------member data ---------------------------
      
      edm::InputTag probeCollection_;

      bool   useCharge_;
      double ptMin_;
      int    charge_;
      int    nhits_;
      double nchi2_;
      double d0_;
      double z0_;
      double dz0_;
 
      // ----------member data ---------------------------
};




#endif
