#ifndef DataFormats_BTauReco_SoftParticleTransformerAK4TagInfo_h
#define DataFormats_BTauReco_SoftParticleTransformerAK4TagInfo_h

#include "DataFormats/BTauReco/interface/SoftParticleTransformerAK4Features.h"
#include "DataFormats/BTauReco/interface/FeaturesTagInfo.h"

namespace reco {

  typedef FeaturesTagInfo<btagbtvdeep::SoftParticleTransformerAK4Features> SoftParticleTransformerAK4TagInfo;

  DECLARE_EDM_REFS(SoftParticleTransformerAK4TagInfo)

}  // namespace reco

#endif  // DataFormats_BTauReco_SoftParticleTransformerAK4TagInfo_h
