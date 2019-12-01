#ifndef __DataFormats_PatCandidates_MuonTrack_h__
#define __DataFormats_PatCandidates_MuonTrack_h__

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

namespace pat {

  class MuonTrack : public reco::LeafCandidate {
  public:

    enum { dimension = 5 };
    enum { covarianceSize = dimension * (dimension + 1) / 2 };
    typedef math::Error<dimension>::type CovarianceMatrix;

    
    MuonTrack()
      : LeafCandidate(0, LorentzVector(0, 0, 0, 0)),
      cov_(CovarianceMatrix()),
      muon_(MuonRef()) {}

    explicit MuonTrack(    const LorentzVector& p4,
			   const Point &reference,
                           int charge,
			   CovarianceMatrix cov,
			   const edm::Ref<pat::MuonCollection> refToMuon)
        : LeafCandidate(charge, p4, reference, 0),
      cov_(cov),
      muon_(refToMuon) {}

    ~MuonTrack() override {}

    const pat::MuonRef muon()  const { return muon_; }
    const CovarianceMatrix covariance() const { return cov_ ;}
  protected:
    CovarianceMatrix cov_;
    const edm::Ref<pat::MuonCollection> muon_;
  };
  typedef std::vector<MuonTrack> MuonTrackCollection;


}  // namespace pat

#endif
