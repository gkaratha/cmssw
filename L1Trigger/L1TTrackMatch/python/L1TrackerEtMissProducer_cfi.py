import FWCore.ParameterSet.Config as cms
from L1Trigger.VertexFinder.VertexProducer_cff import VertexProducer

L1TrackerEtMiss = cms.EDProducer('L1TrackerEtMissProducer',
    L1TrackInputTag = cms.InputTag("TTTracksFromTrackletEmulation", "Level1TTTracks"),
    L1VertexInputTag = cms.InputTag("VertexProducer", VertexProducer.l1VertexCollectionName.value()),
    L1MetCollectionName = cms.string("L1TrackerEtMiss"),
    maxZ0 = cms.double ( 15. ) ,    # in cm
    maxEta = cms.double ( 2.4 ) ,   # max eta allowed for chosen tracks
    chi2dofMax = cms.double( 100. ), # max chi2/dof allowed for chosen tracks
    splitChi2dofMax = cms.double( 10. ), # max chi2/dof allowed for chosen tracks
    bendChi2Max = cms.double( 2. ),# max bendchi2 allowed for chosen tracks
    minPt = cms.double( 2. ),       # in GeV
    deltaZ = cms.double( 3. ),      # in cm
    nStubsmin = cms.int32( 4 ),     # min number of stubs for the tracks
    nPSStubsMin = cms.int32( -1 ),  # min number of stubs in the PS Modules
    maxPt = cms.double( -10. ),	    # in GeV. When maxPt > 0, tracks with PT above maxPt are considered as
                                    # mismeasured and are treated according to highPtTracks below.
                                    # When maxPt < 0, no special treatment is done for high PT tracks.
    highPtTracks = cms.int32( 1 ),  # when = 0 : truncation. Tracks with PT above maxPt are ignored
                                    # when = 1 : saturation. Tracks with PT above maxPt are set to PT=maxPt.
                                    # When maxPt < 0, no special treatment is done for high PT tracks.
    displaced = cms.bool(False),     # Use promt/displaced tracks
    
    debug     = cms.bool(True)
)

L1TrackerEtMissExtended = cms.EDProducer('L1TrackerEtMissProducer',
    L1TrackInputTag = cms.InputTag("TTTracksFromExtendedTrackletEmulation", "Level1TTTracks"),
    L1VertexInputTag = cms.InputTag("VertexProducer", VertexProducer.l1VertexCollectionName.value()),
    L1MetCollectionName = cms.string("L1TrackerEtMiss"),
    L1MetExtendedCollectionName = cms.string("L1TrackerExtendedEtMiss"),
    maxZ0 = cms.double ( 15. ) ,    # in cm
    maxEta = cms.double ( 2.4 ) ,   # max eta allowed for chosen tracks
    chi2dofMax = cms.double( 40. ), # max chi2/dof allowed for chosen tracks
    splitChi2dofMax = cms.double( 10. ), # max chi2/dof allowed for chosen tracks
    bendChi2Max = cms.double( 2.4 ),# max bendchi2 allowed for chosen tracks
    minPt = cms.double( 3. ),       # in GeV
    deltaZ = cms.double( 3.0 ),     # in cm
    nStubsmin = cms.int32( 4 ),     # min number of stubs for the tracks
    nPSStubsMin = cms.int32( -1 ),  # min number of stubs in the PS Modules
    maxPt = cms.double( -10. ),	    # in GeV. When maxPt > 0, tracks with PT above maxPt are considered as
                                    # mismeasured and are treated according to highPtTracks below.
                                    # When maxPt < 0, no special treatment is done for high PT tracks.
    highPtTracks = cms.int32( 1 ),  # when = 0 : truncation. Tracks with PT above maxPt are ignored
                                    # when = 1 : saturation. Tracks with PT above maxPt are set to PT=maxPt.
                                    # When maxPt < 0, no special treatment is done for high PT tracks.
    displaced = cms.bool(True),      # Use promt/displaced tracks

    debug     = cms.bool(False)
)
