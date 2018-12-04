


import FWCore.ParameterSet.Config as cms


IsData=True
globaltag='101X_mc2017_realistic_TSG_2018_04_09_20_43_53'
HLTsave=False ; 
if IsData:
   print "We have established we Run on data"
   globaltag='101X_dataRun2_HLT_v7'
   HLTsave=True ; 
else:
   print "We have established we Run on MC"
print "Run parameters ",globaltag," HLT ",HLTsave

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")

#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(1)
#)

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag,globaltag,'')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
#'/store/mc/RunIIFall17DRPremix/BToJPsiKMu_JpsiMuMu_TuneCP5_13TeV-pythia8-evtgen/AODSIM/PU2017_94X_mc2017_realistic_v11-v1/910000/EC4C8C41-1149-E811-9973-C0BFC0E56836.root'
#'/store/cmst3/group/bpark/gkaratha/FastSim_GenBToKEE_NonD_muFilter_ForProbe_try1/BToKEE/CRAB3_fastsim/181021_215057/0000/B0tpDstarMu_fast_AOD_253.root'
#'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_61.root',
#'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_62.root',
#'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_63.root'
'/store/data/Run2018B/ParkingBPH5/AOD/PromptReco-v1/000/317/650/00000/E216E20E-E06E-E811-8F13-FA163E15F06E.root'
#'/store/relval/CMSSW_10_2_4_patch1/SingleMuon/RAW-RECO/ZMu-102X_dataRun2_PromptLike_v7_RelVal_sigMu2018B-v1/20000/FD9563D8-9350-784C-82D0-250AE00E4160.root'
#'/store/relval/CMSSW_10_1_4/SingleMuon/RECO/2018_06_05_23_31_HLTnewco_101X_dataRun2_HLT_DT_vDrift_Ttrig_w23_2018-v1/20000/FEF2A74C-896A-E811-BB1B-0CC47A4C8E86.root'
#'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_145.root'
#'/store/data/Run2018A/ParkingBPH6/AOD/14May2018-v1/10002/FEB93B72-7558-E811-A97B-3417EBE528B5.root'
#'/store/data/Run2018D/ParkingBPH5/AOD/PromptReco-v2/000/321/834/00000/FE1CD619-39AE-E811-9789-FA163EAA6035.root'
#'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_107.root',
#'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_108.root'
  ),
   secondaryFileNames=cms.untracked.vstring(
),
  # eventsToProcess=cms.untracked.VEventRange('317650:258:MIN-317650:308:MAX'),
   inputCommands=cms.untracked.vstring(
                  'keep *',
                  'drop *_ctppsPixelClusters_*_*',
                  
          )

)


#For Run A
#print "Obj ","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9IP5Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered7IP4Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered8Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered12Q"

#print "path ","HLT_Mu9_IP6_part","HLT_Mu8p5_IP3p5","HLT_Mu10p5_IP3p5","HLT_Mu8_IP3","empty","empty"

#foir Run B
print "Obj ","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9IP5Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered7IP4Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered8Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered12Q","empty"

print "path ","HLT_Mu9_IP6_part","HLT_Mu9_IP5","HLT_Mu7_IP4","HLT_Mu8_IP3","HLT_Mu12_IP6","empty"

process.demo = cms.EDFilter('LeptonSkimming',
                              beamSpot = cms.InputTag('offlineBeamSpot'),
                              electrons    = cms.InputTag("gedGsfElectrons"),
                              vertices     = cms.InputTag("offlinePrimaryVertices"),
                              conversions  = cms.InputTag('allConversions'),
                             
                      
                              #If you want no L1_Seed, write "default" in the first element and the tree will write the value -100
# RUN A obj.
#                               HLTFilter=cms.vstring("hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered8p5Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered10p5Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered8Q","empty","empty"),
# RUN B obj.
                              HLTFilter=cms.vstring("hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered9IP5Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered7IP4Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered8Q","hltL3fL1sMu22OrParkL1f0L2f10QL3Filtered12Q","empty"),
                          
# RUN A Paths
#                              HLTPath=cms.vstring("HLT_Mu9_IP6_part","HLT_Mu8p5_IP3p5","HLT_Mu10p5_IP3p5","HLT_Mu8_IP3","empty","empty"),
# RUN B Paths
                             HLTPath=cms.vstring("HLT_Mu9_IP6_part","HLT_Mu9_IP5","HLT_Mu7_IP4","HLT_Mu8_IP3","HLT_Mu12_IP6","empty"),
#                             HLTPath=cms.vstring("HLT_Mu9_IP6_part","empty","empty","empty","empty","empty"),
################################NORMALLY USE THIS####################### 
                              triggerresults = cms.InputTag("TriggerResults::HLT"),
                              triggerobjects = cms.InputTag('hltTriggerSummaryAOD','','HLT'),                   

#############################NORMAL RUN###################
                              muons=cms.InputTag("muons::RECO"),
#  muons=cms.InputTag("muons::reRECO"),                          
#########################NORMALLY USE THIS##################### 
#                             gen=cms.InputTag("genParticles::GEN"),
#########################ONLY FOR TTBAR-CMS AOD test#####################
                              gen=cms.InputTag("genParticles::HLT"),
##########################ONLY FOR TTBAR test up to here#####################
#                            gen=cms.InputTag("genParticles::RECO"),
############################FAST SIM SKATA###############################
############################NORMAL ####################
                               tracks=cms.InputTag("generalTracks::RECO"),
# tracks=cms.InputTag("generalTracks"),
#############################FAST SIM KOURADES
#                              tracks=cms.InputTag("generalTracks::HLT"),
                              clusters=cms.InputTag("particleFlowClusterECAL"),
                               
                               RunParameters = cms.PSet(
 
                   Data=cms.bool(IsData),SaveHLT=cms.bool(HLTsave),
#general
MuTrgMatchCone=cms.double(0.02),
#single track related
PtTrack_Cut=cms.double(0),EtaTrack_Cut=cms.double(2.5),TrackMuDz_Cut=cms.double(0.7),
MinChi2Track_Cut=cms.double(0),MaxChi2Track_Cut=cms.double(1000),
MuTrkMinDR_Cut=cms.double(0),TrackSdxy_Cut=cms.double(0),
TrgExclusionCone=cms.double(0.4),
#object sepecific
MuTrgExclusionCone=cms.double(0.4),ElTrgExclusionCone=cms.double(0.4),
TrkObjExclusionCone=cms.double(0.02),PtEl_Cut=cms.double(0),
PtMu_Cut=cms.double(0.0),QualMu_Cut=cms.double(0),MuTrgMuDz_Cut=cms.double(0.3),
ElTrgMuDz_Cut=cms.double(0.3),
#trkobj pair related
MaxMee_Cut=cms.double(5),EpairZvtx_Cut=cms.double(1000),
MinMee_Cut=cms.double(0),Cosee_Cut=cms.double(-10.99),
Probee_Cut=cms.double(0.0000000000000000000000000000000001),
MinMVA_Cut=cms.double(-10.4),MaxMVA_Cut=cms.double(-10.9),
ObjPtLargerThanTrack=cms.bool(False),
#triple track
TrkTrkMinDR_Cut=cms.double(0.02),PtKTrack_Cut=cms.double(0.4),
Ksdxy_Cut=cms.double(0),MaxMB_Cut=cms.double(6.0),MinMB_Cut=cms.double(3.5),
SLxy_Cut=cms.double(4.0),ProbeeK_Cut=cms.double(0.001),CoseeK_Cut=cms.double(0.9),
PtB_Cut=cms.double(5.0),
#configuration
SaveOnlyTracks=cms.bool(False),SaveOnlyEPairTracks=cms.bool(False),
UseOnlyBKeeMCForTriplets=cms.bool(False),EarlyStop=cms.bool(False),
SkipIfNoMuMatch=cms.bool(False),SaveOutputRoot=cms.bool(True)

 )
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    #SkipEvent = cms.untracked.vstring('ProductNotFound')
)


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string( "outputResTest.root" )
                                   )
process.fevt = cms.OutputModule("PoolOutputModule",
   # SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("path")),
    outputCommands = cms.untracked.vstring(#"drop *",
                                          # "keep *_*_*_Demo",
                                         #  "keep *_offlineBeamSpot_*_*",
                                         #  "keep *_offlinePrimaryVertices_*_*",
                                         #  "keep *_offlinePrimaryVerticesWithBS_*_*",
    ),
    fileName = cms.untracked.string("edm_output.root"))

#process.p = cms.Path(process.egmGsfElectronIDSequence)#* process.demo)
#process.endjob=cms.EndPath(process.fevt)
process.p = cms.Path(
   
   process.demo
  
)   
   
#process.p = cms.Path(process.demo)




'''
/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_1.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_2.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_3.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_4.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_5.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_6.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_7.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_8.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_9.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_10.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_11.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_12.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_13.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_14.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_15.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_16.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_17.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_18.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_19.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_20.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_21.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_22.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_23.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_24.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_25.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_26.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_27.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_28.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_29.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_30.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_31.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_32.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_33.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_34.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_35.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_36.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_37.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_38.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_39.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_40.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_41.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_42.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_43.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_44.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_45.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_46.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_47.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_48.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_49.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_50.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_51.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_52.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_53.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_54.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_55.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_56.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_57.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_58.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_59.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_60.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_61.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_62.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_63.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_64.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_65.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_66.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_67.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_68.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_69.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_70.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_71.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_72.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_73.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_74.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_75.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_76.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_77.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_78.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_79.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_80.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_81.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_82.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_83.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_84.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_85.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_86.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_87.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_88.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_89.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_90.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_91.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_92.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_93.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_94.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_95.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_96.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_97.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_98.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_99.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_100.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_101.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_102.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_103.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_104.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_105.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_106.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_107.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_108.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_109.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_110.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_111.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_112.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_113.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_114.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_115.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_116.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_117.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_118.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_119.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_120.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_121.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_122.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_123.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_124.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_125.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_126.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_127.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_128.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_129.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_130.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_131.root',

'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_133.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_134.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_135.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_136.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_137.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_138.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_139.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_140.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_141.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_142.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_143.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_144.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_145.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_146.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_147.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_148.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_149.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_150.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_151.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_152.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_153.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_154.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_155.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_156.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_157.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_158.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_159.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_160.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_161.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_162.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_163.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_164.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_165.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_166.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_167.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_168.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_169.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_170.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_171.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_172.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_173.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_174.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_175.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_176.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_177.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_178.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_179.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_180.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_181.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_182.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_183.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_184.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_185.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_186.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_187.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_188.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_189.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_190.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_191.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_192.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_193.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_194.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_195.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_196.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_197.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_198.root',
'/store/user/tstreble/BToKee_Pythia/BToKee_Pythia_AODSIM_18_03_22/180321_162718/0000/BToKee_AODSIM_199.root'
'''
