#ifndef __DataFormats_PatCandidates_GeneralTrack_h__
#define __DataFormats_PatCandidates_GeneralTrack_h__

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"



namespace pat {

  class GeneralTrack : public reco::LeafCandidate {
  public:

    enum { dimension = 5 };
    enum { covarianceSize = dimension * (dimension + 1) / 2 };
    typedef math::Error<dimension>::type CovarianceMatrix;

    
    GeneralTrack()
      : LeafCandidate(0, LorentzVector(0, 0, 0, 0)),
      cov_(CovarianceMatrix()),
      pfcand_(PackedCandidateRef()) {}

    explicit GeneralTrack(    const LorentzVector& p4,
			   const Point &reference,
                           int charge,
			   CovarianceMatrix cov,
                           const edm::Ref<pat::PackedCandidateCollection> refToPF
                            )
        : LeafCandidate(charge, p4, reference, 0),
      cov_(cov),
      pfcand_(refToPF) {}

    ~GeneralTrack() override {}

    const CovarianceMatrix covariance() const { return cov_ ;}
    const pat::PackedCandidateRef pfCand()  const { return pfcand_; }
  protected:
    CovarianceMatrix cov_;
    const edm::Ref<pat::PackedCandidateCollection> pfcand_;
  };
  typedef std::vector<GeneralTrack> GeneralTrackCollection;


}  // namespace pat

#endif
