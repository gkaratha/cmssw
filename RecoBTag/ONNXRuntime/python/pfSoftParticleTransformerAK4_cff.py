import FWCore.ParameterSet.Config as cms

from RecoBTag.FeatureTools.pfSoftParticleTransformerAK4TagInfos_cfi import pfSoftParticleTransformerAK4TagInfos

from RecoBTag.ONNXRuntime.pfSoftParticleTransformerAK4JetTags_cfi import pfSoftParticleTransformerAK4JetTags as _pfSoftParticleTransformerAK4JetTags
from RecoBTag.ONNXRuntime.pfSoftParticleTransformerAK4DiscriminatorsJetTags_cfi import pfSoftParticleTransformerAK4DiscriminatorsJetTags
from CommonTools.PileupAlgos.Puppi_cff import puppi
from CommonTools.RecoAlgos.primaryVertexAssociation_cfi import primaryVertexAssociation

pfSoftParticleTransformerAK4JetTags = _pfSoftParticleTransformerAK4JetTags.clone()

#print(pfSoftParticleTransformerAK4JetTags.flav_names)
#for disc in pfSoftParticleTransformerAK4DiscriminatorsJetTags.discriminators:
#    print(disc)
#print(pfSoftParticleTransformerAK4DiscriminatorsJetTags.discriminators)

# declare all the discriminators
# probs
_pfSoftParticleTransformerAK4JetTagsProbs = ['pfSoftParticleTransformerAK4JetTags:' + flav_name
                                 for flav_name in pfSoftParticleTransformerAK4JetTags.flav_names]
# meta-taggers
_pfSoftParticleTransformerAK4JetTagsMetaDiscrs = ['pfSoftParticleTransformerAK4DiscriminatorsJetTags:' + disc.name.value()
                                      for disc in pfSoftParticleTransformerAK4DiscriminatorsJetTags.discriminators]
_pfSoftParticleTransformerAK4JetTagsAll = _pfSoftParticleTransformerAK4JetTagsProbs + _pfSoftParticleTransformerAK4JetTagsMetaDiscrs



# ==
# This task is not used, useful only if we run it from RECO jets (RECO/AOD)
pfSoftParticleTransformerAK4Task = cms.Task(puppi, primaryVertexAssociation,
                             pfSoftParticleTransformerAK4TagInfos, pfSoftParticleTransformerAK4JetTags,
                             pfSoftParticleTransformerAK4DiscriminatorsJetTags)
# run from MiniAOD instead
pfSoftParticleTransformerAK4FromMiniAODTask = cms.Task(pfSoftParticleTransformerAK4TagInfos,
                             pfSoftParticleTransformerAK4JetTags,
                             pfSoftParticleTransformerAK4DiscriminatorsJetTags)

# === Negative tags ===                                                                                                                                                      
pfNegativeSoftParticleTransformerAK4TagInfos = pfSoftParticleTransformerAK4TagInfos.clone(
    flip = True,
    secondary_vertices = 'inclusiveCandidateNegativeSecondaryVertices',
)
pfNegativeSoftParticleTransformerAK4JetTags = pfSoftParticleTransformerAK4JetTags.clone(
    src = 'pfNegativeParticleTransformerAK4TagInfos',
)

# probs                                                                                                                                                                      
_pfNegativeSoftParticleTransformerAK4JetTagsProbs = ['pfNegativeSoftParticleTransformerAK4JetTags:' + flav_name
                                 for flav_name in pfSoftParticleTransformerAK4JetTags.flav_names]
