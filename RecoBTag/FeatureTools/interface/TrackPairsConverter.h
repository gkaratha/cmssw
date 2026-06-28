#ifndef RecoBTag_FeatureTools_TrackPairsConverter_h
#define RecoBTag_FeatureTools_TrackPairsConverter_h

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/BTauReco/interface/TrackPairFeatures.h"
#include "RecoBTag/FeatureTools/interface/TrackPairInfoBuilder.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"


namespace btagbtvdeep {

   void trackPairsToFeatures(const reco::TransientTrack& track1,
                               const reco::TransientTrack& track2,
                               const reco::Jet& jet,
                               const reco::Vertex& pv,
                               TrackPairFeatures& tp_features);

}  // namespace btagbtvdeep

#endif  //RecoBTag_FeatureTools_TrackPairsConverter_h
