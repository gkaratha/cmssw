import FWCore.ParameterSet.Config as cms
from L1Trigger.VertexFinder.VertexProducer_cff import VertexProducer

L1TrackJetsEmulation = cms.EDProducer('L1TrackJetEmulatorProducer',
	L1TrackInputTag= cms.InputTag("L1GTTInputProducer", "Level1TTTracksConverted"),
        #L1TrackInputTag= cms.InputTag("L1TrackSelectionProducer", "Level1TTTracksSelectedEmulation"),       
        VertexInputTag=cms.InputTag("VertexProducerEmu","l1verticesEmulation"),
        MaxDzTrackPV = cms.double(1.0),
        trk_zMax = cms.double (15.) ,    # maximum track z
	trk_ptMax = cms.double(200.),    # maximumum track pT before saturation [GeV]
	trk_ptMin = cms.double(2.0),     # minimum track pt [GeV]
   	trk_etaMax = cms.double(2.4),    # maximum track eta
        nStubs4PromptChi2=cms.double(10.0), #Prompt track quality flags for loose/tight
        nStubs4PromptBend=cms.double(2.2),
        nStubs5PromptChi2=cms.double(10.0),
        nStubs5PromptBend=cms.double(2.2),
	trk_nPSStubMin=cms.int32(-1),    # minimum PS stubs, -1 means no cut
	minTrkJetpT=cms.double(-1.),      # minimum track pt to be considered for track jet
	etaBins=cms.int32(24),
	phiBins=cms.int32(27),
	zBins=cms.int32(1),
        d0_cutNStubs4=cms.double(-1),
        d0_cutNStubs5=cms.double(-1),
	lowpTJetMinTrackMultiplicity=cms.int32(2),
        lowpTJetThreshold=cms.double(50.),
	highpTJetMinTrackMultiplicity=cms.int32(3),
        highpTJetThreshold=cms.double(100.),
	displaced=cms.bool(False), #Flag for displaced tracks
	nStubs4DisplacedChi2=cms.double(5.0), #Displaced track quality flags for loose/tight
	nStubs4DisplacedBend=cms.double(1.7),
	nStubs5DisplacedChi2=cms.double(2.75),
	nStubs5DisplacedBend=cms.double(3.5),
	nDisplacedTracks=cms.int32(2) #Number of displaced tracks required per jet
)

L1TrackJetsExtendedEmulation = cms.EDProducer('L1TrackJetEmulatorProducer',
	L1TrackInputTag= cms.InputTag("L1GTTInputProducerExtended", "Level1TTTracksExtendedConverted"),
        VertexInputTag=cms.InputTag("VertexProducerEmu", "l1verticesEmulation"),
	MaxDzTrackPV = cms.double(5.0),
        trk_zMax = cms.double (15.) ,    # maximum track z
	trk_ptMax = cms.double(200.),    # maximumum track pT before saturation [GeV]
	trk_ptMin = cms.double(3.0),     # minimum track pt [GeV]
   	trk_etaMax = cms.double(2.4),    # maximum track eta
	nStubs4PromptChi2=cms.double(10.0), #Prompt track quality flags for loose/tight
        nStubs4PromptBend=cms.double(2.2),
        nStubs5PromptChi2=cms.double(10.0),
        nStubs5PromptBend=cms.double(2.2),
	trk_nPSStubMin=cms.int32(-1),    # minimum # PS stubs, -1 means no cut
	minTrkJetpT=cms.double(5.),      # minimum track pt to be considered for track jet
	etaBins=cms.int32(24),
	phiBins=cms.int32(27),
	zBins=cms.int32(1),
	d0_cutNStubs4=cms.double(-1), # -1 excludes nstub=4 from disp tag
	d0_cutNStubs5=cms.double(0.22),
	lowpTJetMinTrackMultiplicity=cms.int32(2),
        lowpTJetThreshold=cms.double(50.),
	highpTJetMinTrackMultiplicity=cms.int32(3),
        highpTJetThreshold=cms.double(100.),
	displaced=cms.bool(True), #Flag for displaced tracks
	nStubs4DisplacedChi2=cms.double(3.3), #Disp tracks selection [trk<cut]
	nStubs4DisplacedBend=cms.double(2.3),
	nStubs5DisplacedChi2=cms.double(11.3),
	nStubs5DisplacedBend=cms.double(9.8),
	nDisplacedTracks=cms.int32(3) #min Ntracks to tag a jet as displaced
)
