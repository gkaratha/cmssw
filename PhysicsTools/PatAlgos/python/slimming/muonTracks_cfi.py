import FWCore.ParameterSet.Config as cms

muonTracks = cms.EDProducer("PATMuonTrackProducer",
                            generalTracks = cms.InputTag("generalTracks"),
                            slimmedMuons  = cms.InputTag("slimmedMuons"),
                            pT_cut        = cms.double(3.0),
                        )
