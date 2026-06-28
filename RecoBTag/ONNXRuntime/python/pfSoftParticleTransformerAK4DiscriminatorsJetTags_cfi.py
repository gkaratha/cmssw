import FWCore.ParameterSet.Config as cms

pfSoftParticleTransformerAK4DiscriminatorsJetTags = cms.EDProducer(
   'BTagProbabilityToDiscriminator',
   discriminators = cms.VPSet(
      cms.PSet(
         name = cms.string('BvsAll'),
         numerator = cms.VInputTag(
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probb'),
            ),
         denominator=cms.VInputTag(
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probb'),
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probc'),
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probg'),
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probuds'),
         ),
      ),
      cms.PSet(
         name = cms.string('BvsL'),
         numerator = cms.VInputTag(
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probb'),
            ),
         denominator=cms.VInputTag(
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probuds'),
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probg'),
         ),
      ),
      cms.PSet(
         name = cms.string('CvsL'),
         numerator = cms.VInputTag(
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probc'),
            ),
         denominator = cms.VInputTag(
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probuds'),
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probg'),
            ),
         ),
      cms.PSet(
         name = cms.string('CvsB'),
         numerator = cms.VInputTag(
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probc'),
            ),
         denominator = cms.VInputTag(
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probb'),
            ),
         ),
      cms.PSet(
         name = cms.string('QvsG'),
         numerator = cms.VInputTag(
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probuds'),
            ),
         denominator = cms.VInputTag(
            cms.InputTag('pfSoftParticleTransformerAK4JetTags', 'probg'),
            ),
         ),

      )
   )
