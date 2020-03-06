import FWCore.ParameterSet.Config as cms

slimmedGeneralTracks = cms.EDProducer("PATGeneralTrackSlimmer",
                            generalTracks = cms.InputTag("generalTracks"),
                            pfCandidates  = cms.InputTag("particleFlow"),
                            packedCandidates = cms.InputTag("packedPFCandidates"),
                            trk_selection = cms.string(' pt>3.0 && eta<2.4 && eta>-2.4')
                        )
