/* \class HLTMuonL1TFilter
 *
 * This is an HLTFilter implementing filtering on L1T Stage2 GMT objects
 * 
 * \author:  V. Rekovic
 *
 */

#include "HLTrigger/Muon/interface/HLTMuonL1TFilter.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerRefsCollections.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Utilities/interface/EDMException.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "TMath.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <vector>

HLTMuonL1TFilter::HLTMuonL1TFilter(const edm::ParameterSet& iConfig) : HLTFilter(iConfig),
  candTag_( iConfig.getParameter<edm::InputTag>("CandTag") ),
  candToken_( consumes<l1t::MuonBxCollection>(candTag_)),
  previousCandTag_( iConfig.getParameter<edm::InputTag>("PreviousCandTag") ),
  previousCandToken_( consumes<trigger::TriggerFilterObjectWithRefs>(previousCandTag_)),
  maxEta_( iConfig.getParameter<double>("MaxEta") ),
  minPt_( iConfig.getParameter<double>("MinPt") ),
  minN_( iConfig.getParameter<int>("MinN") )
{

  // dump parameters for debugging
  if(edm::isDebugEnabled()){
    LogTrace("HLTMuonL1TFilter")
    <<"Constructed with parameters:"<<endl
    <<"    CandTag = "<<candTag_.encode()<<endl
    <<"    PreviousCandTag = "<<previousCandTag_.encode()<<endl
    <<"    MaxEta = "<<maxEta_<<endl
    <<"    MinPt = "<<minPt_<<endl;
    LogTrace("HLTMuonL1TFilter")<< endl;
  }

}


HLTMuonL1TFilter::~HLTMuonL1TFilter()
{
}


 
void
HLTMuonL1TFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  makeHLTFilterDescription(desc);
  desc.add<edm::InputTag>("CandTag",edm::InputTag("hltGmtStage2Digis"));
  desc.add<edm::InputTag>("PreviousCandTag",edm::InputTag(""));
  desc.add<double>("MaxEta",2.5);
  desc.add<double>("MinPt",0.0);
  desc.add<int>("MinN",1);
  descriptions.add("hltMuonL1TFilter",desc);
}

bool HLTMuonL1TFilter::hltFilter(edm::Event& iEvent, const edm::EventSetup& iSetup, trigger::TriggerFilterObjectWithRefs & filterproduct) const {
  using namespace std;
  using namespace edm;
  using namespace trigger;
  using namespace l1t;

  // All HLT filters must create and fill an HLT filter object,
  // recording any reconstructed physics objects satisfying (or not)
  // this HLT filter, and place it in the Event.

  // get hold of all muons
  Handle<MuonBxCollection> allMuons;
  iEvent.getByToken(candToken_, allMuons);

  // get hold of TFOWRs that fired the previous level
  Handle<TriggerFilterObjectWithRefs> previousLevelTFOWR;
  iEvent.getByToken(previousCandToken_, previousLevelTFOWR);

  vector<MuonRef> prevMuons;
  previousLevelTFOWR->getObjects(TriggerL1Mu, prevMuons);

  // look at all muon candidates, check cuts and add to filter object
  int n = 0;
  for(size_t i = 0; i < allMuons->size(); i++){
    
    MuonRef muon(allMuons, i);

    // Only select muons that were selected in the previous level 
    if(find(prevMuons.begin(), prevMuons.end(), muon) == prevMuons.end()) continue;

    //check maxEta cut
    if(fabs(muon->eta()) > maxEta_) continue;

    //check pT cut
    if(muon->pt() < minPt_) continue;

    //we have a good candidate
    n++;
    filterproduct.addObject(TriggerL1Mu,muon);

  }

  if (saveTags()) filterproduct.addCollectionTag(candTag_);

  // filter decision
  const bool accept(n >= minN_);

  // dump event for debugging
  if(edm::isDebugEnabled()){

    LogTrace("HLTMuonL1TFilter")<<"\nHLTMuonL1TFilter -----------------------------------------------" << endl;
    LogTrace("HLTMuonL1TFilter")<<"Decision of filter is "<<accept<<", number of muons passing = "<<filterproduct.l1tmuonSize() << endl;
    LogTrace("HLTMuonL1TFilter")<<"L1mu#"<<'\t'<<"q" << "\t" << "pt"<<'\t'<<'\t'<<"eta"<<'\t'<<"phi" "\t (|maxEta| = " << maxEta_ << ")" <<endl;
    LogTrace("HLTMuonL1TFilter")<<"--------------------------------------------------------------------------"<<endl;

    vector<MuonRef> firedMuons;
    filterproduct.getObjects(TriggerL1Mu, firedMuons);
    for(size_t i=0; i<firedMuons.size(); i++){
      l1t::MuonRef mu = firedMuons[i]; 
      LogTrace("HLTMuonL1TFilter")<<i<<'\t'<<setprecision(2) << scientific<<mu->charge() <<'\t' << mu->pt()<<'\t'<<fixed<<mu->eta()<<'\t'<<mu->phi()<<endl;
    }
    LogTrace("HLTMuonL1TFilter")<<"--------------------------------------------------------------------------"<<endl;
    LogTrace("HLTMuonL1TFilter")<<"Decision of this filter is "<<accept<<", number of muons passing = "<<filterproduct.l1tmuonSize();
  }

  return accept;
}
