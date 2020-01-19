import FWCore.ParameterSet.Config as cms

muonTracks = cms.EDProducer("PATMuonTrackProducer",
                            generalTracks = cms.InputTag("generalTracks"),
                            slimmedMuons  = cms.InputTag("slimmedMuons"),
                            pfCandidates  = cms.InputTag("particleFlow"),
                            packedCandidates = cms.InputTag("packedPFCandidates"),
                            trk_selection = cms.string(' pt>3.0 && eta<2.4 && eta>-2.4')
                        )
