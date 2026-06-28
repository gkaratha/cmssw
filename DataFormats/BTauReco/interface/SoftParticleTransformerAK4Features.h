#ifndef DataFormats_BTauReco_SoftParticleTransformerAK4Features_h
#define DataFormats_BTauReco_SoftParticleTransformerAK4Features_h

#include "DataFormats/BTauReco/interface/SecondaryVertexFeatures.h"
#include "DataFormats/BTauReco/interface/ChargedCandidateFeatures.h"
#include "DataFormats/BTauReco/interface/LostTracksFeatures.h"
#include "DataFormats/BTauReco/interface/TrackPairFeatures.h"
#include <vector>

namespace btagbtvdeep {

  class SoftParticleTransformerAK4Features {
  public:
    bool is_filled = true;
    std::vector<ChargedCandidateFeatures> c_pf_features;
    std::vector<SecondaryVertexFeatures> sv_features;
    std::vector<LostTracksFeatures> lt_features;
    std::vector<TrackPairFeatures> pair_features;
  };

}  // namespace btagbtvdeep

#endif  //DataFormats_BTauReco_SoftParticleTransformerAK4Features_h
