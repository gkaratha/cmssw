#include <string>

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/PatCandidates/interface/MuonTrack.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"

#include "MagneticField/Engine/interface/MagneticField.h"

namespace pat {

  class PATMuonTrackProducer : public edm::stream::EDProducer<> {
  public:
    typedef pat::MuonTrack::LorentzVector LorentzVector;

    explicit PATMuonTrackProducer(const edm::ParameterSet&);
    ~PATMuonTrackProducer() override;

    void produce(edm::Event&, const edm::EventSetup&) override;

  private:
    const edm::EDGetTokenT<reco::TrackCollection> gt_;
    const edm::EDGetTokenT<pat::MuonCollection> mu_;
    const float pT_cut_;

  };
}  // namespace pat

pat::PATMuonTrackProducer::PATMuonTrackProducer(const edm::ParameterSet& iConfig)
  :   gt_(consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("generalTracks"))),
      mu_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("slimmedMuons"))),
      pT_cut_(iConfig.getParameter<double>("pT_cut"))
{
  produces<pat::MuonTrackCollection>();

}

pat::PATMuonTrackProducer::~PATMuonTrackProducer() {}

void pat::PATMuonTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  auto outPtrP = std::make_unique<std::vector<pat::MuonTrack>>();


  // generalTracks collection
  edm::Handle<reco::TrackCollection> gt_h;
  iEvent.getByToken(gt_, gt_h);

  // slimmedMuons collection
  edm::Handle<pat::MuonCollection> muons;
  iEvent.getByToken(mu_, muons);

  const reco::TrackCollection* generalTracks = gt_h.product();

  
  for (unsigned int igt = 0; igt < generalTracks->size(); igt++) {
    const reco::Track& gentk = (*gt_h)[igt];
    if (gentk.pt() < pT_cut_) continue;

    reco::TrackRef trackerTrackRef(generalTracks, igt);    
    
    LorentzVector p4;
    double m = 0.105;  //assume muon mass
    double E = sqrt(m * m + gentk.p() * gentk.p());
    p4.SetPxPyPzE(gentk.px(), gentk.py(), gentk.pz(), E);


    pat::MuonRef muonRef;

    // match to slimmed muons 
    for (unsigned int imu = 0; imu < muons->size(); imu++) {
      const pat::Muon& muon = (*muons)[imu];
      if (muon.track() == trackerTrackRef){
	muonRef = pat::MuonRef(muons, imu);
	break;
      }
	
    }

    outPtrP->push_back(pat::MuonTrack(p4,
				      gentk.referencePoint(),
				      gentk.charge(),
				      gentk.covariance(),
				      muonRef
				      ));

  }
  auto orphHandle = iEvent.put(std::move(outPtrP));

}






using pat::PATMuonTrackProducer;
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PATMuonTrackProducer);
